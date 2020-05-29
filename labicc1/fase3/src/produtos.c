#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> //Booleans
#include <string.h>  //Funções para strings

#include "produtos.h"

#define MAX_LINESIZE 300 //Tamanho para cada linha

//====== Estrutura (privada) ======

struct _produto {
  int id; //Id numerico do produto
  char *nome; //Nome do produto
  float valor; //Valor unitario do produto
  int estoque; //Estoque atual do produto
};


//====== Funções básicas do TAD  ======

/*Recebe os campos, aloca e constroi uma instancia de produto*/
produto *produto_criar(int id, char *nome, float valor, int estoque) {
  //Checar se algum campo é inválido
  if (id == 0 || nome == NULL) {
    printf("** Campos inválidos **\n");
    return(NULL);
  }
  //Produto a ser retornoado
  produto *out = malloc(sizeof(produto));
  if (out == NULL) {
    printf("** Erro ao alocar **\n");
    return(NULL);
  }
  //Campos
  out->id = id;
  out->nome = strdup(nome);
  out->valor = valor;
  out->estoque = estoque;

  return(out);
}

/*Dado produto, desaloca ele e todos os seus campos*/
void produto_destruir(produto *in) {
  free(in->nome);
  free(in);
  return;
}

/*Recebe um produto, retorna informação formatada sobre ele*/
char *produto_info(produto *in) {
  if (in == NULL) {
    printf("** Produto inválido **\n");
    return (NULL);
  }
  
  char buffer[MAX_LINESIZE] = "";
  snprintf(buffer, MAX_LINESIZE, "ID: %d | Nome: %s | Valor: %f | Estoque: %d", in->id, in->nome, in->valor, in->estoque);
  char *out = strdup(buffer);
  return(out);
}




//====== Funções para parse e unparse (não são públicas) ======

/*Corta e formata uma linha (um produto), aloca a estrutura e retorna*/
produto *produto_parse(char *in) {
  char *s = strdup(in);

  //Cortar usando o delimitador
  char *in_id      = strtok(s, ";");
  char *in_nome    = strtok(NULL, ";");
  char *in_valor   = strtok(NULL, ";");
  char *in_estoque = strtok(NULL, ";");
  
  //Verificar se foi possível ler todos
  if (in_id == NULL || in_nome == NULL || in_valor == NULL || in_estoque == NULL) {
    free(s);
    return(NULL);
  }

  produto *out = produto_criar(
    (int) strtol(in_id, NULL, 10),
    in_nome,
    (float) strtod(in_valor, NULL),
    (int) strtol(in_estoque, NULL, 10)
  );

  free(s);
  return(out);
}

/*Recebe um produto, e devolve a linha formatada para armazenamento ou busca*/
char *produto_unparse(produto *in) {
  if (in == NULL) {
    printf("** Produto inválido **\n");
  }
  //Converter na string formatada usando um buffer temporário
  char buffer[MAX_LINESIZE] = "";
  snprintf(buffer, MAX_LINESIZE, "%d;%s;%f;%d", in->id, in->nome, in->valor, in->estoque);
  //Alocar uma string do tamanho certinho e copiar
  char *out = strdup(buffer);

  return(out);
}


//====== Funções para modificação da database. ======

