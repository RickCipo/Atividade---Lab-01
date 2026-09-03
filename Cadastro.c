#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da struct
//Representa um item no estoque. 'nome' é um ponteiro (char *) em vez de um vetor fixo pois assim alocamos exatamente o espaço necessário para cada nome
typedef struct {
    int codigo;
    char *nome;
    float preco;
    int quantidade;
} Produto;

//Adicionar produtos
// Recebe Produto** e int* porque precisa alterar as variuaveis que o main() possui: ponteiro 'estoque' e o contador 'total_produtos'
//Se usamos Produto* e int, as alterações poderiam ser pedirdas no final da função
void adicionar_produto(Produto **estoque, int *total_produtos) {
    char buffer_nome[100]; //buffer temporario para ler a entrada do usuario
    char preco_str[20];
    float preco_temp;
    int quantidade_temp;

    printf("\n--- Adicionar Produto ---\n");
    printf("Nome: ");
    scanf(" %[^\n]", buffer_nome); //le a linha inteira
    printf("Preco: ");
    scanf("%19s", preco_str);

    //troca virgula por ponto antes do atof
    for (int i = 0; preco_str[i] != '\0'; i++) {
        if (preco_str[i] == ',') {
            preco_str[i] = '.';
        }
    }
    preco_temp = atof(preco_str);

    printf("Quantidade: ");
    scanf("%d", &quantidade_temp);

    //cresce o vetor de structs em 1 posição, preservando os produtos que ja existem
    //usamos realloc pois mantém o conteúdo antigo;
    //com malloc teríamos que alocar, copiar manualmente tudo e liberar o antigo.
    int novo_tamanho = (*total_produtos) + 1;
    Produto *novo_estoque = realloc(*estoque, novo_tamanho * sizeof(Produto));

    if(novo_estoque == NULL) {
        printf("Erro fatal: Nao foi possivel alocar memoria para o vetor de produtos.\n");
        return;
    }

    *estoque = novo_estoque; //atualiza o ponteiro do main, pois o endereço pode ter mudado
    int indice = *total_produtos;

    //aloca memória própria (no heap) só para o nome deste produto, do tamanho exato da string + 1 para o '\0'
    (*estoque)[indice].nome = (char *)malloc((strlen(buffer_nome) + 1) * sizeof(char));

    if ((*estoque)[indice].nome == NULL) {
        printf("Erro fatal: Nao foi possivel alocar memoria para o nome do produto.\n");
        return;
    }
    
    strcpy((*estoque)[indice].nome, buffer_nome);

    (*estoque)[indice].codigo = indice + 1; 
    (*estoque)[indice].preco = preco_temp;
    (*estoque)[indice].quantidade = quantidade_temp;

    //incrementa o contador total de produtos
    (*total_produtos)++;

    printf("Produto adicionado com codigo %d!\n", (*estoque)[indice].codigo);

}

//listar produtos
//apenas le os dados, por isso recebe Produto* e int por valor
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

//buscar produto
//retorna Produto* (ponteiro para dentro do vetor original), não uma cópia (Produto)
//permite que quem chamou a função possa modificar o produto e retornar NULL para sinalizar 'não encontrado'
Produto* buscar_produto(Produto *estoque, int total_produtos, int codigo){
    for(int i = 0; i < total_produtos; i++){
        if (estoque[i].codigo == codigo){
            return &estoque[i];
        }
    }
    return NULL;
}

//armazenar estoque
//recebe Produto* e n'ao Produto** pois não precisa trocar o endereço do vetor nem seu tamanho, apenas alterar um campo de um elemento que ja existe
//o ponteiro retornado por buscar_produto aponta pro vetor original, então "produto->quantidade = ..." já modifica o estoque de verdade (passagem por referência via ponteiro)
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

//remover produtos
//recebe Produto** e int* pelo mesmo motivo de adicionar_produto: o realloc pode mudar o endereço do vetor, e o total_produtos precisa diminuir 'de verdade' no main()
void remover_produto(Produto **estoque, int *total_produtos) {
    int codigo;

    printf("\n--- Remover Produto ---\n");
    printf("Codigo do produto a ser removido: ");
    scanf("%d", &codigo);

    Produto *produto = buscar_produto(*estoque, *total_produtos, codigo);

    if (produto == NULL) {
        printf("Produto com codigo %d nao encontrado.\n", codigo);
        return;
    }

    int indice = produto - *estoque; //calcula a posição a partir do endereço

    printf("Produto encontrado: %s\n", produto->nome);

    //liberamos a string do nome ANTES de sobrescrever essa posição no loop abaixo. Se não fizéssemos isso agora, o ponteiro
    //seria perdido no shift e essa memória vazaria
    free(produto->nome);
    produto->nome = NULL;


    //desloca todos os produtos seguintes uma posição para trás, "apagando" o produto removido do vetor
    for (int i = indice; i < (*total_produtos) - 1; i++) {
        (*estoque)[i] = (*estoque)[i + 1];
    }


    (*total_produtos)--;

    
    if (*total_produtos == 0) {
        free(*estoque);
        *estoque = NULL;
    } else {
        //encolhe o vetor para o novo tamanho (evita desperdicio de memoria)
        Produto *novo_estoque = realloc(
            *estoque,
            (*total_produtos) * sizeof(Produto)
        );

        if (novo_estoque != NULL) {
            *estoque = novo_estoque;
        } else {
            printf("Aviso: nao foi possivel reduzir o tamanho do vetor.\n");
            printf("Os produtos continuam armazenados corretamente.\n");
        }
    }

    printf("Produto removido com sucesso!\n");
}

//liberar memoria
//libera toda a memoria alocada dinamicamente, na ordem correta: primeiro cada string 'nome' (alocada individualmente), depois o
//vetor 'estoque' em si. Se liberassemos o vetor primeiro, perderíamos os ponteiros para os nomes e não poderíamos mais liberá-los -> leak.
void liberar_memoria(Produto *estoque, int total_produtos) {
    printf("\nLiberando memoria...\n");

    for (int i = 0; i < total_produtos; i++) {
        if (estoque[i].nome != NULL) {
            printf("Memoria do produto \"%s\" liberada.\n",
                   estoque[i].nome);

            free(estoque[i].nome);
            estoque[i].nome = NULL;
        }
    }

    //libera o vetor
    free(estoque);

    printf("Vetor de produtos liberado.\n");
    printf("Memoria liberada com sucesso!\n");
    printf("Programa encerrado.\n");
}

int main() {
    Produto *estoque = NULL;  //vetor dinamico começa vazio
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
                //passa &estoque e &total_produtos: a função PRECISA poder alterar essas duas variáveis do main.
                adicionar_produto(&estoque, &total_produtos);
                break;
            
            case 2:
                listar_produtos(estoque, total_produtos);
                break;
            
            case 3: { 
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
            }
                break;
            
            case 4:
                atualizar_estoque(estoque, total_produtos);
                break;
            
            case 5:
                remover_produto(&estoque, &total_produtos);
                break;
            
            case 6:
                printf("\nLiberando memoria...\n");
                
                liberar_memoria( estoque, total_produtos);
                break;
            
            default:
                printf("\nOpcao invalida! Digite um numero entre 1 e 6.\n");
        }

    } while (opcao != 6);

    return 0;
}
