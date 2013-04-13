#include <iostream>
#include <vector>
#include <getopt.h>
#include "filereader.h"

char* flags = "wlmchv";
void print_help_and_exit(char* name)
{
    std::cout << "usage: " << name << "-[" << flags << "] file1 [file2 ...]" << std::endl;
    exit(EXIT_SUCCESS);
}

void print_version_and_exit()
{
    std::cout << "wc-boost v.0.0, author Adam Syrek" <<std::endl;
    exit(EXIT_SUCCESS);
}

int main(int argc, char** argv)
{
    bool w = false, l = false, c = false;
    bool flag_set = false;
    int n_flags = 1;
    char flag;
    while((flag = getopt(argc, argv, flags)) != -1)
    {
        switch(flag)
        {
        case 'm':
        case 'c':
            c = true;
            n_flags++;
            flag_set = true;
            break;
        case 'l':
            l = true;
            n_flags ++;
            flag_set = true;
            break;
        case 'w':
            w = true;
            n_flags ++;
            flag_set = true;
            break;
        case 'h':
            print_help_and_exit(argv[0]);
            break;
        case 'v':
            print_version_and_exit();
            break;
        }
    }
    if(!flag_set)
    {
        w = true;
        l = true;
        c = true;
        argv++;
        argc--;
    }
    else{
        argv += optind;
        argc -= optind;
    }
    std::vector<FileReader*> vec;
    int wcount, lcount, ccount;
    wcount = lcount = ccount = 0;
    for(int i=0 ; i < argc; ++i)
    {
        FileReader* fr = new FileReader();
        if(!fr->initialize(argv[i]))
        {
            std::cout << "No such file: " << argv[i] <<std::endl;
            delete fr;
        } else
        {
            fr->read();
            vec.push_back(fr);
        }
    }
    std::for_each(vec.begin(), vec.end(), [](FileReader* fr){ fr->joinAll(); });
    std::for_each(vec.begin(), vec.end(),
                  [&](FileReader* fr){
        if(l) std::cout << fr->getLines() << "\t";
        if(w) std::cout << fr->getWords() << "\t";
        if(c) std::cout << fr->getChars() << "\t";
        std::cout << fr->getFilename() << '\n';
        wcount += fr->getWords();
        lcount += fr->getLines();
        ccount += fr->getChars();
        delete fr;
    });
    if(vec.size() > 1) {
        if(l) std::cout << lcount << "\t";
        if(w) std::cout << wcount << "\t";
        if(c) std::cout << ccount << "\t";
        std::cout << "total" << '\n';
    }
    exit(EXIT_SUCCESS);
}

