#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <set>
#include <cstdlib>
#include "Encode.h"
#include "Decode.h"

using std::string;
using std::vector;

int main(int argc, char** argv) {
    if (strcmp(argv[0], "archiver") != 0) {
        exit(0);
    }
    if (strcmp(argv[1], "-c") == 0) {
        //Encode
        string archive_name = argv[2];
        vector<string> files;
        for (auto i = 3; i < argc; ++i) {
            files.push_back(argv[i]);
        }
        Encode c = Encode(files);
        c.Print(archive_name);
    } else if (strcmp(argv[1], "-d") == 0) {
        //Decode
        string archive_name = argv[2];
        Decode(archive_name);
    } else if (strcmp(argv[1], "-h") == 0) {
        //Help
        std::cout << "archiver -c archive_name file1 [file2 ...] - заархивировать файлы fil1, file2, ... и сохранить результат в файл archive_name.\n";
        std::cout << "archiver -d archive_name - разархивировать файлы из архива archive_name и положить в текущую директорию.\n";
        std::cout << "archiver -h - вывести справку по использованию программы.\n";
    }
}

