#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "Huffman.h"
#include <map>
#include <fstream>
#include <set>
#include <utility>
#include <algorithm>

using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

const int32_t FILENAME_END = 256, ONE_MORE_FILE = 257, ARCHIVE_END = 258;
const int32_t SIZE_OF_CHAR = 8;

void MakeCanonic(set<pair<size_t, int32_t> >& len_of_codes, map<int32_t, vector<int8_t> >& codes);

vector<int8_t> CodeOfSize(size_t x, size_t min_len);

class Encode {
public:
    Encode();
    Encode(const vector<string>& files);
    void Print(const string& archive_name);

private:
    vector<int8_t> archive_code_;
};
