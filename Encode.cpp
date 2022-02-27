#include "Encode.h"

void MakeCanonic(set<pair<size_t, int32_t>>& len_of_codes, map<int32_t, vector<int8_t>>& codes) {
    vector<int8_t> cur;
    cur.push_back(0);
    for (auto i : len_of_codes) {
        while (i.first > cur.size()) {
            cur.push_back(0);
        }
        codes[i.second] = cur;
        std::reverse(cur.begin(), cur.end());
        ++cur[0];
        for (size_t j = 0; j < cur.size(); ++j) {
            if (j + 1 < cur.size()) {
                cur[j + 1] += cur[j] / 2;
                cur[j] %= 2;
            } else {
                if (cur[j] / 2 > 0) {
                    cur.push_back(cur[j] / 2);
                    cur[j] %= 2;
                }
            }
        }
        std::reverse(cur.begin(), cur.end());
    }
}
vector<int8_t> CodeOfSize(size_t x, size_t min_len) {
    vector<int8_t> ans;
    while (x > 0) {
        ans.push_back(static_cast<int8_t>(x % 2));
        x /= 2;
    }
    while (ans.size() < min_len) {
        ans.push_back(0);
    }
    return ans;
}
Encode::Encode() {
}
Encode::Encode(const vector<string>& files) {
    for (size_t p = 0; p < files.size(); ++p) {
        string file_name = files[p];
        map<int32_t, int32_t> cnt;
        for (auto i : file_name) {
            ++cnt[i];
        }
        ++cnt[FILENAME_END];
        std::ifstream fin(file_name);
        string file;
        unsigned char c;
        while (fin.read(reinterpret_cast<char*>(&c), 1)) {
            ++cnt[c];
            file += c;
        }
        if (p + 1 < files.size()) {
            ++cnt[ONE_MORE_FILE];
        } else {
            ++cnt[ARCHIVE_END];
        }
        std::priority_queue<Huffman> q;
        for (auto i : cnt) {
            q.push(Huffman(i.first, i.second));
        }
        Huffman root = BuildHuffman(q);
        set<pair<size_t, int32_t>> len_of_codes;
        GoingDown(root, len_of_codes);
        map<int32_t, vector<int8_t>> codes;
        MakeCanonic(len_of_codes, codes);
        size_t symbols_count = cnt.size();
        // SYMBOLS_COUNT
        auto cur_viv = CodeOfSize(symbols_count, 9);
        for (auto i : cur_viv) {
            archive_code_.push_back(i);
        }
        // ALPHABET
        for (auto i : len_of_codes) {
            cur_viv = CodeOfSize(static_cast<size_t>(i.second), 9);
            for (auto j : cur_viv) {
                archive_code_.push_back(j);
            }
        }
        size_t max_symbol_code_size = 0;
        vector<size_t> num_of_lens(10);
        for (auto i : len_of_codes) {
            ++num_of_lens[i.first];
            max_symbol_code_size = std::max(max_symbol_code_size, i.first);
        }
        for (size_t i = 1; i <= max_symbol_code_size; ++i) {
            cur_viv = CodeOfSize(num_of_lens[i], 9);
            for (auto j : cur_viv) {
                archive_code_.push_back(j);
            }
        }
        for (auto i : file_name) {
            cur_viv = codes[i];
            for (auto j : cur_viv) {
                archive_code_.push_back(j);
            }
        }
        cur_viv = codes[FILENAME_END];
        for (auto j : cur_viv) {
            archive_code_.push_back(j);
        }
        for (auto i : file) {
            cur_viv = codes[i];
            for (auto j : cur_viv) {
                archive_code_.push_back(j);
            }
        }
        if (p + 1 < files.size()) {
            cur_viv = codes[ONE_MORE_FILE];
        } else {
            cur_viv = codes[ARCHIVE_END];
        }
        for (auto j : cur_viv) {
            archive_code_.push_back(j);
        }
    }
}
void Encode::Print(const string& archive_name) {
    std::ofstream fout(archive_name);
    if (!fout) {
        std::cout << "Archive file is not opened!\n";
        return;
    }
    int32_t buffer = 0;
    int32_t buffer_size = 0;
    while (static_cast<int32_t>(archive_code_.size()) % SIZE_OF_CHAR != 0) {
        archive_code_.push_back(0);
    }
    for (auto i : archive_code_) {
        buffer += (i << buffer_size);
        ++buffer_size;
        if (buffer_size == SIZE_OF_CHAR) {
            unsigned char viv = static_cast<unsigned char>(buffer);
            fout << viv;
            buffer = 0;
            buffer_size = 0;
        }
    }
}
