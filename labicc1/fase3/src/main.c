#include "clientes.h"
#include <stdio.h>

int main() {
  cliente *a = cliente_get(23858708860);
  cliente_destruir(a);
}
