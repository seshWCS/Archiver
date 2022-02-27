#include "Huffman.h"

Huffman::Huffman() {
}
Huffman::Huffman(int32_t val, int32_t prior, std::shared_ptr<Huffman> left, std::shared_ptr<Huffman> right)
    : val_(val), prior_(prior), left_(left), right_(right) {
}
Huffman::Huffman(const Huffman& huffman)
    : val_(huffman.val_), prior_(huffman.prior_), left_(huffman.left_), right_(huffman.right_) {
}
Huffman Huffman::Merge(const Huffman& x) const {
    Huffman ans = Huffman(*this);
    ans.val_ += x.val_;
    ans.prior_ += x.prior_;
    ans.left_ = std::make_shared<Huffman>(*this);
    ans.right_ = std::make_shared<Huffman>(x);
    return ans;
}
Huffman& Huffman::operator=(const Huffman& x) {
    val_ = x.val_;
    prior_ = x.prior_;
    left_ = x.left_;
    right_ = x.right_;
    return *this;
}
bool Huffman::operator<(const Huffman& x) const {
    return std::make_pair(prior_, val_) > std::make_pair(x.prior_, x.val_);
}
std::shared_ptr<Huffman> Huffman::left() {
    return left_;
}
std::shared_ptr<Huffman> Huffman::right() {
    return right_;
}
int32_t Huffman::val() {
    return val_;
}
int32_t Huffman::prior() {
    return prior_;
}
Huffman BuildHuffman(std::priority_queue<Huffman>& q) {
    while (q.size() > 1) {
        auto x = q.top();
        q.pop();
        auto y = q.top();
        q.pop();
        q.push(x.Merge(y));
    }
    return q.top();
}
void GoingDown(Huffman& cur, std::set<std::pair<size_t, int32_t> >& m, size_t len) {
    if (cur.left() == nullptr) {
        m.insert(std::make_pair(len + 1, cur.val()));
    } else {
        GoingDown(*cur.left(), m, len + 1);
        GoingDown(*cur.right(), m, len + 1);
    }
}
