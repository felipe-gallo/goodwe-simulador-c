#ifndef SESSAO_H
#define SESSAO_H

#define MAX_SESSOES 100
#define TAM_NOME 60
#define TAM_PLACA 12
#define TAM_CARREGADOR 30
#define TAM_ESTACAO 50

typedef enum {
    ATIVA = 1,
    FINALIZADA = 2
} StatusSessao;

typedef struct {
    int id;
    char cliente[TAM_NOME];
    char placa[TAM_PLACA];
    char estacao[TAM_ESTACAO];
    char carregador[TAM_CARREGADOR];
    float tarifaPorKwh;
    float energiaKwh;
    int duracaoMinutos;
    float valorTotal;
    StatusSessao status;
} SessaoRecarga;

const char *textoStatus(StatusSessao status);
float calcularValorTotal(float energiaKwh, float tarifaPorKwh);
int buscarSessaoPorId(const SessaoRecarga sessoes[], int quantidade, int id, int *comparacoes);
void ordenarPorValorDecrescente(SessaoRecarga sessoes[], int quantidade, int *comparacoes, int *trocas);
void imprimirCabecalhoTabela(void);
void imprimirSessao(const SessaoRecarga *sessao);

#endif
