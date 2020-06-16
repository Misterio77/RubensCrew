/* Trabalho Lab ICC1 - Sistema de mercado
 *
 * 10856803 - Gabriel Silva Fontes
 * 11912533 - Samuel Rubens
 * 11816139 - Julio Igor Casemiro Oliveira
 * xxxxxxxx - João Pedro
 * xxxxxxxx - Augusto
 *
 */


#include "clientes.h"
#include "produtos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

//Cores para output
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"


/*
Função generica pra ler um numero inteiro.
Usa fgets e sscanf pra ler de forma segura
O argumento prompt é usado para exibir algo ao usuário
Retorna 0 em caso de erro
*/
long long int ler_num(char *prompt) {
  char buffer[256];
  long long int retorno;

  printf(BLUE"%s"RESET, prompt);

  if(fgets(buffer, 256, stdin)) {
    if (1 == sscanf(buffer, "%lld", &retorno)) {
      return(retorno);
    }
  }
  return(0);
}

/*
Função generica pra ler um numero decimal.
Usa fgets e sscanf pra ler de forma segura
O argumento prompt é usado para exibir algo ao usuário
Retorna 0 em caso de erro
*/
double ler_dec(char *prompt) {
  char buffer[256];
  double retorno;

  printf(BLUE"%s"RESET, prompt);

  if(fgets(buffer, 256, stdin)) {
    if (1 == sscanf(buffer, "%lf", &retorno)) {
      return(retorno);
    }
  }
  return(0);
}

/*
Função generica pra ler string
Usa fgets e sscanf pra ler de forma segura
O argumento prompt é usado para exibir algo ao usuário
Retorna NULL em caso de erro
*/
char *ler_string(char *prompt) {
  char buffer[256];
  char *retorno;

  printf(BLUE"%s"RESET, prompt);

  if(fgets(buffer, 256, stdin)) {
    if (1 == sscanf(buffer, "%[^\n]", buffer)) {
      retorno = strdup(buffer);
      return(retorno);
    }
  }
  return(NULL);
}

void imprimir_cliente(cliente *in) {
  char *info = cliente_info(in, 0);
  printf("%s\n", info);
  free(info);
}

void imprimir_produto(produto *in) {
  char *info = produto_info(in, 0);
  printf("%s\n", info);
  free(info);
}

int main() {
  setlocale(LC_ALL, "");
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
    
    int escolha = ler_num(">");

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
        int escolha = ler_num(">");

        //Listar clientes
        if (escolha == 1) {
          //Criar vetor com clientes
          cliente **vetor = cliente_list(arquivo_clientes, 0);
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
          cliente *consulta = cliente_get(arquivo_clientes, ler_num("[CPF] >"), 0);
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
          long long int cpf = ler_num("[CPF] >");
          char *nome = ler_string("[NOME] >");
          char *email = ler_string("[EMAIL] >");
          
          //Criar nova instancia de cliente
          cliente *novo = cliente_criar(cpf, nome, email, 0);
          //Desalocar as strings lidas
          free(nome);
          free(email);

          //Postar instancia na database
          //Caso seja bem-sucedido (resposta 0), imprimir na tela avisando
          if (cliente_post(arquivo_clientes, novo, 0) == 0) printf(GREEN"Cliente adicionado!\n"RESET);
          //Desalocar instancia
          cliente_destruir(novo);   
        }

        //Apagar um cliente
        else if (escolha == 4) {
          printf("Digite o CPF do cliente (apenas números): \n");
          long long int cpf = ler_num("[CPF] >");
          //Ler cpf e apagar cliente da database
          //Caso seja bem-sucedido (resposta 0), imprimir na tela avisando
          if (cliente_delete(arquivo_clientes, cpf, 0) == 0) printf(GREEN"Cliente removido!\n"RESET);
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
        printf(GREEN"1 - Listar todos os produtos\n"RESET);
        printf(GREEN"2 - Consultar um produto\n"RESET);
        printf(GREEN"3 - Cadastrar um produto\n"RESET);
        printf(GREEN"4 - Remover um produto\n"RESET);
        printf(GREEN"5 - Alterar o estoque de um produto\n"RESET);
        printf(RED  "0 - Voltar ao menu\n"RESET);
        
        //Ler numero
        int escolha = ler_num(">");

        //Listar produtos
        if (escolha == 1) {
          //Criar vetor com produtos
          produto **vetor = produto_list(arquivo_produtos, 0);
          //Caso lido corretamente
          if (vetor != NULL) {
            printf(BLUE"------\n"RESET);
            for (int i = 0; vetor[i] != NULL; i++) {
              //Imprimir cada produto
              imprimir_produto(vetor[i]);
              //E desalocar logo em seguida
              produto_destruir(vetor[i]);
            }
            //Desalocar vetor
            free(vetor);
            printf(BLUE"------\n"RESET);
          }
        }

        //Consultar um produto
        else if (escolha == 2) {
          printf("Digite o ID do produto: \n");
          //Receber cpf do usuario, buscar utilizando ele
          //E atribuir a instancia
          produto *consulta = produto_get(arquivo_produtos, ler_num("[ID] >"), 0);
          if (consulta != NULL) {
            printf(BLUE"------\n"RESET);
            //Imprimir instancia
            imprimir_produto(consulta);
            //Destruir instancia
            produto_destruir(consulta);
            printf(BLUE"------\n"RESET);
          }
        }
   
        //Cadastrar um produto
        else if (escolha == 3) {
          printf("Digite o ID, nome, valor (apenas números e '%s') e estoque do produto:\n", localeconv()->mon_decimal_point);
          int id = ler_num("[ID] >");
          char *nome = ler_string("[NOME] >");
          float valor = ler_dec("[VALOR] >");
          int estoque = ler_num("[ESTOQUE] >");
 
          //Criar nova instancia de produto
          produto *novo = produto_criar(id, nome, valor, estoque, 0);
          //Desalocar a string lida
          free(nome);

          //Postar instancia na database
          //Caso seja bem-sucedido (resposta 0), imprimir na tela avisando
          if (produto_post(arquivo_produtos, novo, 0) == 0) printf(GREEN"Produto adicionado!\n"RESET);
          //Desalocar instancia
          produto_destruir(novo);
        }

        //Apagar um produto
        else if (escolha == 4) {
          printf("Digite o ID do produto: \n");
          int id = ler_num("[ID] >");
          //Ler id e produto da database
          //Caso seja bem-sucedido (resposta 0), imprimir na tela avisando
          if (produto_delete(arquivo_produtos, id, 0) == 0) printf(GREEN"Produto removido!\n"RESET);
        }

        else if (escolha == 5) {
          printf("Digite o ID do produto e a mudança de estoque (negativo para retirar)\n");
          int id = ler_num("[ID] >");
          int mudanca = ler_num("[ESTOQUE] >");
          if (produto_alterar_estoque(arquivo_produtos, id, mudanca, 0) == 0) printf("Estoque alterado!");
        }

        //Sair do menu
        else {
          break;
        }
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

