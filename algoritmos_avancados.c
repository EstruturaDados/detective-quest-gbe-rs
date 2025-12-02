#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 10

typedef struct Sala {
    char nome[40];
    char pista[80];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

typedef struct PistaNode {
    char pista[80];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

typedef struct HashNode {
    char pista[80];
    char suspeito[40];
    struct HashNode *prox;
} HashNode;

HashNode *tabela[TAM_HASH];

Sala *criarSala(char nome[], char pista[]) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

PistaNode *inserirPista(PistaNode *raiz, char pista[]) {
    if (raiz == NULL) {
        PistaNode *novo = (PistaNode*) malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirPista(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirPista(raiz->dir, pista);
    return raiz;
}

int hash(char *str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++)
        soma += str[i];
    return soma % TAM_HASH;
}

void inserirNaHash(char pista[], char suspeito[]) {
    int pos = hash(pista);
    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[pos];
    tabela[pos] = novo;
}

char *encontrarSuspeito(char pista[]) {
    int pos = hash(pista);
    HashNode *aux = tabela[pos];
    while (aux != NULL) {
        if (strcmp(aux->pista, pista) == 0)
            return aux->suspeito;
        aux = aux->prox;
    }
    return NULL;
}

void emOrdem(PistaNode *raiz) {
    if (raiz == NULL) return;
    emOrdem(raiz->esq);
    printf(" - %s\n", raiz->pista);
    emOrdem(raiz->dir);
}

void contarSuspeitos(PistaNode *raiz, char *alvo, int *contador) {
    if (raiz == NULL) return;

    contarSuspeitos(raiz->esq, alvo, contador);
    char *sus = encontrarSuspeito(raiz->pista);
    if (sus != NULL && strcmp(sus, alvo) == 0)
        (*contador)++;
    contarSuspeitos(raiz->dir, alvo, contador);
}

void explorarSalas(Sala *atual, PistaNode **arvorePistas) {
    char opcao;

    while (1) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        printf("Escolha o caminho: (e) esquerda, (d) direita, (s) sair\n> ");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esq != NULL)
            atual = atual->esq;
        else if (opcao == 'd' && atual->dir != NULL)
            atual = atual->dir;
        else if (opcao == 's')
            break;
        else
            printf("Caminho inválido!\n");
    }
}

int main() {
    for (int i = 0; i < TAM_HASH; i++)
        tabela[i] = NULL;

    Sala *hall = criarSala("Hall de Entrada", "Pegada suspeita");
    Sala *salaEstar = criarSala("Sala de Estar", "Perfume raro");
    Sala *cozinha = criarSala("Cozinha", "");
    Sala *quarto = criarSala("Quarto", "Relógio quebrado");
    Sala *jardim = criarSala("Jardim", "Luvas rasgadas");

    hall->esq = salaEstar;
    hall->dir = cozinha;
    salaEstar->esq = quarto;
    salaEstar->dir = jardim;

    inserirNaHash("Pegada suspeita", "Sr. Dalton");
    inserirNaHash("Perfume raro", "Sra. Violet");
    inserirNaHash("Relógio quebrado", "Sr. Dalton");
    inserirNaHash("Luvas rasgadas", "Jardineiro Carlos");

    PistaNode *arvorePistas = NULL;

    printf("=== DETECTIVE QUEST — NIVEL MESTRE ===\n");
    printf("Explore os cômodos e colete pistas!\n");

    explorarSalas(hall, &arvorePistas);

    printf("\nPistas coletadas:\n");
    emOrdem(arvorePistas);

    char acusado[40];
    printf("\nQuem você acusa? ");
    scanf(" %[^\n]s", acusado);

    int contador = 0;
    contarSuspeitos(arvorePistas, acusado, &contador);

    printf("\nResultado da investigação:\n");
    if (contador >= 2)
        printf("O suspeito %s foi acusado com %d pistas. CULPADO!\n", acusado, contador);
    else
        printf("O suspeito %s possui apenas %d pista(s). Insuficiente! Caso reaberto.\n", acusado, contador);

    return 0;
}