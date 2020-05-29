#include "clientes.h"
#include "produtos.h"
#include <stdio.h>

int main() {
  produto *a = produto_get(1234);
  if (a != NULL) {
    printf("%s\n", produto_info(a));
    produto_destruir(a);
 }
}
