#ifndef PRODUTOS_H_
#define PRODUTOS_H_

typedef struct _produto produto;

produto *produto_criar(int id, char *nome, float valor, int estoque, int silent);
void produto_destruir(produto *in);
char *produto_info(produto *in, int silent);

produto **produto_list(char *database, int silent);
produto *produto_get(char *database, int id, int silent);
int produto_post(char *database, produto *in, int silent);
int produto_delete(char *database, int id, int silent);
int produto_alterar_estoque(char *database, int id, int mudanca, int silent);

#endif
