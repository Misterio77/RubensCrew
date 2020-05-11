#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //Booleans
#include <string.h>  //Funções para strings

#include "clientes.h"

#define MAX_LINESIZE 300 //Tamanho para cada linha

//====== Estrutura (privada) ======

struct _cliente {
  long long int cpf; //Numero de 9 digitos, cpf do cliente
  char *nome; //String, nome do cliente
  char *email; //String, do formato exemplo@exemplo.com, email do cliente
};


//====== Funções básicas do TAD  ======

/*Recebe os campos, aloca e constroi uma instancia de cliente*/
cliente *cliente_criar(long long int cpf, char *nome, char *email) {
  //Checar se algum campo é NULL
  if (cpf == 0 || nome == NULL || email == NULL) {
    printf("** Campos inválidos **\n");
    return(NULL);
  }
  //Cliente a ser retornoado
  cliente *out = malloc(sizeof(cliente));
  if (out == NULL) {
    printf("** Erro ao alocar **\n");
    return(NULL);
  }
  //Campos
  out->cpf = cpf;
  out->nome = strdup(nome);
  out->email = strdup(email);

  return(out);
}

/*Dado cliente, desaloca ele e todos os seus campos*/
void cliente_destruir(cliente *in) {
  free(in->nome);
  free(in->email);
  free(in);
  return;
}

/*Recebe um cliente, exibe informação formatada sobre ele (ideal para printar)*/
char *cliente_info(cliente *in) {
  if (in == NULL) {
    printf("** Cliente inválido **\n");
    return (NULL);
  }
  char buffer[MAX_LINESIZE] = "";
  snprintf(buffer, MAX_LINESIZE, "CPF: %lld | Nome: %s | Email: %s", in->cpf, in->nome, in->email);

  //Alocar uma string do tamanho certinho e copiar
  char *out = malloc((strlen(buffer)+1)*sizeof(char));
  strcpy(out, buffer);

  return(out);
}




//====== Funções para parse e unparse (não são públicas) ======

/*Corta e formata uma linha (um cliente), aloca a estrutura e retorna*/
cliente *cliente_parse(char *in) {
  char *s = strdup(in);

  //Cortar usando o delimitador
  long long int in_cpf = strtol(strtok(s, ";"), NULL, 10);
  char *in_nome = strtok(NULL, ";");
  char *in_email = strtok(NULL, ";");

  cliente *out = cliente_criar(in_cpf, in_nome, in_email);
  free(s);

  return(out);
}

/*Recebe um cliente, e devolve a linha formatada para armazenamento ou busca*/
char *cliente_unparse(cliente *in) {
  if (in == NULL) {
    printf("** Cliente inválido **\n");
  }
  //Converter na string formatada usando um buffer temporário
  char buffer[MAX_LINESIZE] = "";
  snprintf(buffer, MAX_LINESIZE, "%lld;%s;%s", in->cpf, in->nome, in->email);
  //Alocar uma string do tamanho certinho e copiar
  char *out = malloc(strlen(buffer) + 1);
  strcpy(out, buffer);

  return(out);
}


//====== Funções para modificação da database. ======

/*Retorna um vetor com todos os clientes armazenados, NULL caso mal-sucedido*/
cliente **cliente_list() {
  FILE *arquivo;
  //Abrir o arquivo, checar por erros
  arquivo = fopen("data/clientes.txt","r");
  if (arquivo == NULL) {
    printf("** Erro ao abrir o arquivo clientes **\n");
    return(NULL);
  }
  //Declara o vetor apontando para null
  cliente **vetor_clientes = NULL;

  int posicao = 0;
  do {
    char linha[MAX_LINESIZE] = "";
    //Ler a linha, caso fgets retorne NULL, vamos sair do loop
    if (fgets(linha, MAX_LINESIZE, arquivo) == NULL) break;
    linha[strcspn(linha, "\n")] = 0; //Remover trailing newline
    //Realocar o vetor para a quantidade de linhas lidas até agora
    vetor_clientes = realloc(vetor_clientes, (posicao+1) * sizeof(cliente*));
    if (vetor_clientes == NULL) {
      printf("** Erro ao realocar **\n");
      return(NULL);
    }
    //Formata e aloca o cliente
    cliente *cliente_atual = cliente_parse(linha);
    //Vamos verificar se é valido
    if (cliente_atual == NULL) {
      printf("** Erro ao processar o cliente na posição %d **\n", posicao);
      //Caso não seja válido, não vamos incrementar posicao (evitar espaços vazios)
    }
    else {
      //Caso seja válido, armazenar
      vetor_clientes[posicao] = cliente_atual;
      posicao++;
    }
  } while(true);

  //Vou adicionar um espaço e colocar NULL, para sabermos onde acaba o array
  vetor_clientes = realloc(vetor_clientes, (posicao+1) * sizeof(cliente*));
  vetor_clientes[posicao] = NULL;

  fclose(arquivo);
  return(vetor_clientes);
}

