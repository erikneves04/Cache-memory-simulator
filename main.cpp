#include <stdio.h>
#include <sys/types.h>
#include <cstdint>
#include <iostream>
#include <vector>

// DEFINIÇÃO DE CONSTANTES E TIPOS
#define SUCESSO (00)
typedef uint32_t Address;

// VARIÁVEIS GLOBAIS
unsigned int _hitts = 0;
unsigned int _misses = 0;

int _inputCacheSize, _inputLineSize, _inputGroupSize;
const char* _inputFileName;

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

// PROGRAMA PRINCIPAL
int main(int argc, char const *argv[])
{
    ParseArguments(argc, argv);

    return SUCESSO;
}