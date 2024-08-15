#pragma once

#ifndef IO_MANAGER_HPP
#define IO_MANAGER_HPP

#include <Set.hpp>
#include <vector>
#include "Constantes.hpp"

class IOManager
{
    private:
        FILE* _inputFile;
        FILE* _outputFile;
        const char* OUTPUT_FILE_NAME = "output.txt";

        void PrintGroupInOutputFile(Set set, int index);

         int offsetBits;

    public:
        IOManager(const char* inputFileName);
        ~IOManager();

        void SetOffsetBits(int offset);
        Address GetBlockIdentifier(Address address);

        std::vector<Address> ListInputs();
        void WriteOutputGroups(std::vector<Set> sets);
        void WriteOutputStatistics(int hitts, int misses);
};

#endif