#include "clientes.h"
#include "produtos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Cores para output
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

//Alternativa mais segura a scanf, e mais facil de recuperar de erros
int ler_int() {
  char buffer[256];
  int retorno;

  printf(BLUE">"RESET);

  if(fgets(buffer, 256, stdin)) {
    if (1 == sscanf(buffer, "%d", &retorno)) {
      return(retorno);
    }
  }
  return(0);
}

long long int ler_cpf() {
  char buffer[256];
  long long int retorno;

  printf(BLUE"[CPF] >"RESET);

  if(fgets(buffer, 256, stdin)) {
    if (1 == sscanf(buffer, "%lld", &retorno)) {
      return(retorno);
    }
  }
  return(0);
}

char *ler_nome() {
  char buffer[256];
  char *retorno;

  printf(BLUE"[NOME] >"RESET);

  if(fgets(buffer, 256, stdin)) {
    if (1 == sscanf(buffer, "%[^\n]", buffer)) {
      retorno = strdup(buffer);
      return(retorno);
    }
  }
  return(NULL);
}

char *ler_email() {
  char buffer[256];
  char *retorno;

  printf(BLUE"[EMAIL] >"RESET);

  if(fgets(buffer, 256, stdin)) {
    if (1 == sscanf(buffer, "%s", buffer)) {
      retorno = strdup(buffer);
      return(retorno);
    }
  }
  return(NULL);
}

void imprimir_cliente(cliente *in) {
  char *info = cliente_info(in);
  printf("%s\n", info);
  free(info);
}

int main() {
  char arquivo_clientes[] = "data/clientes.txt";
  char arquivo_produtos[] = "data/produtos.txt";


  printf(CYAN"  __  __                             _  _         _            \n");
  printf(" |  \\/  |                           | |(_)       | |           \n");
  printf(" | \\  / |  ___  _ __  ___  __ _   __| | _  _ __  | |__    ___  \n");
  printf(" | |\\/| | / _ \\| '__|/ __|/ _` | / _` || || '_ \\ | '_ \\  / _ \\ \n");
  printf(" | |  | ||  __/| |  | (__| (_| || (_| || || | | || | | || (_) |\n");
  printf(" |_|  |_| \\___||_|   \\___|\\__,_| \\__,_||_||_| |_||_| |_| \\___/ \n");
  printf("  ____   _          ____                                       \n");
  printf(" |  _ \\ (_)        |  _ \\                                      \n");
  printf(" | |_) | _   __ _  | |_) |  ___    ___   _ __ ___              \n");
  printf(" |  _ < | | / _` | |  _ <  / _ \\  / _ \\ | '_ ` _ \\             \n");
  printf(" | |_) || || (_| | | |_) || (_) || (_) || | | | | |            \n");
  printf(" |____/ |_| \\__, | |____/  \\___/  \\___/ |_| |_| |_|            \n");
  printf("             __/ |                                             \n");
  printf("            |___/                                              \n");
  printf(RESET"\n");

  printf("Seja bem-vindo ao sistema de gerência do mercadinho big boom!\n");
  
  while(1) {
    printf("\n");
    printf(CYAN"========\n");
    printf("Menu\n");
    printf("========\n"RESET);
    printf("O que você deseja gerir? Digite o número.\n");
    printf(GREEN"1 - Clientes\n"RESET);
    printf(GREEN"2 - Produtos\n"RESET);
    printf(RED  "0 - Sair\n"RESET);
    
    int escolha = ler_int();

    //Menu Clientes
    if (escolha == 1) {
      //Repetir até quebrar
      while(1) {
        printf("\n");
        printf(CYAN"========\n");
        printf("Clientes\n");
        printf("========\n"RESET);
        printf("O que você deseja fazer?\n");
        printf(GREEN"1 - Listar todos os clientes\n"RESET);
        printf(GREEN"2 - Consultar um cliente\n"RESET);
        printf(GREEN"3 - Cadastrar um cliente\n"RESET);
        printf(GREEN"4 - Remover um cliente\n"RESET);
        printf(RED  "0 - Voltar ao menu\n"RESET);
  
        //Ler numero
        int escolha = ler_int();

        //Listar clientes
        if (escolha == 1) {
          //Criar vetor com clientes
          cliente **vetor = cliente_list(arquivo_clientes);
          //Caso lido corretamente
          if (vetor != NULL) {
            printf(BLUE"------\n"RESET);
            for (int i = 0; vetor[i] != NULL; i++) {
              //Imprimir cada cliente
              imprimir_cliente(vetor[i]);
              //E desalocar logo em seguida
              cliente_destruir(vetor[i]);
            }
            //Desalocar vetor
            free(vetor);
            printf(BLUE"------\n"RESET);
          }
        }

        //Consultar um cliente
        else if (escolha == 2) {
          printf("Digite o CPF do cliente (apenas números): \n");
          //Receber cpf do usuario, buscar utilizando ele
          //E atribuir a instancia
          cliente *consulta = cliente_get(arquivo_clientes, ler_cpf());
          if (consulta != NULL) {
            printf(BLUE"------\n"RESET);
            //Imprimir instancia
            imprimir_cliente(consulta);
            //Destruir instancia
            cliente_destruir(consulta);
            printf(BLUE"------\n"RESET);
          }
        }
   
        //Cadastrar um cliente
        else if (escolha == 3) {
          printf("Digite o CPF (apenas numeros), nome e email do cliente:\n");
          long long int cpf = ler_cpf();
          char *nome = ler_nome();
          char *email = ler_email();
          
          //Criar nova instancia de cliente
          cliente *novo = cliente_criar(cpf, nome, email);
          //Desalocar as strings lidas
          free(nome);
          free(email);

          //Postar instancia na database
          //Caso seja bem-sucedido (resposta 0), imprimir na tela avisando
          if (cliente_post(arquivo_clientes, novo) == 0) printf(GREEN"Cliente adicionado!\n"RESET);
          //Desalocar instancia
          cliente_destruir(novo);   
        }

        //Apagar um cliente
        else if (escolha == 4) {
          printf("Digite o CPF do cliente (apenas números): \n");
          //Ler cpf e apagar cliente da database
          //Caso seja bem-sucedido (resposta 0), imprimir na tela avisando
          if (cliente_delete(arquivo_clientes, ler_cpf()) == 0) printf(GREEN"Cliente removido!\n"RESET);
        }

        //Sair do menu
        else {
          break;
        }
      }
    }
    
    //Menu Produtos
    else if (escolha == 2) {
      while(1) {
        printf("\n");
        printf(CYAN"========\n");
        printf("Produtos\n");
        printf("========\n"RESET);
        printf("O que você deseja fazer?\n");
        printf(GREEN"1 - Listar todos os clientes\n"RESET);
        printf(GREEN"2 - Consultar um cliente\n"RESET);
        printf(GREEN"3 - Cadastrar um cliente\n"RESET);
        printf(GREEN"4 - Remover um cliente\n"RESET);
        printf(RED  "0 - Voltar ao menu\n"RESET);
      }
    }

    //Sair do programa
    else {
      break;
    }
  }

  printf(CYAN"==\n"RESET);
  printf("Volte sempre!\n");
  return(0);

  
}

