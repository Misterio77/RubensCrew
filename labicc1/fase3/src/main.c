#include "clientes.h"
#include <stdio.h>

int main() {
  cliente *a = cliente_get(23858708860);
  printf("%s\n", cliente_info(a));
  cliente_destruir(a);
}
