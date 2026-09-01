// Definição da struct
typedef struct {
    int codigo;
    char *nome;
    float preco;
    int quantidade;
} Produto;

// Funções obrigatórias (você define os parâmetros)
void adicionar_produto(...);
void listar_produtos(...);
Produto* buscar_produto(...);
void atualizar_estoque(...);
void remover_produto(...);
void liberar_memoria(...);