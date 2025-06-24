#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct produto {
    int codigo;
    char nome[30];
    int qtd;
    char categoria[11];
    float precos[3]; // preços a serem atualizados
    struct produto *prox;
} Produto;

// Cria um novo produto com dados do arquivo estoque.txt
Produto *cria_produto_arquivo(int codigo, char *nome, int qtd, char *categoria) {
    Produto *novo = (Produto *)malloc(sizeof(Produto));
    if (novo) {
        novo->codigo = codigo;
        strncpy(novo->nome, nome, sizeof(novo->nome));
        novo->nome[sizeof(novo->nome) - 1] = '\0'; // segurança extra
        novo->qtd = qtd;
        strncpy(novo->categoria, categoria, sizeof(novo->categoria));
        novo->categoria[sizeof(novo->categoria) - 1] = '\0';
        novo->precos[0] = novo->precos[1] = novo->precos[2] = 0.0f; // inicializa preços
        novo->prox = NULL;
    } else {
        printf("Problema na alocação de produto\n");
    }
    return novo;
}

// Insere o produto no fim da lista
void insere_produto_lista(Produto **lista, Produto *novo) {
    if (*lista == NULL) {
        *lista = novo;
    } else {
        Produto *atual = *lista;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo;
    }
}

// Lê os produtos do estoque.txt e preenche a lista
void ler_estoque(const char *nomeArquivo, Produto **lista) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de estoque");
        return;
    }

    char linha[100];
    int codigo, qtd;
    char nome[30], categoria[11];

    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remove a quebra de linha, se houver
        linha[strcspn(linha, "\n")] = '\0';

        // Tenta extrair os dados
        if (sscanf(linha, "%d %29s %d %10s", &codigo, nome, &qtd, categoria) == 4) 
        {
            Produto *novo = cria_produto_arquivo(codigo, nome, qtd, categoria);
            if (novo) {
                insere_produto_lista(lista, novo);
            }
        } else {
            printf("Linha inválida: %s\n", linha);
        }
    }

    fclose(arquivo);
}

// Atualiza os preços dos produtos com base no arquivo precos.txt
void atualizar_precos(const char *nomeArquivo, Produto *lista) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo de preços");
        return;
    }

    int codigo;
    float p1, p2, p3;

    while (fscanf(arquivo, "%d %f %f %f", &codigo, &p1, &p2, &p3) == 4) {
        Produto *atual = lista;

        // procura o produto correspondente pelo código
        while (atual != NULL) {
            if (atual->codigo == codigo) {
                atual->precos[0] = p1;
                atual->precos[1] = p2;
                atual->precos[2] = p3;
                break;
            }
            atual = atual->prox;
        }
    }

    fclose(arquivo);
}

// Imprime todos os produtos da lista
void imprime_lista(Produto *lista) {
    while (lista != NULL) {
        printf("Código: %d | Nome: %s | Quantidade: %d | Categoria: %s\n",
               lista->codigo, lista->nome, lista->qtd, lista->categoria);
        printf("  Preços: R$ %.2f | R$ %.2f | R$ %.2f\n",
               lista->precos[0], lista->precos[1], lista->precos[2]);
        lista = lista->prox;
    }
}

// Libera memória da lista
void liberar_lista(Produto *lista) {
    Produto *temp;
    while (lista) {
        temp = lista;
        lista = lista->prox;
        free(temp);
    }
}

// Função principal
int main() {
    Produto *lista_produtos = NULL;

    ler_estoque("estoque.txt", &lista_produtos);
    atualizar_precos("precos.txt", lista_produtos);
    imprime_lista(lista_produtos);
    liberar_lista(lista_produtos);

    return 0;
}
