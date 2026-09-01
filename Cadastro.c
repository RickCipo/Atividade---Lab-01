#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da struct
typedef struct {
    int codigo;
    char *nome;
    float preco;
    int quantidade;
} Produto;

// Funções obrigatórias (você define os parâmetros)
void adicionar_produto(Produto **estoque, int *total_produtos) {
    char buffer_nome[100];
    float preco_temp;
    int quantidade_temp;

    printf("\n--- Adicionar Produto ---\n");
    printf("Nome: ");
    scanf(" %[^\n]", buffer_nome);
    printf("Preco: ");
    scanf("%f", &preco_temp);
    
    printf("Quantidade: ");
    scanf("%d", &quantidade_temp);


    int novo_tamanho = (*total_produtos) + 1;
    Produto *novo_estoque = realloc(*estoque, novo_tamanho * sizeof(Produto));

    if(novo_estoque == NULL) {
        printf("Erro fatal: Nao foi possivel alocar memoria para o vetor de produtos.\n");
        return;
    }

    *estoque = novo_estoque;
    int indice = *total_produtos;

    (*estoque)[indice].nome = (char *)malloc((strlen(buffer_nome) + 1) * sizeof(char));

    if ((*estoque)[indice].nome == NULL) {
        printf("Erro fatal: Nao foi possivel alocar memoria para o nome do produto.\n");
        return;
    }
    
    strcpy((*estoque)[indice].nome, buffer_nome);

    (*estoque)[indice].codigo = indice + 1; 
    (*estoque)[indice].preco = preco_temp;
    (*estoque)[indice].quantidade = quantidade_temp;

    // Incrementa o contador total de produtos
    (*total_produtos)++;

    printf("Produto adicionado com codigo %d!\n", (*estoque)[indice].codigo);

}
void listar_produtos(...);
Produto* buscar_produto(...);
void atualizar_estoque(...);
void remover_produto(...);
void liberar_memoria(...);

int main() {
    // INICIALIZAÇÃO CRÍTICA:
    // O ponteiro deve começar como NULL. Assim, quando o realloc rodar pela primeira vez, 
    // ele vai entender que precisa se comportar igual a um malloc comum.
    Produto *estoque = NULL; 
    int total_produtos = 0;

    printf("=== TESTE DO SISTEMA ===\n");

    // Vamos adicionar 2 produtos para testar.
    // Usamos '&' para passar o ENDEREÇO da memória (ponteiros) para a função.
    adicionar_produto(&estoque, &total_produtos);
    adicionar_produto(&estoque, &total_produtos);

    // Um "mini" listar produtos improvisado só para provar que salvou:
    printf("\n--- VERIFICACAO DO QUE FOI SALVO ---\n");
    for (int i = 0; i < total_produtos; i++) {
        printf("Cod: %d | Nome: %s | Preco: %.2f | Qtd: %d\n", 
               estoque[i].codigo, estoque [i].nome, estoque[i].preco, estoque[i].quantidade);
    }

    // Mini limpeza de memória improvisada (depois faremos uma função oficial para isso)
    for (int i = 0; i < total_produtos; i++) {
        free(estoque[i].nome); // Libera o nome de cada produto
    }
    free(estoque); // Libera o vetor principal

    printf("\nMemoria liberada e programa encerrado com sucesso.\n");
    return 0;
}