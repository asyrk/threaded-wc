#ifndef FILEREADER_H
#define FILEREADER_H

#include <iostream>
#include <cstdio>
#include <cmath>
#include <boost/thread.hpp>
#include "chunkinfo.h"

class FileReader {

private:
    ChunkInfo* chunks;
    FILE* file;
    int n_chunks;
    boost::thread_group threads;
    boost::mutex mutex;
    int total_words;
    int total_chars;
    int total_bytes;
    int total_lines;
    int maxLine;
    void processChunk(ChunkInfo *chunk);

public:
    static const int BUF_SIZE = 500;

    FileReader();
    ~FileReader();
    bool initialize(char* filename);
    void read();

    int getMaxLine() const;
    int getLines() const;
    int getBytes() const;
    int getChars() const;
    int getWords() const;
    char* getFilename() const;

    void joinAll();
};

#endif // FILEREADER_H
