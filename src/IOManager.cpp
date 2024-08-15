#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "IOManager.hpp"

IOManager::IOManager(const char* inputFileName)
{
    _inputFile = fopen(inputFileName, "r");
    if (_inputFile == NULL)
    {
        printf("Erro ao abrir o arquivo de entrada.\n");
        exit(1);
    }

    _outputFile = fopen(OUTPUT_FILE_NAME, "w");
    if (_outputFile == NULL)
    {
        printf("Erro ao abrir o arquivo de saída.\n");
        exit(1);
    }
}

IOManager::~IOManager()
{
    fclose(_inputFile);
    fclose(_outputFile);
}

std::vector<Address> IOManager::ListInputs()
{
    std::vector<Address> inputs;
    Address input;
    
    while (fscanf(_inputFile, "%x", &input) != EOF)
    {
        inputs.push_back(input);
    }
    
    return inputs;
}

std::string AddressToHexadecimal(Address address)
{
    std::stringstream stream;
    stream << "0x" 
           << std::setfill('0') << std::setw(sizeof(Address)*2) 
           << std::hex << std::uppercase << address;
    return stream.str();
}

void IOManager::PrintGroupInOutputFile(Set set, int index)
{
    std::vector<std::pair<bool, Address>> cache = set.GetCache();
    int chacheSize = (int)cache.size();
    for (int i = 0; i < chacheSize; i++)
    {   
        bool valid = cache[i].first;
        Address address = cache[i].second;

        int line = i + index;
        if (valid)
        {
            std::string hexaAddress = AddressToHexadecimal(address);
            fprintf(_outputFile, "%03d 1 %s\n", line, hexaAddress.c_str());
        }
        else
        {
            fprintf(_outputFile, "%03d 0\n", line);
        }
    }
}

void IOManager::WriteOutuputGroups(std::vector<Set> sets)
{
    fprintf(_outputFile, "================\n");
    fprintf(_outputFile, "IDX V ** ADDR **\n");

    for (int i = 0; i < (int)sets.size(); i++)
        PrintGroupInOutputFile(sets[i], i);
}

void IOManager::WriteOutputStatistics(int hitts, int misses)
{
    fprintf(_outputFile, "\n");
    fprintf(_outputFile, "#hits: %d\n", hitts);
    fprintf(_outputFile, "#miss: %d", misses);
}