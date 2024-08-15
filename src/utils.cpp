#include "utils.hpp"

Address GetBlockIdentifier(Address address, int offsetBits) {
    Address blockAddress = (address >> offsetBits) & 0xFFFFFFFF;
    return blockAddress;
}