/*Retorna um cliente, dado seu cpf. (Implementado como busca sequencial) Retorna NULL caso mal-sucedido*/
cliente *cliente_get(long long int cpf) {
  //Carregar os clientes do arquivo, guardar num vetor
  cliente **vetor_clientes = cliente_list();
  //Ponteiro para retornar
  cliente *retorno = NULL;
  //Iterar pelos clientes
  for (int i = 0; vetor_clientes[i] != NULL; i++) {
    //Caso seja o cpf que buscamos
    if (vetor_clientes[i]->cpf == cpf) {
      //Guardar o ponteiro
      retorno = vetor_clientes[i];
      for (int j = 0; vetor_clientes[j] != NULL; j++) if (j != i) cliente_destruir(vetor_clientes[j]);
      free(vetor_clientes);
      return(retorno); //retornar o elemento
    }
  }
  //Liberar a memória do vetor
  for (int j = 0; vetor_clientes[j] != NULL; j++) cliente_destruir(vetor_clientes[j]);
  free(vetor_clientes);
  printf("** Cliente não encontrado **\n");
  return(NULL);

}

/*Adiciona um novo cliente. O retorna caso seja bem sucedido, NULL caso contrário*/
cliente *cliente_post(cliente *in) {
  FILE *arquivo;
  //Abrir o arquivo, checar por erros
  arquivo = fopen("data/clientes.txt", "a");
  if (arquivo == NULL) {
    printf("** Erro ao escrever o arquivo clientes **\n");
    return(NULL);
  }

  //Transformar o cliente a ser adicionado em string formatada
  char *output = cliente_unparse(in);
  //Colocar no fim do arquivo
  fputs(output, arquivo);
  fputs("\n", arquivo);

  fclose(arquivo);

  return(in);
}

/*Apaga um cliente do banco de dadoss, dado seu cpf. Retorna 0 caso bem sucedido, -1 caso contrário.*/
int cliente_delete(long long int cpf) {
  //Abrir os arquivos e verificar por erros
  FILE *arquivo_source, *arquivo_target;
  arquivo_source = fopen("data/clientes.txt", "r");
  if (arquivo_source == NULL) {
    printf("** Erro ao ler o arquivo clientes **\n");
    return(-1);
  }
  arquivo_target = fopen("data/clientes.tmp", "w");
  if (arquivo_source == NULL) {
    printf("** Erro ao criar um novo arquivo clientes **\n");
    fclose(arquivo_source);
    return(-1);
  }
  //Definir linha a ser buscada
  cliente *buscado = cliente_get(cpf);
  if (buscado == NULL) {
    fclose(arquivo_source);
    fclose(arquivo_target);
    return (-1);
  }
  char *buscado_unparsed = cliente_unparse(buscado);
  cliente_destruir(buscado);

  do {
    char linha[MAX_LINESIZE];
    if (fgets(linha, MAX_LINESIZE, arquivo_source) == NULL) break;
    linha[strcspn(linha, "\n")] = 0; //Remover trailing newline
    if (strcmp(linha, buscado_unparsed) != 0) {
      fputs(linha, arquivo_target);
      fputs("\n", arquivo_target);
    }
  } while(true);

  free(buscado_unparsed);
  fclose(arquivo_source);
  fclose(arquivo_target);

  //TODO Handle erros que n sao da busca, e sim de escrever o arquivo
  remove("data/clientes.txt");
  rename("data/clientes.tmp", "data/clientes.txt");

  return(0);
}
