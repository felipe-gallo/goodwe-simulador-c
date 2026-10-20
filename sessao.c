#include <stdio.h>
#include "sessao.h"

const char *textoStatus(StatusSessao status) {
    return status == ATIVA ? "Ativa" : "Finalizada";
}

float calcularValorTotal(float energiaKwh, float tarifaPorKwh) {
    return energiaKwh * tarifaPorKwh;
}

int buscarSessaoPorId(const SessaoRecarga sessoes[], int quantidade, int id, int *comparacoes) {
    int i;
    *comparacoes = 0;

    for (i = 0; i < quantidade; i++) {
        (*comparacoes)++;
        if (sessoes[i].id == id) {
            return i;
        }
    }
    return -1;
}

void ordenarPorValorDecrescente(SessaoRecarga sessoes[], int quantidade, int *comparacoes, int *trocas) {
    int fim;
    int houveTroca;

    *comparacoes = 0;
    *trocas = 0;

    for (fim = quantidade - 1; fim > 0; fim--) {
        int i;
        houveTroca = 0;
        for (i = 0; i < fim; i++) {
            (*comparacoes)++;
            if (sessoes[i].valorTotal < sessoes[i + 1].valorTotal) {
                SessaoRecarga auxiliar = sessoes[i];
                sessoes[i] = sessoes[i + 1];
                sessoes[i + 1] = auxiliar;
                (*trocas)++;
                houveTroca = 1;
            }
        }
        if (!houveTroca) {
            break;
        }
    }
}

void imprimirCabecalhoTabela(void) {
    printf("\n%-4s %-16s %-10s %-14s %-10s %-10s %-10s %-12s\n",
           "ID", "Cliente", "Placa", "Carregador", "Status", "kWh", "Duracao", "Valor");
    printf("------------------------------------------------------------------------------------------------\n");
}

void imprimirSessao(const SessaoRecarga *sessao) {
    printf("%-4d %-16.16s %-10.10s %-14.14s %-10s %-10.2f %-10d R$ %-9.2f\n",
           sessao->id,
           sessao->cliente,
           sessao->placa,
           sessao->carregador,
           textoStatus(sessao->status),
           sessao->energiaKwh,
           sessao->duracaoMinutos,
           sessao->valorTotal);
}
