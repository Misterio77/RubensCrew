#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "produtos.h"

struct produto {
  int id; //Identificador do produto
  char *nome; //String, nome do produto
  float valor; //Valor monetário do produto
  int estoque; //Quantidade no estoque
};

/*Retorna um vetor com todos os produtos armazenados*/
produto **produtos_get_all() {

};

/*Retorna um produto, dado seu id*/
produto *produtos_get(int id) {

};

/*Modifica um produto, dado seu id. Cria caso não exista, idempotente. Retorna ele atualizado caso seja bem sucedido, NULL caso contrário.*/
produto *produtos_put(int id, char *nome, float valor, int estoque) {

};

/*Adiciona um novo produto, dado seu id. Retorna ele caso seja bem sucedido, NULL caso contrário*/
produto *produtos_post(int id, char *nome, float valor, int estoque) {
};

/*Apaga um produto, dado seu id. Retorna true caso bem sucedido, false caso contrário*/
bool produtos_del(int id) {

};
