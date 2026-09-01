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
    char preco_str[20];
    float preco_temp;
    int quantidade_temp;

    printf("\n--- Adicionar Produto ---\n");
    printf("Nome: ");
    scanf(" %[^\n]", buffer_nome);
    printf("Preco: ");
    scanf("%19s", preco_str);
    
    for (int i = 0; preco_str[i] != '\0'; i++) {
        if (preco_str[i] == ',') {
            preco_str[i] = '.';
        }
    }
    preco_temp = atof(preco_str);

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
void listar_produtos(Produto *estoque, int total_produtos){
    if (total_produtos == 0){
        printf("Nenhum produto cadastrado,\n");
        return;
    }

    printf("\n--- Lista de Produtos ---\n");
    printf("+--------+--------------------+----------+------+---------------+\n");
    printf("| Codigo | Nome               | Preco    | Qtd  | Valor Estoque |\n");
    printf("+--------+--------------------+----------+------+---------------+\n");

    float valor_total_estoque = 0;

    for (int i = 0; i < total_produtos; i++){
        float valor_estoque = estoque[i].preco * estoque[i].quantidade;
        valor_total_estoque += valor_estoque;

        printf("| %6d | %-18s | %8.2f | %4d | %13.2f |\n",
               estoque[i].codigo,
               estoque[i].nome,
               estoque[i].preco,
               estoque[i].quantidade,
               valor_estoque);
    }

    printf("+--------+--------------------+----------+------+---------------+\n");
    printf("Valor total do estoque: R$ %.2f\n", valor_total_estoque);

}

Produto* buscar_produto(Produto *estoque, int total_produtos, int codigo){
    for(int i = 0; i < total_produtos; i++){
        if (estoque[i].codigo == codigo){
            return &estoque[i];
        }
    }
    return NULL;
}

void atualizar_estoque(Produto *estoque, int total_produtos){
    int codigo;
    int nova_quantidade;

    printf("Codigo do produto: \n");
    scanf("%d", &codigo);

    Produto *produto = buscar_produto(estoque, total_produtos, codigo);

    if (produto == NULL){
        printf("Produto com codigo %d nao encontrado.\n", codigo);
        return;
    }

    printf("Produto encontrado: %s (quantidade atual: %d)\n", produto->nome, produto->quantidade);

    printf("Nova quantidade: ");
    scanf("%d", &nova_quantidade);

    produto->quantidade = nova_quantidade;
    printf("Estoque atualizado com sucesso!\n");

}
void remover_produto(...);
void liberar_memoria(...);

int main() {
    Produto *estoque = NULL; 
    int total_produtos = 0;
    int opcao;

    do {
        // Imprime o cabeçalho idêntico à imagem
        printf("\n========================================\n");
        printf("    SISTEMA DE CADASTRO DE PRODUTOS\n");
        printf("========================================\n\n");
        printf("Menu:\n");
        printf("1. Adicionar produto\n");
        printf("2. Listar produtos\n");
        printf("3. Buscar produto\n");
        printf("4. Atualizar estoque\n");
        printf("5. Remover produto\n");
        printf("6. Sair\n\n");
        
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                adicionar_produto(&estoque, &total_produtos);
                break;
            
            case 2:
                listar_produtos(estoque, total_produtos);
                break;
            
            case 3: 
                int codigo_busca;
                printf("Codigo do produto: \n");
                scanf("%d", &codigo_busca);

                Produto *encontrado = buscar_produto(estoque, total_produtos, codigo_busca);

                if(encontrado != NULL){

                    printf("+--------+--------------------+----------+------+\n");
                    printf("| Codigo | Nome               | Preco    | Qtd  |\n");
                    printf("+--------+--------------------+----------+------+\n");

                    printf("| %6d | %-18s | %8.2f | %4d |\n",
                        encontrado->codigo,
                        encontrado->nome,
                        encontrado->preco,
                        encontrado->quantidade);
                    printf("+--------+--------------------+----------+------+\n");
                }else{
                    printf("Produto %d não encontrado", codigo_busca);
                }
            
                break;
            
            case 4:
                atualizar_estoque(estoque, total_produtos);
                break;
            
            case 5:
                // TODO: Implementar a função remover_produto
                printf("\n[Em desenvolvimento] Remocao de produto...\n");
                break;
            
            case 6:
                printf("\nLiberando memoria...\n");
                
                // TODO: Idealmente, moveremos isso para uma função void liberar_memoria(Produto *estoque, int total)
                for (int i = 0; i < total_produtos; i++) {
                    printf("Memoria do produto \"%s\" liberada.\n", estoque[i].nome);
                    free(estoque[i].nome); // Libera o nome alocado com malloc
                }
                free(estoque); // Libera o vetor alocado com realloc
                printf("Vetor de produtos liberado.\n");
                
                printf("Programa encerrado.\n");
                break;
            
            default:
                printf("\nOpcao invalida! Digite um numero entre 1 e 6.\n");
        }

    } while (opcao != 6);

    return 0;
}
