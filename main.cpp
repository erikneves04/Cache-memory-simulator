#include <stdio.h>
#include <sys/types.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <sstream>
#include <string>

// DEFINIÇÃO DE CONSTANTES E TIPOS
#define SUCESSO (00)
#define UNDEFINED_CACHE_VALUE (-1)

typedef uint32_t Address;
typedef struct Group
{
    Address* cache;
    std::queue<Address> fifo;
} Group;

const char* OUTPUT_FILE_NAME = "output.txt";

// VARIÁVEIS GLOBAIS
unsigned int _hitts = 0;
unsigned int _misses = 0;

int _inputCacheSize, _inputLineSize, _inputGroupSize;
const char* _inputFileName;

Group* _groups;

FILE* _inputFile = NULL;
FILE* _outputFile = NULL;

// FUNÇÕES AUXILIARES
void ParseArguments(int argc, char const *argv[])
{
    if (argc != 5)
    {
        std::cout << "Número de argumentos inválido. Uso: <cache-size> <line-size> <group-size> <input-file-name>" << std::endl;
        exit(1);
    }

    _inputCacheSize = atoi(argv[1]);
    _inputLineSize = atoi(argv[2]);
    _inputGroupSize = atoi(argv[3]);
    _inputFileName = argv[4];
}

void OpenFiles()
{
    _inputFile = fopen(_inputFileName, "r");
    if (_inputFile == NULL)
    {
        std::cout << "Erro ao abrir o arquivo de entrada." << std::endl;
        exit(1);
    }

    _outputFile = fopen(OUTPUT_FILE_NAME, "w");
    if (_outputFile == NULL)
    {
        std::cout << "Erro ao abrir o arquivo de saída." << std::endl;
        exit(1);
    }
}

void Setup(int argc, char const *argv[])
{
    ParseArguments(argc, argv);

    _groups = new Group[_inputGroupSize];
    for (int i = 0; i < _inputGroupSize; i++)
    {
        _groups[i].cache = new Address[_inputLineSize * _inputGroupSize];
        for (int j = 0; j < _inputLineSize * _inputGroupSize; j++)
        {
            _groups[i].cache[j] = UNDEFINED_CACHE_VALUE;
        }
    }

    OpenFiles();
}

Group DefineGroup(Address address)
{
    int index = (address / _inputLineSize) % _inputGroupSize;
    return _groups[index];
}

Address DefinePosition(Group group)
{
    if (group.fifo.size() < _inputGroupSize)
    {
        return group.fifo.size();
    }
    else
    {
        Address position = group.fifo.front();
        group.fifo.pop();
        return position;
    }
}

std::string AddressToHexadecimal(Address address)
{
  std::stringstream stream;
  stream << "0x" 
         << std::setfill ('0') << std::setw(sizeof(Address)*2) 
         << std::hex << address;
  return stream.str();
}

void PrintGroupInOutputFile(int groupIndex)
{
    Group group = _groups[groupIndex];

    fprintf(_outputFile, "================\n");
    fprintf(_outputFile, "IDX V ** ADDR **\n");
    
    for (int i = 0; i < _inputGroupSize; i++)
    {   
        int line = i + (groupIndex * _inputGroupSize);
        if (group.cache[i] == UNDEFINED_CACHE_VALUE)
            fprintf(_outputFile, "%03d 0\n", line);
        else
        {
            std::string address = AddressToHexadecimal(group.cache[i]);
            fprintf(_outputFile, "%03d 1 %s\n", line, address.c_str());
        }
    }
}

void SaveStateToOutputFile()
{
    for (int i = 0; i < _inputGroupSize; i++)
        PrintGroupInOutputFile(i);
    
    fprintf(_outputFile, "\n");
    fprintf(_outputFile, "#hits: %d\n", _hitts);
    fprintf(_outputFile, "#miss: %d", _misses);
}

void PerformCacheSimulation()
{
    Address address;
    while (fscanf(_inputFile, "%x", &address) != EOF)
    {
        Group group = DefineGroup(address);

        for (int i = 0; i < _inputGroupSize; i++)
        {
            if (group.cache[i] == address)
            {
                _hitts++;
                break;
            }
        }

        Address position = DefinePosition(group);

        _misses++;
        group.cache[position] = address;
        group.fifo.push(position);
    }
}

void MemoryClean()
{
    for (int i = 0; i < _inputGroupSize; i++)
        delete[] _groups[i].cache;

    delete[] _groups;

    fclose(_inputFile);
    fclose(_outputFile);
}

int main(int argc, char const *argv[])
{
    // Inicialização dos dados
    Setup(argc, argv);

    // Simulação da cache
    PerformCacheSimulation();

    // Salva o estado final no arquivo de saída
    SaveStateToOutputFile();

    // Limpeza da memória
    MemoryClean();

    return SUCESSO;
}