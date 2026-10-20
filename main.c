#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sessao.h"

static void limparEntrada(void) {
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }
}

static void lerTexto(const char *mensagem, char destino[], size_t tamanho) {
    size_t fim;
    do {
        printf("%s", mensagem);
        if (fgets(destino, (int)tamanho, stdin) == NULL) {
            printf("\nEntrada encerrada.\n");
            exit(EXIT_SUCCESS);
        }
        fim = strcspn(destino, "\n");
        if (destino[fim] == '\n') {
            destino[fim] = '\0';
        } else {
            limparEntrada();
        }
    } while (destino[0] == '\0' && printf("Campo obrigatorio. Tente novamente.\n"));
}

static int lerInteiro(const char *mensagem, int minimo) {
    char linha[64];
    char *fim;
    long valor;
    do {
        printf("%s", mensagem);
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            exit(EXIT_SUCCESS);
        }
        valor = strtol(linha, &fim, 10);
        if (fim == linha || (*fim != '\n' && *fim != '\0') || valor < minimo || valor > 1000000) {
            printf("Informe um numero inteiro valido (minimo %d).\n", minimo);
            continue;
        }
        return (int)valor;
    } while (1);
}

static float lerFloat(const char *mensagem, float minimo) {
    char linha[64];
    char *fim;
    float valor;
    do {
        printf("%s", mensagem);
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            exit(EXIT_SUCCESS);
        }
        for (char *p = linha; *p != '\0'; p++) {
            if (*p == ',') {
                *p = '.';
            }
        }
        valor = strtof(linha, &fim);
        if (fim == linha || (*fim != '\n' && *fim != '\0') || valor < minimo || valor > 1000000.0f) {
            printf("Informe um numero valido (minimo %.2f).\n", minimo);
            continue;
        }
        return valor;
    } while (1);
}

static void iniciarSessao(SessaoRecarga sessoes[], int *quantidade, int *proximoId) {
    SessaoRecarga *nova;
    if (*quantidade >= MAX_SESSOES) {
        printf("Limite de %d sessoes atingido.\n", MAX_SESSOES);
        return;
    }

    nova = &sessoes[*quantidade];
    nova->id = (*proximoId)++;
    lerTexto("Cliente: ", nova->cliente, sizeof(nova->cliente));
    lerTexto("Placa do veiculo: ", nova->placa, sizeof(nova->placa));
    lerTexto("Nome do eletroposto: ", nova->estacao, sizeof(nova->estacao));
    lerTexto("Identificacao do carregador: ", nova->carregador, sizeof(nova->carregador));
    nova->tarifaPorKwh = lerFloat("Tarifa por kWh (R$): ", 0.01f);
    nova->energiaKwh = 0.0f;
    nova->duracaoMinutos = 0;
    nova->valorTotal = 0.0f;
    nova->status = ATIVA;
    (*quantidade)++;

    printf("\nSessao %d iniciada para %s. Tarifa registrada: R$ %.2f/kWh.\n",
           nova->id, nova->cliente, nova->tarifaPorKwh);
}

static void finalizarSessao(SessaoRecarga sessoes[], int quantidade) {
    int comparacoes;
    int id;
    int indice;
    if (quantidade == 0) {
        printf("Nao ha sessoes cadastradas.\n");
        return;
    }

    id = lerInteiro("ID da sessao para finalizar: ", 1);
    indice = buscarSessaoPorId(sessoes, quantidade, id, &comparacoes);
    if (indice == -1) {
        printf("Sessao nao encontrada apos %d comparacao(oes).\n", comparacoes);
        return;
    }
    if (sessoes[indice].status == FINALIZADA) {
        printf("A sessao %d ja esta finalizada.\n", id);
        return;
    }

    sessoes[indice].energiaKwh = lerFloat("Energia consumida (kWh): ", 0.01f);
    sessoes[indice].duracaoMinutos = lerInteiro("Duracao da recarga (minutos): ", 1);
    sessoes[indice].valorTotal = calcularValorTotal(sessoes[indice].energiaKwh, sessoes[indice].tarifaPorKwh);
    sessoes[indice].status = FINALIZADA;

    printf("\nRecarga finalizada. %.2f kWh x R$ %.2f = R$ %.2f.\n",
           sessoes[indice].energiaKwh,
           sessoes[indice].tarifaPorKwh,
           sessoes[indice].valorTotal);
}

static void listarSessoes(const SessaoRecarga sessoes[], int quantidade) {
    int i;
    if (quantidade == 0) {
        printf("Nenhuma sessao cadastrada.\n");
        return;
    }
    imprimirCabecalhoTabela();
    for (i = 0; i < quantidade; i++) {
        imprimirSessao(&sessoes[i]);
    }
}

