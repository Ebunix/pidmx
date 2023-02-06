#pragma once

#include <ankerl/unordered_dense.h>
#include "pidmx_utils.h"

/*
template<typename T>
class Set : public std::unordered_set<T> {
public:
    Set() = default;
    inline bool includes(const T &value) {
        return this->find(value) != this->end();
    }


};

template<typename T, typename U>
class Map : public std::unordered_map<T, U> {
public:
    inline bool includes(const T &value) {
        return this->find(value) != this->end();
    }
};

typedef Set<Hash> IDSet;
 */



template <typename T>
using Set = ankerl::unordered_dense::set<T, ankerl::unordered_dense::hash<T>, std::equal_to<T>>;

template <typename T, typename U>
using Map = ankerl::unordered_dense::map<T, U, ankerl::unordered_dense::hash<T>, std::equal_to<T>>;

typedef Set<Hash> IDSet;

#define ANKERL_HASH(T) template <> \
struct ankerl::unordered_dense::hash<T> { \
    using is_avalanching = void; \
    [[nodiscard]] auto operator()(T const& x) const noexcept -> uint64_t { \
        return detail::wyhash::hash(&x, sizeof(x)); \
    } \
};
