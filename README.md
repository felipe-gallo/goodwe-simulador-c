# GoodWe Simulador de Recargas em C

Simulador de terminal para a Sprint 4 do projeto GoodWe. O programa representa o nucleo de um eletroposto: registra sessoes de recarga, calcula a tarifacao, consulta dados e gera indicadores operacionais.

## Funcionalidades

- Iniciar sessoes para diferentes veiculos e carregadores.
- Finalizar uma sessao, informando energia consumida e duracao.
- Calcular automaticamente o valor por `kWh x tarifa`.
- Armazenar ate 100 sessoes em memoria.
- Listar as sessoes registradas.
- Buscar uma sessao por ID com busca linear.
- Ordenar sessoes finalizadas por valor total com Bubble Sort.
- Exibir estatisticas e relatorio final.

## Compilacao

No Windows, usando GCC ou Clang:

```powershell
gcc -Wall -Wextra -Wpedantic -std=c11 main.c sessao.c -o goodwe_simulador.exe
.\goodwe_simulador.exe
```

No Linux ou macOS:

```bash
gcc -Wall -Wextra -Wpedantic -std=c11 main.c sessao.c -o goodwe_simulador
./goodwe_simulador
```

No PowerShell, tambem e possivel executar `./build.ps1`. O script encontra `clang` ou `gcc` e gera o executavel.

## Roteiro de dados para teste

O arquivo `demo_input.txt` contem uma sequencia pronta para cadastrar tres sessoes, finaliza-las, listar, buscar, ordenar, calcular estatisticas e gerar o relatorio. Depois de compilar, execute:

```powershell
Get-Content .\demo_input.txt | .\goodwe_simulador.exe
```

## Estrutura usada

`typedef struct SessaoRecarga` agrupa as informacoes de uma recarga. O vetor `sessoes` armazena varias estruturas em memoria durante a execucao. A busca por ID percorre o vetor, portanto e `O(n)`. A ordenacao usa Bubble Sort por valor total, portanto e `O(n^2)` no pior caso.

## Observacao

Os dados sao simulados e armazenados apenas enquanto o programa esta aberto. Persistencia em arquivo ou banco de dados e uma evolucao futura proposta para o projeto.
