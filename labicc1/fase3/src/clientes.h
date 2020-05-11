#ifndef CLIENTES_H_
#define CLIENTES_H_

typedef struct _cliente cliente;

cliente *cliente_criar(long long int cpf, char *nome, char *email);
void cliente_destruir(cliente *in);
char *cliente_info(cliente *in);

cliente **cliente_list();
cliente *cliente_get(long long int cpf);
cliente *cliente_post(cliente *in);
int cliente_delete(long long int cpf);

#endif