static void buscarSessao(const SessaoRecarga sessoes[], int quantidade) {
    int id;
    int indice;
    int comparacoes;
    if (quantidade == 0) {
        printf("Nenhuma sessao cadastrada.\n");
        return;
    }
    id = lerInteiro("ID da sessao procurada: ", 1);
    indice = buscarSessaoPorId(sessoes, quantidade, id, &comparacoes);
    if (indice == -1) {
        printf("Sessao nao encontrada. Busca linear executou %d comparacao(oes).\n", comparacoes);
        return;
    }
    printf("\nSessao encontrada com %d comparacao(oes):\n", comparacoes);
    imprimirCabecalhoTabela();
    imprimirSessao(&sessoes[indice]);
    printf("Eletroposto: %s | Tarifa: R$ %.2f/kWh\n", sessoes[indice].estacao, sessoes[indice].tarifaPorKwh);
}

static void ordenarSessoes(SessaoRecarga sessoes[], int quantidade) {
    int comparacoes;
    int trocas;
    if (quantidade < 2) {
        printf("Cadastre pelo menos duas sessoes para ordenar.\n");
        return;
    }
    ordenarPorValorDecrescente(sessoes, quantidade, &comparacoes, &trocas);
    printf("Sessoes ordenadas por valor total, do maior para o menor.\n");
    printf("Bubble Sort: %d comparacao(oes) e %d troca(s).\n", comparacoes, trocas);
    listarSessoes(sessoes, quantidade);
}

static void estatisticas(const SessaoRecarga sessoes[], int quantidade) {
    int i;
    int finalizadas = 0;
    int ativas = 0;
    int maiorIndice = -1;
    float energiaTotal = 0.0f;
    float faturamento = 0.0f;

    for (i = 0; i < quantidade; i++) {
        if (sessoes[i].status == FINALIZADA) {
            finalizadas++;
            energiaTotal += sessoes[i].energiaKwh;
            faturamento += sessoes[i].valorTotal;
            if (maiorIndice == -1 || sessoes[i].valorTotal > sessoes[maiorIndice].valorTotal) {
                maiorIndice = i;
            }
        } else {
            ativas++;
        }
    }

    printf("\n========== ESTATISTICAS GOODWE ==========\n");
    printf("Sessoes cadastradas: %d\n", quantidade);
    printf("Sessoes ativas: %d\n", ativas);
    printf("Sessoes finalizadas: %d\n", finalizadas);
    printf("Energia total entregue: %.2f kWh\n", energiaTotal);
    printf("Faturamento total: R$ %.2f\n", faturamento);
    if (finalizadas > 0) {
        printf("Consumo medio: %.2f kWh por sessao\n", energiaTotal / finalizadas);
        printf("Ticket medio: R$ %.2f\n", faturamento / finalizadas);
        printf("Maior recarga: sessao %d (%s), R$ %.2f\n",
               sessoes[maiorIndice].id, sessoes[maiorIndice].placa, sessoes[maiorIndice].valorTotal);
    }
    printf("Complexidade do calculo de estatisticas: O(n).\n");
}

static void relatorioFinal(const SessaoRecarga sessoes[], int quantidade) {
    int i;
    printf("\n============= RELATORIO FINAL GOODWE =============\n");
    printf("Eletroposto | Sessao | Veiculo | Energia | Valor | Status\n");
    printf("---------------------------------------------------\n");
    for (i = 0; i < quantidade; i++) {
        printf("%-12.12s | %3d | %-8.8s | %6.2f kWh | R$ %7.2f | %s\n",
               sessoes[i].estacao,
               sessoes[i].id,
               sessoes[i].placa,
               sessoes[i].energiaKwh,
               sessoes[i].valorTotal,
               textoStatus(sessoes[i].status));
    }
    estatisticas(sessoes, quantidade);
}

static void mostrarMenu(void) {
    printf("\n========== GOODWE | SIMULADOR DE ELETROPOSTO ==========\n");
    printf("1 - Iniciar sessao de recarga\n");
    printf("2 - Finalizar sessao e calcular tarifacao\n");
    printf("3 - Listar sessoes\n");
    printf("4 - Buscar sessao por ID\n");
    printf("5 - Ordenar sessoes por valor total\n");
    printf("6 - Exibir estatisticas\n");
    printf("7 - Gerar relatorio final\n");
    printf("0 - Encerrar programa\n");
}

int main(void) {
    SessaoRecarga sessoes[MAX_SESSOES];
    int quantidade = 0;
    int proximoId = 1;
    int opcao;

    printf("Bem-vindo ao GoodWe, simulador de gerenciamento de recargas.\n");
    do {
        mostrarMenu();
        opcao = lerInteiro("Escolha uma opcao: ", 0);
        switch (opcao) {
            case 1: iniciarSessao(sessoes, &quantidade, &proximoId); break;
            case 2: finalizarSessao(sessoes, quantidade); break;
            case 3: listarSessoes(sessoes, quantidade); break;
            case 4: buscarSessao(sessoes, quantidade); break;
            case 5: ordenarSessoes(sessoes, quantidade); break;
            case 6: estatisticas(sessoes, quantidade); break;
            case 7: relatorioFinal(sessoes, quantidade); break;
            case 0: printf("Programa encerrado. Obrigado por usar o GoodWe.\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);

    return EXIT_SUCCESS;
}
