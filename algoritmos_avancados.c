#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOME 64
#define MAX_PISTA 64

/* Estrutura que representa um cômodo da mansão (nó da árvore binária) */
typedef struct Sala {
    char nome[MAX_NOME];
    char pista[MAX_PISTA];    // string vazia se não houver pista
    struct Sala *esq;
    struct Sala *dir;
} Sala;

/* Nó da árvore binária de busca que armazena pistas encontradas */
typedef struct PistaNode {
    char pista[MAX_PISTA];
    struct PistaNode *esq;
    struct PistaNode *dir;
} PistaNode;

/* criarSala: cria dinamicamente uma sala com nome, pista (pode ser "") e filhos */
Sala* criarSala(const char *nome, const char *pista, Sala *esq, Sala *dir) {
    Sala *s = (Sala*) malloc(sizeof(Sala));
    if (!s) {
        fprintf(stderr, "Erro: falha na alocação de memória para sala.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(s->nome, nome, MAX_NOME-1);
    s->nome[MAX_NOME-1] = '\0';
    if (pista) {
        strncpy(s->pista, pista, MAX_PISTA-1);
        s->pista[MAX_PISTA-1] = '\0';
    } else {
        s->pista[0] = '\0';
    }
    s->esq = esq;
    s->dir = dir;
    return s;
}

/* novoPistaNode: cria um nó para a BST de pistas */
PistaNode* novoPistaNode(const char *pista) {
    PistaNode *n = (PistaNode*) malloc(sizeof(PistaNode));
    if (!n) {
        fprintf(stderr, "Erro: falha na alocação de memória para pista.\n");
        exit(EXIT_FAILURE);
    }
    strncpy(n->pista, pista, MAX_PISTA-1);
    n->pista[MAX_PISTA-1] = '\0';
    n->esq = n->dir = NULL;
    return n;
}

/* inserirPista: insere uma pista na BST; ignora duplicatas */
PistaNode* inserirPista(PistaNode *raiz, const char *pista) {
    if (raiz == NULL) {
        return novoPistaNode(pista);
    }
    int cmp = strcmp(pista, raiz->pista);
    if (cmp < 0) {
        raiz->esq = inserirPista(raiz->esq, pista);
    } else if (cmp > 0) {
        raiz->dir = inserirPista(raiz->dir, pista);
    } /* se igual, não insere duplicata */
    return raiz;
}

/* exibirPistas: imprime as pistas em ordem alfabética (em-ordem) */
void exibirPistas(PistaNode *raiz) {
    if (raiz == NULL) return;
    exibirPistas(raiz->esq);
    printf(" - %s\n", raiz->pista);
    exibirPistas(raiz->dir);
}

/* liberarPistas: libera memória da BST de pistas */
void liberarPistas(PistaNode *raiz) {
    if (!raiz) return;
    liberarPistas(raiz->esq);
    liberarPistas(raiz->dir);
    free(raiz);
}

/* liberarMansao: libera memória da árvore de salas (pos-ordem) */
void liberarMansao(Sala *raiz) {
    if (!raiz) return;
    liberarMansao(raiz->esq);
    liberarMansao(raiz->dir);
    free(raiz);
}

/* explorarSalasComPistas: navegação interativa e coleta automática de pistas
   - opcoes: e (esquerda), d (direita), p (ver pistas coletadas até agora), s (sair) */
void explorarSalasComPistas(Sala *inicio, PistaNode **colecao) {
    Sala *atual = inicio;
    char opcao;

    printf("\n=== EXPLORAÇÃO DA MANSÃO (NÍVEL AVENTUREIRO) ===\n");

    while (1) {
        if (atual == NULL) {
            printf("Você está em uma área inexistente. Voltando ao Hall de Entrada.\n");
            atual = inicio;
        }

        printf("\nVocê está em: %s\n", atual->nome);

        /* se a sala tiver pista não vazia, adiciona automaticamente (ignora se já coletada) */
        if (atual->pista[0] != '\0') {
            printf("Você encontrou uma pista: \"%s\"\n", atual->pista);
            *colecao = inserirPista(*colecao, atual->pista);
        }

        printf("Opções:\n");
        if (atual->esq) printf("  (e) Ir para a esquerda → %s\n", atual->esq->nome);
        if (atual->dir) printf("  (d) Ir para a direita  → %s\n", atual->dir->nome);
        printf("  (p) Ver pistas coletadas até agora\n");
        printf("  (s) Sair da exploração\n");
        printf("Escolha: ");

        /* lê um caractere não branco */
        if (scanf(" %c", &opcao) != 1) {
            /* caso de leitura inválida, limpar e continuar */
            int c;
            while ((c = getchar()) != '\n' && c != EOF) ;
            printf("Entrada inválida. Tente novamente.\n");
            continue;
        }

        opcao = tolower(opcao);

        if (opcao == 'e') {
            if (atual->esq) atual = atual->esq;
            else printf("Não há caminho à esquerda dessa sala.\n");
        } else if (opcao == 'd') {
            if (atual->dir) atual = atual->dir;
            else printf("Não há caminho à direita dessa sala.\n");
        } else if (opcao == 'p') {
            printf("\n=== PISTAS COLETADAS (ATÉ O MOMENTO) ===\n");
            if (*colecao == NULL) {
                printf("(Nenhuma pista coletada)\n");
            } else {
                exibirPistas(*colecao);
            }
        } else if (opcao == 's') {
            printf("Exploração encerrada pelo usuário.\n");
            return;
        } else {
            printf("Opção inválida. Use e, d, p ou s.\n");
        }
    }
}

/* construirMansao: monta o mapa fixo da mansão com pistas associadas */
Sala* construirMansao() {
    /* Exemplo de mapa:
                 Hall de Entrada
                 /            \
            Biblioteca      Sala de Jantar
             /     \          /      \
       Escritório Adega   Cozinha   Jardim

       Algumas salas têm pistas associadas (strings).
    */

    Sala *escritorio = criarSala("Escritório", "Luvas ensanguentadas", NULL, NULL);
    Sala *adega      = criarSala("Adega", "", NULL, NULL);
    Sala *cozinha    = criarSala("Cozinha", "Pegadas com farinha", NULL, NULL);
    Sala *jardim     = criarSala("Jardim", "Chave enferrujada", NULL, NULL);

    Sala *biblioteca = criarSala("Biblioteca", "Página arrancada", escritorio, adega);
    Sala *jantar     = criarSala("Sala de Jantar", "", cozinha, jardim);

    Sala *hall       = criarSala("Hall de Entrada", "", biblioteca, jantar);

    return hall;
}

int main(void) {
    Sala *raiz = construirMansao();
    PistaNode *colecao = NULL;

    printf("========================================\n");
    printf("DETECTIVE QUEST – NÍVEL AVENTUREIRO\n");
    printf("========================================\n");
    printf("Você começa no Hall de Entrada. Explore a mansão e colete pistas.\n");

    explorarSalasComPistas(raiz, &colecao);

    printf("\n=== PISTAS COLETADAS (RESUMO FINAL) ===\n");
    if (colecao == NULL) {
        printf("(Nenhuma pista coletada)\n");
    } else {
        exibirPistas(colecao);
    }

    /* libera memória antes de terminar */
    liberarPistas(colecao);
    liberarMansao(raiz);

    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}

