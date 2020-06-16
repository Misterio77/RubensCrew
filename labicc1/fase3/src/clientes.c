/* Trabalho Lab ICC1 - Sistema de mercado
 *
 * 10856803 - Gabriel Silva Fontes
 * 11912533 - Samuel Rubens
 * 11816139 - Julio Igor Casemiro Oliveira
 * xxxxxxxx - João Pedro
 * xxxxxxxx - Augusto
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //Funções para strings

#include "clientes.h"

#define MAX_LINESIZE 300 //Tamanho para cada linha

//Cores para output
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

//====== Estrutura (privada) ======

struct _cliente {
  long long int cpf; //Numero de 9 digitos, cpf do cliente
  char *nome; //String, nome do cliente
  char *email; //String, do formato exemplo@exemplo.com, email do cliente
};


//====== Funções básicas do TAD  ======

/*Recebe os campos, aloca e constroi uma instancia de cliente*/
cliente *cliente_criar(long long int cpf, char *nome, char *email, int silent) {
  //Checar se algum campo é NULL
  if (cpf == 0 || nome == NULL || email == NULL) {
    if (!silent) printf(RED"** Campos inválidos fornecidos **\n"RESET);
    return(NULL);
  }
  //Cliente a ser retornoado
  cliente *out = malloc(sizeof(cliente));
  if (out == NULL) {
    if (!silent) printf(RED"** Erro ao alocar **\n"RESET);
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
  if(in != NULL) {
      free(in->nome);
      free(in->email);
      free(in);
  }
}

/*Recebe um cliente, devolve informação formatada sobre ele*/
char *cliente_info(cliente *in, int silent) {
  if (in == NULL) {
    if (!silent) printf(RED"** Cliente inválido **\n"RESET);
    return (NULL);
  }
  char buffer[MAX_LINESIZE] = "";
  snprintf(buffer, MAX_LINESIZE, "CPF: %lld | Nome: %s | Email: %s", in->cpf, in->nome, in->email);
  char *out = strdup(buffer);
  return(out);
}




//====== Funções para parse e unparse (não são públicas) ======

/*Corta e formata uma linha (um cliente), aloca a estrutura e retorna*/
cliente *cliente_parse(char *in, int silent) {
  char *s = strdup(in);

  //Cortar usando o delimitador
  char *in_cpf   = strtok(s, ";");
  char *in_nome  = strtok(NULL, ";");
  char *in_email = strtok(NULL, ";");

  //Verificar se foi possível ler todos
  if (in_cpf == NULL || in_nome == NULL || in_email == NULL) {
    free(s);
    return(NULL);
  }

  cliente *out = cliente_criar(
    (long long int) strtol(in_cpf, NULL, 10),
    in_nome,
    in_email,
    silent
  );

  free(s);
  return(out);
}

/*Recebe um cliente, e devolve a linha formatada para armazenamento ou busca*/
char *cliente_unparse(cliente *in, int silent) {
  if (in == NULL) {
    if (!silent) printf(RED"** Cliente inválido **\n"RESET);
    return(NULL);
  }
  //Converter na string formatada usando um buffer temporário
  char buffer[MAX_LINESIZE] = "";
  snprintf(buffer, MAX_LINESIZE, "%lld;%s;%s", in->cpf, in->nome, in->email);
  //Alocar uma string do tamanho certinho e copiar
  char *out = strdup(buffer);

  return(out);
}


//====== Funções para modificação da database. ======

/*Retorna um vetor com todos os clientes armazenados, NULL caso mal-sucedido*/
cliente **cliente_list(char *database, int silent) {
  FILE *arquivo;
  //Abrir o arquivo, checar por erros
  arquivo = fopen(database,"r");
  if (arquivo == NULL) {
    if (!silent) printf(RED"** Erro ao abrir o arquivo clientes **\n"RESET);
    return(NULL);
  }
  //Declara o vetor apontando para null
  cliente **vetor_clientes = NULL;

  int posicao_vetor = 0;
  int posicao_arquivo = 0;
  do {
    posicao_arquivo++;
    char linha[MAX_LINESIZE] = "";

    //Ler a linha, caso fgets retorne NULL, vamos sair do loop
    if (fgets(linha, MAX_LINESIZE, arquivo) == NULL) break;
    linha[strcspn(linha, "\n")] = 0; //Remover trailing newline

    //Realocar o vetor para a quantidade de linhas lidas até agora
    vetor_clientes = realloc(vetor_clientes, (posicao_vetor+1) * sizeof(cliente*));
    if (vetor_clientes == NULL) {
      if (!silent) printf(RED"** Erro ao realocar **\n"RESET);
      return(NULL);
    }

    //Formata e aloca o cliente
    cliente *cliente_atual = cliente_parse(linha, silent);

    //Vamos verificar se é valido
    if (cliente_atual == NULL) {
      if (!silent) printf(RED"** Erro ao processar o cliente na linha %d, pulando **\n"RESET, posicao_arquivo);
      //Caso não seja válido, não vamos incrementar posicao_vetor (evitar espaços vazios)
    }
    else {
      //Caso seja válido, armazenar, incrementar posicao_vetor
      vetor_clientes[posicao_vetor] = cliente_atual;
      posicao_vetor++;
    }
  } while(1);

  //Vou adicionar um espaço e colocar NULL, para sabermos onde acaba o array
  vetor_clientes = realloc(vetor_clientes, (posicao_vetor+1) * sizeof(cliente*));
  vetor_clientes[posicao_vetor] = NULL;

  fclose(arquivo);
  return(vetor_clientes);
}

/*Retorna um cliente, dado seu cpf. (Implementado como busca sequencial) Retorna NULL caso mal-sucedido*/
cliente *cliente_get(char *database, long long int cpf, int silent) {
  //Carregar os clientes do arquivo, guardar num vetor
  cliente **vetor_clientes = cliente_list(database, silent);
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
  if (!silent) printf(RED"** Cliente não encontrado **\n"RESET);
  return(NULL);

}

/*Adiciona um novo cliente. Retorna 0 caso seja bem sucedido, -1 caso contrário*/
int cliente_post(char *database, cliente *in, int silent) {
  FILE *arquivo;

  //Transformar o cliente a ser adicionado em string formatada
  char *output = cliente_unparse(in, silent);
  if (output == NULL) {
    return (-1);
  }

  //Verificar se existe
  if (cliente_get(database, in->cpf, 1) != NULL) {
    if (!silent) printf(RED"** Cliente já existe, substituindo **\n"RESET);
    //Apagar todos, ate nao encontrar mais
    while(cliente_delete(database, in->cpf, 1) != -1);
  }

  //Abrir o arquivo, checar por erros
  arquivo = fopen(database, "a");
  if (arquivo == NULL) {
    if (!silent) printf(RED"** Erro ao escrever o arquivo clientes **\n"RESET);
    return(-1);
  }

  //Colocar no fim do arquivo
  fputs(output, arquivo);
  fputs("\n", arquivo);
  
  free(output);
  fclose(arquivo);

  return(0);
}

/*Apaga um cliente do banco de dadoss, dado seu cpf. Retorna 0 caso bem sucedido, -1 caso contrário.*/
int cliente_delete(char *database, long long int cpf, int silent) {
  FILE *arquivo_source, *arquivo_target;
  //Abrir arquivo inicial
  arquivo_source = fopen(database, "r");
  if (arquivo_source == NULL) {
    if (!silent) printf(RED"** Erro ao ler o arquivo clientes **\n"RESET);
    return(-1);
  }
  //Buscar cliente especificado
  cliente *buscado = cliente_get(database, cpf, silent);
  if (buscado == NULL) {
    fclose(arquivo_source);
    return (-1);
  }
  //Abrir arquivo destino
  arquivo_target = fopen("clientes.tmp", "w");
  if (arquivo_source == NULL) {
    if (!silent) printf(RED"** Erro ao criar um novo arquivo clientes **\n"RESET);
    fclose(arquivo_source);
    return(-1);
  }
  //Transformar em string
  char *buscado_unparsed = cliente_unparse(buscado, silent);
  //Desalocar estrutura
  cliente_destruir(buscado);

  //Copiar arquivo, exceto linha a ser apagada
  do {
    //buffer
    char linha[MAX_LINESIZE];
    //Pegar linha do arquivo source
    if (fgets(linha, MAX_LINESIZE, arquivo_source) == NULL) break;
    //Remover trailing newline
    linha[strcspn(linha, "\n")] = 0;
    //Vamos parsear e unparsear a linha. Assim evitamos um problema de comparação quando há duas formas de escrever no arquivo o mesmo campo (ex: decimal e espaços)
    char *linha_corrigida = cliente_unparse(cliente_parse(linha, silent), silent);
    //Somente copiar se NÃO for a linha que buscamos
    if (strcmp(linha_corrigida, buscado_unparsed) != 0) {
      fputs(linha_corrigida, arquivo_target);
      fputs("\n", arquivo_target);
    }
  } while(1);

  free(buscado_unparsed);
  fclose(arquivo_source);
  fclose(arquivo_target);

  remove(database);
  rename("clientes.tmp", database);

  return(0);
}
