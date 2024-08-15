#include <stdio.h>
#include <iostream>
#include <cmath>

#include "Constantes.hpp"
#include "Set.hpp"
#include "IOManager.hpp"

// DECLARAÇÃO DE VARIÁVEIS GLOBAIS
unsigned int _hitts = 0;
unsigned int _misses = 0;

int _inputCacheSize, _inputLineSize, _inputGroupSize;
const char* _inputFileName;

std::vector<Set> _sets;

// DEFINIÇÃO DOS SERVIÇOS
IOManager* _ioManager;

// FUNÇÕES AUXILIARES
void ParseArguments(int argc, char const *argv[])
{
    if (argc != 5)
    {
        printf("Número de argumentos inválido. Uso: <cache-size> <line-size> <set-size> <input-file-name>");
        exit(1);
    }

    _inputCacheSize = atoi(argv[1]);
    _inputLineSize = atoi(argv[2]);
    _inputGroupSize = atoi(argv[3]);
    _inputFileName = argv[4];
}

void Setup(int argc, char const *argv[])
{
    ParseArguments(argc, argv);

    _ioManager = new IOManager(_inputFileName);

    int setsSize = _inputCacheSize / (_inputGroupSize * _inputLineSize);
    _sets = std::vector<Set>(setsSize, Set(_inputGroupSize));
    int offsetBits = std::log2(_inputLineSize);

     _ioManager->SetOffsetBits(offsetBits);
}

void PerformCacheSimulation()
{
    std::vector<Address> inputs = _ioManager->ListInputs();
    int inputsSize = (int)inputs.size();

    for (int i = 0; i < inputsSize; i++)
    {
        Address input = inputs[i];
        int setIndex = (input / (_inputLineSize * _sets.size())) % _sets.size();

        Result result = _sets[setIndex].Insert(input);

        if (result == HIT)
            _hitts++;
        else
        {
            _misses++;
            _ioManager->WriteOutputGroups(_sets);
        }
    }
}

int main(int argc, char const *argv[])
{
    // Inicialização dos dados
    Setup(argc, argv);

    // Simulação da cache
    PerformCacheSimulation();

    // Salva as estatísticas finais
    _ioManager->WriteOutputStatistics(_hitts, _misses);

    // Limpeza da memória
    delete _ioManager;

    return SUCESSO;
}