/*Retorna um vetor com todos os produtos armazenados, NULL caso mal-sucedido*/
produto **produto_list() {
  FILE *arquivo;
  //Abrir o arquivo, checar por erros
  arquivo = fopen("data/produtos.txt","r");
  if (arquivo == NULL) {
    printf("** Erro ao abrir o arquivo produtos **\n");
    return(NULL);
  }
  //Declara o vetor apontando para null
  produto **vetor_produtos = NULL;

  int posicao = 0;
  do {
    char linha[MAX_LINESIZE] = "";
    //Ler a linha, caso fgets retorne NULL, vamos sair do loop
    if (fgets(linha, MAX_LINESIZE, arquivo) == NULL) break;
    linha[strcspn(linha, "\n")] = 0; //Remover trailing newline
    //Realocar o vetor para a quantidade de linhas lidas até agora
    vetor_produtos = realloc(vetor_produtos, (posicao+1) * sizeof(produto*));
    if (vetor_produtos == NULL) {
      printf("** Erro ao realocar **\n");
      return(NULL);
    }
    //Formata e aloca o produto
    produto *produto_atual = produto_parse(linha);
    //Vamos verificar se é valido
    if (produto_atual == NULL) {
      printf("** Erro ao processar o produto na posição %d **\n", posicao);
      //Caso não seja válido, não vamos incrementar posicao (evitar espaços vazios)
    }
    else {
      //Caso seja válido, armazenar
      vetor_produtos[posicao] = produto_atual;
      posicao++;
    }
  } while(true);

  //Vou adicionar um espaço e colocar NULL, para sabermos onde acaba o array
  vetor_produtos = realloc(vetor_produtos, (posicao+1) * sizeof(produto*));
  vetor_produtos[posicao] = NULL;

  fclose(arquivo);
  return(vetor_produtos);
}

/*Retorna um produto, dado seu id. (Implementado como busca sequencial) Retorna NULL caso mal-sucedido*/
produto *produto_get(int id) {
  //Carregar os produtos do arquivo, guardar num vetor
  produto **vetor_produtos = produto_list();
  //Ponteiro para retornar
  produto *retorno = NULL;
  //Iterar pelos produtos
  for (int i = 0; vetor_produtos[i] != NULL; i++) {
    //Caso seja o id que buscamos
    if (vetor_produtos[i]->id == id) {
      //Guardar o ponteiro
      retorno = vetor_produtos[i];
      for (int j = 0; vetor_produtos[j] != NULL; j++) if (j != i) produto_destruir(vetor_produtos[j]);
      free(vetor_produtos);
      return(retorno); //retornar o elemento
    }
  }
  //Liberar a memória do vetor
  for (int j = 0; vetor_produtos[j] != NULL; j++) produto_destruir(vetor_produtos[j]);
  free(vetor_produtos);
  printf("** Produto não encontrado **\n");
  return(NULL);

}

/*Adiciona um novo produto. O retorna caso seja bem sucedido, NULL caso contrário*/
produto *produto_post(produto *in) {
  FILE *arquivo;
  //Abrir o arquivo, checar por erros
  arquivo = fopen("data/produtos.txt", "a");
  if (arquivo == NULL) {
    printf("** Erro ao escrever o arquivo produtos **\n");
    return(NULL);
  }

  //Transformar o produto a ser adicionado em string formatada
  char *output = produto_unparse(in);
  //Colocar no fim do arquivo
  fputs(output, arquivo);
  fputs("\n", arquivo);

  fclose(arquivo);

  return(in);
}

/*Apaga um produto do banco de dados, dado seu id. Retorna 0 caso bem sucedido, -1 caso contrário.*/
int produto_delete(int id) {
  //Abrir os arquivos e verificar por erros
  FILE *arquivo_source, *arquivo_target;
  arquivo_source = fopen("data/produtos.txt", "r");
  if (arquivo_source == NULL) {
    printf("** Erro ao ler o arquivo produtos **\n");
    return(-1);
  }
  arquivo_target = fopen("data/produtos.tmp", "w");
  if (arquivo_source == NULL) {
    printf("** Erro ao criar um novo arquivo produtos **\n");
    fclose(arquivo_source);
    return(-1);
  }
  //Definir linha a ser buscada
  produto *buscado = produto_get(id);
  if (buscado == NULL) {
    fclose(arquivo_source);
    fclose(arquivo_target);
    return (-1);
  }
  char *buscado_unparsed = produto_unparse(buscado);
  produto_destruir(buscado);

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
  remove("data/produtos.txt");
  rename("data/produtos.tmp", "data/produtos.txt");

  return(0);
}
