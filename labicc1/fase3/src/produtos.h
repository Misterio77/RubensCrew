#ifndef PRODUTOS_H_
#define PRODUTOS_H_

typedef struct _produto produto;

produto *produto_criar(int id, char *nome, float valor, int estoque);
void produto_destruir(produto *in);
char *produto_info(produto *in);

produto **produto_list();
produto *produto_get(int id);
int produto_post(produto *in);
int produto_delete(int id);

#endif
