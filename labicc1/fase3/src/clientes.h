#ifndef CLIENTES_H_
#define CLIENTES_H_

typedef struct _cliente cliente;

cliente *cliente_criar(long long int cpf, char *nome, char *email, int silent);
void cliente_destruir(cliente *in);
char *cliente_info(cliente *in, int silent);

cliente **cliente_list(char *database, int silent);
cliente *cliente_get(char *database, long long int cpf, int silent);
int cliente_post(char *database, cliente *in, int silent);
int cliente_delete(char *database, long long int cpf, int silent);

#endif
