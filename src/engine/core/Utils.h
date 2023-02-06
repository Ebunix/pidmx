#pragma once
#include <vector>
#include <algorithm>

template<typename T, typename Pred>
void Filter(const Set<T> &in, Set<T> &out, Pred p) {
    std::copy_if(in.begin(), in.end(), std::back_inserter(out), p);
}

template<typename T, typename U, typename Pred>
void Filter(const Map<T, U> &in, Map<T, U> &out, Pred p) {
    std::copy_if(in.begin(), in.end(), std::inserter(out, std::next(out.begin())), p);
}


