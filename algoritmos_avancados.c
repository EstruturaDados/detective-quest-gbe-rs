#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Representa uma sala da mansão (nó da árvore binária)
typedef struct Sala {
    char nome[40];
    struct Sala *esq;
    struct Sala *dir;
} Sala;

// Cria dinamicamente uma sala com nome e filhos
Sala* criarSala(const char *nome, Sala *esq, Sala *dir) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));

    if (!nova) {
        printf("Erro ao alocar memória!\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    nova->esq = esq;
    nova->dir = dir;

    return nova;
}

// Permite que o jogador explore a mansão tomando decisões
void explorarSalas(Sala *atual) {
    char opcao;

    printf("\n=== EXPLORAÇÃO DA MANSÃO ===\n");

    while (atual != NULL) {

        printf("\nVocê está em: %s\n", atual->nome);

        // Se chegou a um cômodo sem caminhos, a exploração termina
        if (atual->esq == NULL && atual->dir == NULL) {
            printf("Este é um beco sem saída. Não há portas aqui.\n");
            return;
        }

        printf("Para onde deseja ir?\n");

        if (atual->esq != NULL)
            printf("  (e) Esquerda → %s\n", atual->esq->nome);

        if (atual->dir != NULL)
            printf("  (d) Direita  → %s\n", atual->dir->nome);

        printf("  (s) Sair da exploração\n");
        printf("Opção: ");

        scanf(" %c", &opcao);
        opcao = tolower(opcao);

        if (opcao == 'e' && atual->esq != NULL) {
            atual = atual->esq;
        }
        else if (opcao == 'd' && atual->dir != NULL) {
            atual = atual->dir;
        }
        else if (opcao == 's') {
            printf("Você encerrou a exploração.\n");
            return;
        }
        else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// Monta a árvore que representa o mapa da mansão
Sala* construirMansao() {

    Sala *escritorio = criarSala("Escritório", NULL, NULL);
    Sala *adega      = criarSala("Adega", NULL, NULL);
    Sala *cozinha    = criarSala("Cozinha", NULL, NULL);
    Sala *jardim     = criarSala("Jardim", NULL, NULL);

    Sala *biblioteca = criarSala("Biblioteca", escritorio, adega);
    Sala *jantar     = criarSala("Sala de Jantar", cozinha, jardim);

    Sala *hall       = criarSala("Hall de Entrada", biblioteca, jantar);

    return hall;
}

int main() {

        Sala *raiz = construirMansao();

    printf("========================================\n");
    printf("DETECTIVE QUEST – NÍVEL NOVATO\n");
    printf("========================================\n");
    printf("Você está no Hall de Entrada da mansão.\n");

    explorarSalas(raiz);

    printf("\nObrigado por jogar Detective Quest!\n");

    return 0;

    // 🔍 Nível Aventureiro: Armazenamento de Pistas com Árvore de Busca
    //
    // - Crie uma struct Pista com campo texto (string).
    // - Crie uma árvore binária de busca (BST) para inserir as pistas coletadas.
    // - Ao visitar salas específicas, adicione pistas automaticamente com inserirBST().
    // - Implemente uma função para exibir as pistas em ordem alfabética (emOrdem()).
    // - Utilize alocação dinâmica e comparação de strings (strcmp) para organizar.
    // - Não precisa remover ou balancear a árvore.
    // - Use funções para modularizar: inserirPista(), listarPistas().
    // - A árvore de pistas deve ser exibida quando o jogador quiser revisar evidências.

    // 🧠 Nível Mestre: Relacionamento de Pistas com Suspeitos via Hash
    //
    // - Crie uma struct Suspeito contendo nome e lista de pistas associadas.
    // - Crie uma tabela hash (ex: array de ponteiros para listas encadeadas).
    // - A chave pode ser o nome do suspeito ou derivada das pistas.
    // - Implemente uma função inserirHash(pista, suspeito) para registrar relações.
    // - Crie uma função para mostrar todos os suspeitos e suas respectivas pistas.
    // - Adicione um contador para saber qual suspeito foi mais citado.
    // - Exiba ao final o “suspeito mais provável” baseado nas pistas coletadas.
    // - Para hashing simples, pode usar soma dos valores ASCII do nome ou primeira letra.
    // - Em caso de colisão, use lista encadeada para tratar.
    // - Modularize com funções como inicializarHash(), buscarSuspeito(), listarAssociacoes().

    return 0;
}

