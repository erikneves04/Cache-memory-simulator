#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "Set.hpp"
#include "utils.hpp"

Set::Set(int setSize, int offsetBits)
{
    _cache = std::vector<std::pair<bool, Address>>(setSize, std::make_pair(false, UNDEFINED_CACHE_VALUE));
    _cacheSize = setSize;
    _offsetBits = offsetBits;
}

Set::~Set()
{ }

Result Set::Insert(Address address)
{
    Address blockIdentifier = GetBlockIdentifier(address, _offsetBits);
    for (int i = 0; i < _cacheSize; i++)
    {
        if (_cache[i].first && _cache[i].second == blockIdentifier) {
            // printf("Entra\n");
            return HIT;
        }
    }
    
    for (int i = 0; i < _cacheSize; i++)
    {
        if (_cache[i].first == false)
        {
            _cache[i] = std::make_pair(true, blockIdentifier);
            _fifo.push(i);
            return MISS;
        }
    }

    int topPosition = _fifo.front();
    _fifo.pop();
    _cache[topPosition] = std::make_pair(true, blockIdentifier);
    _fifo.push(topPosition);

    return MISS;
}

std::vector<std::pair<bool, Address>> Set::GetCache()
{
    return _cache;
}