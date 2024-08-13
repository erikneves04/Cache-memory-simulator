#include "Set.hpp"

Set::Set(int setSize)
{
    _cache = std::vector<std::pair<bool, Address>>(setSize, std::make_pair(false, UNDEFINED_CACHE_VALUE));
    _isFull = false;
    _cacheSize = setSize;
}

Set::~Set()
{ }

Result Set::Insert(Address address)
{
    for (int i = 0; i < _cacheSize; i++)
    {
        if (_cache[i].second == address)
            return HIT;
    }

    if (_isFull)
    {
        int topPosition = _fifo.front();
        _fifo.pop();
        _cache[topPosition] = std::make_pair(true, address);
    }
    else
    {
        _isFull = (int)_fifo.size() == _cacheSize;

        for (int i = 0; i < _cacheSize; i++)
        {
            if (_cache[i].first == false)
            {
                _cache[i] = std::make_pair(true, address);
                _fifo.push(i);
                return MISS;
            }
        }
    }

    return MISS;
}

std::vector<std::pair<bool, Address>> Set::GetCache()
{
    return _cache;
}