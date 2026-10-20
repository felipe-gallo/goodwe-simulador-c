# Guia de demonstracao da Sprint 4

## Papel de cada repositorio

- Site EMPS/GoodWe: contexto visual do produto completo e painel de administracao do eletroposto.
- Simulador em C: nucleo academico demonstrado no terminal, com estruturas de dados e algoritmos exigidos pela Sprint.

## Sequencia de demonstracao do terminal

1. Inicie tres sessoes para placas diferentes, tarifas e carregadores diferentes.
2. Finalize as tres sessoes, informando consumo e duracao.
3. Liste as sessoes para comprovar o armazenamento em vetor.
4. Busque uma sessao pelo ID e aponte o contador de comparacoes.
5. Ordene pelo valor total e aponte comparacoes e trocas do Bubble Sort.
6. Exiba estatisticas e gere o relatorio final.

## Defesa tecnica

- `typedef struct SessaoRecarga`: representa todos os dados de uma recarga.
- `sessoes[MAX_SESSOES]`: guarda varias estruturas em memoria.
- `buscarSessaoPorId`: busca linear, O(n), pois pode examinar todas as sessoes.
- `ordenarPorValorDecrescente`: Bubble Sort, O(n^2) no pior caso, pois compara pares em passadas repetidas.
- `estatisticas`: O(n), pois percorre as sessoes uma vez.

## Evolucoes futuras

1. Salvar sessoes em arquivo ou banco de dados.
2. Integrar comunicacao com carregadores reais e tarifas dinamicas.
