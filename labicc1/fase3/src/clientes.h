#ifndef CLIENTES_H_
#define CLIENTES_H_

typedef struct _cliente cliente;

cliente *cliente_criar(long long int cpf, char *nome, char *email);
void cliente_destruir(cliente *in);
char *cliente_info(cliente *in);

cliente **cliente_list(char *database);
cliente *cliente_get(char *database, long long int cpf);
int cliente_post(char *database, cliente *in);
int cliente_delete(char *database, long long int cpf);

#endif
