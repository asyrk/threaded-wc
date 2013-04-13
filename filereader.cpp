#include "filereader.h"

void FileReader::processChunk(ChunkInfo *chunk)
{
    char* buf = new char[BUF_SIZE];

    mutex.lock();
    fseek(file, chunk->offset, SEEK_SET);
    int read_bytes = fread(buf, sizeof(char), BUF_SIZE, file);
    mutex.unlock();

    chunk->bytes = read_bytes;
    chunk->setBuffer(buf);
    char* tmp = chunk->getBuffer();
    int tmpline = 0;
    for(int n = 0; n < chunk->bytes; ++n)
    {
        chunk->chars++;
        tmpline++;
        if(isspace(tmp[n]) && (n > 0 ? !isspace(tmp[n-1]) : 0))
        {
            chunk->words++;
        }
        if(tmp[n] == '\n')
        {
            chunk->lines++;
            mutex.lock();
            maxLine = tmpline > maxLine ? tmpline : maxLine;
            mutex.unlock();
            tmpline = 0;
        }
    }

    mutex.lock();
    total_bytes += chunk->bytes;
    total_words += chunk->words;
    total_lines += chunk->lines;
    total_chars += chunk->chars;
    mutex.unlock();
}

FileReader::FileReader() :
    total_words(0),
    total_chars(0),
    total_bytes(0),
    total_lines(0),
    maxLine(0)
{
}

FileReader::~FileReader()
{
    for(int i = 0; i < n_chunks; ++i)
    {
        delete chunks[i].getBuffer();
    }
    delete chunks;
}

bool FileReader::initialize(char *fname)
{
    file = fopen(fname, "rb");
    if(!file)
        return false;
    fseek(file, 0, SEEK_END);
    n_chunks = std::ceil((double)ftell(file) / (double)BUF_SIZE);
    chunks = new ChunkInfo[n_chunks];
    rewind(file);
    for(int i = 0; i < n_chunks; ++i)
    {
        chunks[i].setFilename(fname);
        chunks[i].offset = i * BUF_SIZE;
    }
    return true;
}


void FileReader::read()
{
    for(int i = 0; i < n_chunks; ++i)
    {
        threads.add_thread(new boost::thread(boost::bind(&FileReader::processChunk, this, &chunks[i])));
    }
}

int FileReader::getMaxLine() const
{
    return maxLine;
}


int FileReader::getWords() const
{
    return total_words;
}

void FileReader::joinAll()
{
    threads.join_all();
}


int FileReader::getChars() const
{
    return total_chars;
}


int FileReader::getBytes() const
{
    return total_bytes;
}


int FileReader::getLines() const
{
    return total_lines;
}

char* FileReader::getFilename() const
{
    return chunks[0].getFilename();
}
