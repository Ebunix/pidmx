#pragma once
#include <string>
#include <vector>

template<typename T>
struct Collection {
	std::string name;
	std::vector<T> data;
};
