#!/bin/bash

# Cores ANSI
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Diretórios e parâmetros
input_dir="Tests/Inputs"
expected_output_dir="Tests/ExpectedOutputs"
output_file="output.txt"
listaDeParametros=(
    "4096 1024 4 1"
    "4096 1024 4 2"
    "4096 1024 4 3"
    "4096 1024 4 4"
    "4096 1024 4 5"
    "4096 1024 2 6"
)

# Compilando o programa
make

# Inicializa contador de testes
total_tests=0
passed_tests=0

# Função para imprimir linha da tabela com cor
print_table_line() {
    local test_number=$1
    local status=$2

    if [[ $status == "PASSED" ]]; then
        printf "| %-2s | ${GREEN}%-6s${NC} |\n" "$test_number" "$status"
    else
        printf "| %-2s | ${RED}%-6s${NC} |\n" "$test_number" "$status"
    fi
}

echo "Iniciando os testes da aplicação:"
echo

# Tabela de resultados
printf "+%s+%s+\n" "----" "--------"
printf "| %-2s | %-6s |\n" "ID" "STATUS"
printf "+%s+%s+\n" "----" "--------"

# Itera sobre a lista de parâmetros
for params in "${listaDeParametros[@]}"; do
    # Extrai os parâmetros e o nome do arquivo de entrada
    set -- $params
    cache_size=$1
    line_size=$2
    associativity=$3
    id=$4

    expected_output_file="$expected_output_dir/test$id.txt"
    
    # Remove caracteres especiais do windows do arquivo esperado
    sed -i 's/\r$//' $expected_output_file

    # Executa o programa simulador com os parâmetros e o arquivo de entrada
    ./simulador $cache_size $line_size $associativity "$input_dir/test$id.txt"

    # Compara a saída gerada com a saída esperada
    if diff -q "$output_file" "$expected_output_file" > /dev/null; then
        status="PASSED"
        passed_tests=$((passed_tests + 1))
        rm $output_file
    else
        status="FAILED"
    fi

    # Imprime o resultado do teste na tabela com cor
    print_table_line "$id" "$status"

    total_tests=$((total_tests + 1))
done

printf "+%s+%s+\n" "----" "--------"

# Resultados finais
echo
if [[ $total_tests -eq $passed_tests ]]; then
    echo -e "Todos os testes ${GREEN}passaram${NC}."
else
    echo -e "Alguns testes ${RED}falharam${NC}."
fi

echo "Testes concluídos."