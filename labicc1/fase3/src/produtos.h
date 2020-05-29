#ifndef PRODUTOS_H_
#define PRODUTOS_H_

typedef struct _produto produto;

produto *produto_criar(int id, char *nome, float valor, int estoque);
void produto_destruir(produto *in);
char *produto_info(produto *in);

produto **produto_list(char *database);
produto *produto_get(char *database, int id);
int produto_post(char *database, produto *in);
int produto_delete(char *database, int id);

#endif
