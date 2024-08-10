#include <stdio.h>
#include <sys/types.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <queue>

// DEFINIÇÃO DE CONSTANTES E TIPOS
#define SUCESSO (00)
#define UNDEFINED_CACHE_VALUE (-1)
typedef uint32_t Address;
const char* OUTPUT_FILE_NAME = "output.txt";

// VARIÁVEIS GLOBAIS
unsigned int _hitts = 0;
unsigned int _misses = 0;

int _inputCacheSize, _inputLineSize, _inputGroupSize;
const char* _inputFileName;

Address* _cache;
std::queue<Address>* _fifo;

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

void Setup()
{
    _cache = new Address[_inputCacheSize];
    for (int i = 0; i < _inputCacheSize; i++)
    {
        _cache[i] = UNDEFINED_CACHE_VALUE;
    }

    _fifo = new std::queue<Address>[_inputCacheSize / _inputGroupSize];

    OpenFiles();
}

Address GroupIndex(Address address)
{
    return (address / _inputLineSize) % _inputGroupSize;
}

Address DefinePosition(Address address)
{
    int groupIndex = GroupIndex(address);
    int groupStart = groupIndex * _inputGroupSize;
    int groupEnd = groupStart + _inputGroupSize;

    for (int i = groupStart; i < groupEnd; i++)
    {
        if (_cache[i] == UNDEFINED_CACHE_VALUE)
            return i;
    }

    Address addressToReplace = _fifo[groupIndex].front();
    _fifo[groupIndex].pop();

    return addressToReplace;
}

void PrintGroupInOutputFile(int groupIndex)
{
    int groupStart = groupIndex * _inputGroupSize;
    int groupEnd = groupStart + _inputGroupSize;

    fprintf(_outputFile, "================\n");
    fprintf(_outputFile, "IDX V ** ADDR **\n");
    for (int i = groupStart; i < groupEnd; i++)
    {
        // ID DEFINIDO DE FORMA ESTÁTICA, ALTERAR DEPOIS
        int lineIndex = i % _inputGroupSize;
        if (_cache[i] != UNDEFINED_CACHE_VALUE)
        {
            fprintf(_outputFile, "%.3d %d %08x\n", lineIndex, 1, _cache[i]);
        }
        else
        {
            fprintf(_outputFile, "%.3d %d\n", lineIndex, 0);
        }
    }
}

// PROGRAMA PRINCIPAL
int main(int argc, char const *argv[])
{
    // Inicialização dos dados
    ParseArguments(argc, argv);
    Setup();

    Address address;
    while (fscanf(_inputFile, "%x", &address) != EOF)
    {
        int groupIndex = GroupIndex(address);
        int groupStart = groupIndex * _inputGroupSize;
        int groupEnd = groupStart + _inputGroupSize;

        bool hit = false;
        for (int i = groupStart; i < groupEnd; i++)
        {
            if (_cache[i] == address)
            {
                hit = true;
                break;
            }
        }

        if (hit)
        {
            _hitts++;
        }
        else
        {
            _misses++;
            Address position = DefinePosition(address);
            _cache[position] = address;
            _fifo[groupIndex].push(address);
        }
    }

    for (int i = 0; i < 3; i++)
    {
        PrintGroupInOutputFile(i);
    }
    
    fprintf(_outputFile, "\n");
    fprintf(_outputFile, "#hits: %d\n", _hitts);
    fprintf(_outputFile, "#miss: %d\n", _misses);

    // Limpeza da memória
    delete[] _cache;
    delete[] _fifo;

    return SUCESSO;
}