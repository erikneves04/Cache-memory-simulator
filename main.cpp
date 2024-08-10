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

void Setup()
{
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
            fprintf(_outputFile, "%03d 1 %08x\n", line, group.cache[i]);
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
        Group group = DefineGroup(address);
        Address position = DefinePosition(group);

        if (group.cache[position] == address)
        {
            _hitts++;
        }
        else
        {
            _misses++;
            group.cache[position] = address;
            group.fifo.push(position);
        }
    }

    for (int i = 0; i < _inputGroupSize; i++)
        PrintGroupInOutputFile(i);
    
    fprintf(_outputFile, "\n");
    fprintf(_outputFile, "#hits: %d\n", _hitts);
    fprintf(_outputFile, "#miss: %d\n", _misses);

    // Limpeza da memória e fechamento dos arquivos
    delete[] _groups;
    fclose(_inputFile);
    fclose(_outputFile);

    return SUCESSO;
}