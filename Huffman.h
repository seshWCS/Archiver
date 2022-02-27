#pragma once

#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <queue>
#include <map>
#include <set>

class Huffman {
public:
    Huffman();
    Huffman(int32_t val, int32_t prior, std::shared_ptr<Huffman> left = nullptr,
            std::shared_ptr<Huffman> right = nullptr);
    Huffman(const Huffman& huffman);
    Huffman Merge(const Huffman& x) const;
    Huffman& operator=(const Huffman& x);
    bool operator<(const Huffman& x) const;
    std::shared_ptr<Huffman> left();
    std::shared_ptr<Huffman> right();
    int32_t val();
    int32_t prior();

private:
    int32_t val_;
    int32_t prior_;
    std::shared_ptr<Huffman> left_;
    std::shared_ptr<Huffman> right_;
};

Huffman BuildHuffman(std::priority_queue<Huffman>& q);

void GoingDown(Huffman& cur, std::set<std::pair<size_t, int32_t> >& m, size_t len = 0);
