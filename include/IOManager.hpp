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

    public:
        IOManager(const char* inputFileName);
        ~IOManager();

        std::vector<Address> ListInputs();
        void WriteOutuputGroups(std::vector<Set> sets);
        void WriteOutputStatistics(int hitts, int misses);
};

#endif