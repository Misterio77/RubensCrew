#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //Booleans
#include <string.h>  //Funções para strings

#include "clientes.h"

#define LINESIZE 200 //Tamanho para cada linha

//== Estrutura (privada) ==

struct cliente {
  long long int cpf; //Numero de 9 digitos, cpf do cliente
  char *nome; //String, nome do cliente
  char *email; //String, do formato exemplo@exemplo.com, email do cliente
};

//== Funções privadas ==
/*Corta e formata uma linha (um cliente), aloca a estrutura e retorna*/
cliente *clientes_parse(char *in) {
  char *in_cpf = strtol(strtok(in, "; "), NULL, 10);
  char *in_nome = strtok(NULL, "; ");
  char *in_email = strtok(NULL, "; ");

  //Cliente a ser retornoado
  cliente *out = malloc(sizeof(cliente));

  //Verificar se algum campo deu NULL, ou se a conversão do cpf não funcionou;
  if (cpf == 0 || nome == NULL || email == NULL) {
    printf("** Erro ao ler o cliente armazenado (Mal formatado?)**\n");
    return(NULL);
  }
  out->cpf = in_cpf;
  out->nome = in_nome;
  out->email = in_email;

  return (out);
}

//== Funções publicas ==

/*Retorna um vetor com todos os clientes armazenados, NULL caso mal-sucedido*/
cliente **clientes_get_all() {
  FILE *arquivo;
  //Abrir o arquivo, checar por erros
  arquivo = fopen("data/clientes.txt", 'r');
  if (arquivo == NULL) {
    printf("** Erro ao abrir o arquivo clientes **\n");
    return(NULL);
  }

  //Declara o vetor apontando para null
  cliente **vetor_clientes = NULL;

  char *linha;
  //Ler a primeira linha
  fgets(linha, LINESIZE, arquivo);
  //Ler linha por linha
  int posicao;
  for (posicao = 0; linha != NULL; posicao++) {
    //Realoca o vetor para a quantidade de linhas lidas até agora
    vetor_clientes = realloc(vetor_clientes, (posicao+1) * sizeof(*cliente));
    //Formata e aloca o cliente
    cliente *cliente_atual = clientes_parse(linha)

    //Vamos verificar se é valido
    if (cliente_atual == NULL) {
      printf("** Erro ao processar o cliente na posição %d **\n", posicao);
      //Caso não seja válido, vamos simplesmente pular e continuar, tirando 1 de i para garantir que não tenha posição vazia
      i--;
    }
    else {
      //Caso seja válido, armazenar
      vetor_clientes[posicao] = cliente_atual;
    }

    //Lê a próxima linha
    fgets(linha, LINESIZE, arquivo);
  }
  //Vou adicionar um espaço e colocar NULL, para sabermos onde acaba o array
  vetor_clientes = realloc(vetor_clientes, (posicao+1) * sizeof(*cliente));
  vetor_clientes[posicao] = NULL;
  return(vetor_clientes);
}

/*Retorna um cliente, dado seu cpf. Retorna NULL caso mal-sucedido*/
cliente *clientes_get(long long int cpf) {
  //Carregar os clientes do arquivo, guardar num vetor
  cliente **vetor_clientes = clientes_get_all();

  //Iterar pelos clientes
  for (int i = 0; vetor_clientes[i] != NULL; i++) {
    //Caso seja o cpf que buscamos
    if (vetor_clientes[i]->cpf == cpf) {
      return(vetor_clientes[i]); //retornar o elemento
    }
  }
  //TODO dar free nos clientes

  printf("** Cliente não encontrado **\n");
  return(NULL);

}

/*Modifica um cliente, dado seu cpf. Cria caso não exista, idempotente. Retorna ele atualizado caso seja bem sucedido, NULL caso contrário.*/
cliente *clientes_put(long long int cpf, char *nome, char *email) {

}

/*Adiciona um novo cliente, dado seu cpf. Retorna ele caso seja bem sucedido, NULL caso contrário*/
cliente *clientes_post(long long int cpf, char *nome, char *email) {
}

/*Apaga um cliente, dado seu cpf. Retorna true caso bem sucedido, false caso contrário*/
bool clientes_del(long long int cpf) {

}
