#pragma once

#ifndef SET_HPP
#define SET_HPP

#include <queue>
#include <vector>
#include "Constantes.hpp"

class Set
{
    private:
        std::vector<std::pair<bool, Address>> _cache;
        std::queue<int> _fifo;
        int _cacheSize;
        int _offsetBits;

    public:
        Set(int setSize, int offsetBits);
        ~Set();

        Result Insert(Address address);
        std::vector<std::pair<bool, Address>> GetCache();
};

#endif