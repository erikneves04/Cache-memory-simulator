# Cache-memory-simulator
 
Instruções para Compilação e Execução do Simulador de Cache

Requisitos:

- Sistema operacional Linux
- Compilador GCC
- Utilitário `make`

Passos para Compilação:

1. Acesse o diretório principal do projeto.

2. Compile o programa:
   Execute o comando abaixo para compilar o programa:
   make
   Isso gerará o executável `simulador` no diretório atual.

Passos para Execução:

1. Execute o simulador:
   Para rodar o simulador de cache, utilize o seguinte comando:
   `./simulador <tamanho da cache> <tamanho da linha> <tamanho do conjunto> <nome do arquivo de entrada>`
   Substitua os parâmetros conforme necessário:
   - `<tamanho da cache>`: O tamanho total da cache em bytes.
   - `<tamanho da linha>`: O tamanho de cada linha da cache em bytes.
   - `<tamanho do conjunto>`: O número de linhas em cada conjunto (associatividade).
   - `<nome do arquivo de entrada>`: O nome do arquivo de texto contendo os endereços de memória a serem simulados.

   Exemplo de uso:
   ./simulador 1024 64 4 enderecos.txt

2. Saída do programa:
   A saída da simulação será gravada em um arquivo chamado `output.txt` no mesmo diretório em que o simulador foi executado.

Integrantes do grupo:

Erik Roberto Reis Neves - 2022093040 </br>
Gabriel </br>
Gabriella de Lima Araujo - 2022104786
