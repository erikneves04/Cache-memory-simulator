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
        bool _isFull;
        int _cacheSize;

    public:
        Set(int setSize);
        ~Set();

        Result Insert(Address address);
        std::vector<std::pair<bool, Address>> GetCache();
};

#endif