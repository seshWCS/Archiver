//
// Created by Александр Нам on 16.10.2021.
//

#include "Decode.h"
void Decode(const string& archive_name) {
    std::ifstream fin(archive_name);
    vector<int32_t> archive;
    unsigned char c;
    while (fin.read(reinterpret_cast<char*>(&c), 1)) {
        archive.push_back(c);
    }
    vector<int8_t> archive_code;
    for (auto i : archive) {
        int32_t cur = i;
        for (int32_t j = 0; j < SIZE_OF_CHAR; ++j) {
            archive_code.push_back(cur % 2);
            cur /= 2;
        }
    }
    size_t cur_p = 0;

    while (true) {
        int32_t symbols_count = 0;
        size_t limit = cur_p + 9;
        int32_t cur_st = 1;
        for (; cur_p < limit; ++cur_p) {
            symbols_count += archive_code[cur_p] * cur_st;
            cur_st *= 2;
        }
        vector<int32_t> alphabet;
        for (int32_t i = 0; i < symbols_count; ++i) {
            int32_t cur_symbol = 0;
            limit = cur_p + 9;
            cur_st = 1;
            for (; cur_p < limit; ++cur_p) {
                cur_symbol += archive_code[cur_p] * cur_st;
                cur_st *= 2;
            }
            alphabet.push_back(cur_symbol);
        }
        vector<int32_t> num_of_lens(10, 0);
        int32_t cur_len = 1;
        int32_t sum = 0;
        while (sum < symbols_count) {
            int32_t num_of_cur_len = 0;
            limit = cur_p + 9;
            cur_st = 1;
            for (; cur_p < limit; ++cur_p) {
                num_of_cur_len += archive_code[cur_p] * cur_st;
                cur_st *= 2;
            }
            num_of_lens[cur_len] = num_of_cur_len;
            sum += num_of_cur_len;
            ++cur_len;
        }
        set<pair<size_t, int32_t>> len_of_codes;
        for (auto i : alphabet) {
            for (size_t j = 1; j < 10; ++j) {
                if (num_of_lens[j] > 0) {
                    len_of_codes.insert(std::make_pair(j, i));
                    --num_of_lens[j];
                    break;
                }
            }
        }
        map<int32_t, vector<int8_t>> codes1;
        MakeCanonic(len_of_codes, codes1);
        map<vector<int8_t>, int32_t> codes;
        for (auto& i : codes1) {
            codes[i.second] = i.first;
        }
        vector<int8_t> cur_code;
        string file_name;
        while (true) {
            cur_code.push_back(archive_code[cur_p]);
            ++cur_p;
            if (codes.find(cur_code) != codes.end()) {
                if (codes[cur_code] == FILENAME_END) {
                    cur_code.clear();
                    break;
                } else {
                    file_name += static_cast<char>(codes[cur_code]);
                    cur_code.clear();
                }
            }
        }
        std::ofstream fout(file_name);
        bool ended = false;
        while (true) {
            cur_code.push_back(archive_code[cur_p]);
            ++cur_p;
            if (codes.find(cur_code) != codes.end()) {
                if (codes[cur_code] == ARCHIVE_END) {
                    ended = true;
                    cur_code.clear();
                    break;
                } else if (codes[cur_code] == ONE_MORE_FILE) {
                    cur_code.clear();
                    break;
                } else {
                    fout << static_cast<char>(codes[cur_code]);
                    cur_code.clear();
                }
            }
        }
        if (ended) {
            break;
        }
    }
}
