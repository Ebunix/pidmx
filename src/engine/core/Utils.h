#pragma once
#include <vector>
#include <algorithm>

template<typename T, typename Pred>
void Filter(const std::vector<T> &in, std::vector<T> &out, Pred p) {
    std::copy_if(in.begin(), in.end(), std::back_inserter(out), p);
}


