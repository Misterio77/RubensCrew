/* Trabalho Lab ICC1 - Sistema de mercado
 *
 * 10856803 - Gabriel Silva Fontes
 * 11912533 - Samuel Rubens
 * 11816139 - Julio Igor Casemiro Oliveira
 * 11832343 - João Pedro
 * xxxxxxxx - Augusto
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>  //Funções para strings

#include "produtos.h"

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

struct _produto {
  int id; //Id numerico do produto
  char *nome; //Nome do produto
  float valor; //Valor unitario do produto
  int estoque; //Estoque atual do produto
};


//====== Funções básicas do TAD  ======

/*Recebe os campos, aloca e constroi uma instancia de produto*/
produto *produto_criar(int id, char *nome, float valor, int estoque, int silent) {
  //Checar se algum campo é inválido
  if (id == 0 || nome == NULL || valor == 0) {
    if (!silent) printf(RED"** Campos inválidos fornecidos **\n"RESET);
    return(NULL);
  }
  //Produto a ser retornoado
  produto *out = malloc(sizeof(produto));
  if (out == NULL) {
    if (!silent) printf(RED"** Erro ao alocar **\n"RESET);
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
  if (in != NULL) {
    free(in->nome);
    free(in);
  }
}

/*Recebe um produto, retorna informação formatada sobre ele*/
char *produto_info(produto *in, int silent) {
  if (in == NULL) {
    if (!silent) printf(RED"** Produto inválido **\n"RESET);
    return (NULL);
  }
  
  char buffer[MAX_LINESIZE] = "";
  snprintf(buffer, MAX_LINESIZE, "ID: %d | Nome: %s | Valor: %.2f | Estoque: %d", in->id, in->nome, in->valor, in->estoque);
  char *out = strdup(buffer);
  return(out);
}




//====== Funções para parse e unparse (não são públicas) ======

/*Corta e formata uma linha (um produto), aloca a estrutura e retorna*/
produto *produto_parse(char *in, int silent) {
  char *s = strdup(in);

  //Cortar usando o delimitador
  char *in_id      = strtok(s, ";");
  char *in_nome    = strtok(NULL, ";");
  char *in_valor   = strtok(NULL, ";");
  char *in_estoque = strtok(NULL, ";");
  
  //Verificar se foi possível ler todos
  if (in_id == NULL || in_nome == NULL || in_valor == NULL || in_estoque == NULL) {
    if (in_id != NULL) free(in_id);
    if (in_nome != NULL) free(in_nome);
    if (in_valor != NULL) free(in_valor);
    if (in_estoque != NULL) free(in_estoque);
    free(s);
    return(NULL);
  }

  produto *out = produto_criar(
    (int) strtol(in_id, NULL, 10),
    in_nome,
    (float) strtod(in_valor, NULL),
    (int) strtol(in_estoque, NULL, 10),
    silent
  );

  free(s);
  return(out);
}

/*Recebe um produto, e devolve a linha formatada para armazenamento ou busca*/
char *produto_unparse(produto *in, int silent) {
  if (in == NULL) {
    if (!silent) printf(RED"** Produto inválido **\n"RESET);
    return(NULL);
  }
  //Converter na string formatada usando um buffer temporário
  char buffer[MAX_LINESIZE] = "";
  snprintf(buffer, MAX_LINESIZE, "%d;%s;%.2f;%d", in->id, in->nome, in->valor, in->estoque);
  //Alocar uma string do tamanho certinho e copiar
  char *out = strdup(buffer);

  return(out);
}


//====== Funções para modificação da database. ======

/*Retorna um vetor com todos os produtos armazenados, NULL caso mal-sucedido*/
produto **produto_list(char *database, int silent) {
  FILE *arquivo;
  //Abrir o arquivo, checar por erros
  arquivo = fopen(database,"r");
  if (arquivo == NULL) {
    if (!silent) printf(RED"** Erro ao abrir o arquivo produtos **\n"RESET);
    return(NULL);
  }
  //Declara o vetor apontando para null
  produto **vetor_produtos = NULL;

  int posicao_vetor = 0;
  int posicao_arquivo = 0;
  do {
    posicao_arquivo++;
    char linha[MAX_LINESIZE] = "";

    //Ler a linha, caso fgets retorne NULL, vamos sair do loop
    if (fgets(linha, MAX_LINESIZE, arquivo) == NULL) break;
    linha[strcspn(linha, "\n")] = 0; //Remover trailing newline

    //Realocar o vetor para a quantidade de linhas lidas até agora
    vetor_produtos = realloc(vetor_produtos, (posicao_vetor+1) * sizeof(produto*));
    if (vetor_produtos == NULL) {
      if (!silent) printf(RED"** Erro ao realocar **\n"RESET);
      return(NULL);
    }

    //Formata e aloca o produto
    produto *produto_atual = produto_parse(linha, silent);

    //Vamos verificar se é valido
    if (produto_atual == NULL) {
      if (!silent) printf(RED"** Erro ao processar o produto na posição %d, pulando **\n"RESET, posicao_arquivo);
      //Caso não seja válido, não vamos incrementar posicao_vetor (evitar espaços vazios)
    }
    else {
      //Caso seja válido, armazenar
      vetor_produtos[posicao_vetor] = produto_atual;
      posicao_vetor++;
    }
  } while(1);

  //Vou adicionar um espaço e colocar NULL, para sabermos onde acaba o array
  vetor_produtos = realloc(vetor_produtos, (posicao_vetor+1) * sizeof(produto*));
  vetor_produtos[posicao_vetor] = NULL;

  fclose(arquivo);
  return(vetor_produtos);
}

/*Retorna um produto, dado seu id. (Implementado como busca sequencial) Retorna NULL caso mal-sucedido*/
produto *produto_get(char *database, int id, int silent) {
  //Carregar os produtos do arquivo, guardar num vetor
  produto **vetor_produtos = produto_list(database, silent);
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
  if (!silent) printf(RED"** Produto não encontrado **\n"RESET);
  return(NULL);

}

/*Adiciona um novo produto. Retorna 0 caso seja bem sucedido, -1 caso contrário*/
int produto_post(char *database, produto *in, int silent) {
  FILE *arquivo;

  //Transformar o produto a ser adicionado em string formatada
  char *output = produto_unparse(in, silent);
  if (output == NULL) {
      return (-1);
  }

  //Verificar se existe
  if (produto_get(database, in->id, 1) != NULL) {
    if (!silent) printf(RED"** Produto já existe, substituindo **\n"RESET);
    //Apagar todos, ate nao encontrar mais
    while(produto_delete(database, in->id, 1) != -1);
  }

  //Abrir o arquivo, checar por erros
  arquivo = fopen(database, "a");
  if (arquivo == NULL) {
    if (!silent) printf(RED"** Erro ao escrever o arquivo produtos **\n"RESET);
    return(-1);
  }

  //Colocar no fim do arquivo
  fputs(output, arquivo);
  fputs("\n", arquivo);

  free(output);
  fclose(arquivo);

  return(0);
}

/*Apaga um produto do banco de dados, dado seu id. Retorna 0 caso bem sucedido, -1 caso contrário.*/
int produto_delete(char *database, int id, int silent) {
  FILE *arquivo_source, *arquivo_target;
  //Abrir arquivo inicial
  arquivo_source = fopen(database, "r");
  if (arquivo_source == NULL) {
    if (!silent) printf(RED"** Erro ao ler o arquivo produtos **\n"RESET);
    return(-1);
  }
  //Definir linha a ser buscada
  produto *buscado = produto_get(database, id, silent);
  if (buscado == NULL) {
    fclose(arquivo_source);
    return (-1);
  }
  //Abrir arquivo destino
  arquivo_target = fopen("produtos.tmp", "w");
  if (arquivo_source == NULL) {
    if (!silent) printf(RED"** Erro ao criar um novo arquivo produtos **\n"RESET);
    fclose(arquivo_source);
    return(-1);
  }
  //Transformar em string
  char *buscado_unparsed = produto_unparse(buscado, silent);
  //Desalocar estrutura
  produto_destruir(buscado);

  //Copiar arquivo, exceto linha a ser apagada
  do {
    //buffer
    char linha[MAX_LINESIZE];
    //Pegar linha do arquivo source
    if (fgets(linha, MAX_LINESIZE, arquivo_source) == NULL) break;
    //Remover trailing newline
    linha[strcspn(linha, "\n")] = 0;
    //Vamos parsear e unparsear a linha. Assim evitamos um problema de comparação quando há duas formas de escrever no arquivo o mesmo campo (ex: decimal e espaços)
    char *linha_corrigida = produto_unparse(produto_parse(linha, silent), silent);
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
  rename("produtos.tmp", database);

  return(0);
}

/*
Altera o estoque de um produto
*/
int produto_alterar_estoque(char *database, int id, int mudanca, int silent) {
  //Buscar o produto, verificar se foi encontrado
  produto *alvo = produto_get(database, id, silent);
  if (alvo == NULL) {
    return(-1);
  }

  //Modificar estoque
  alvo->estoque += mudanca;
  if (alvo->estoque < 0) {
    if (!silent) printf(RED"** O estoque não pode ficar negativo **\n"RESET);
    produto_destruir(alvo);
    return(-1);
  }
  
  //Apagar o produto da database
  if (produto_delete(database, id, silent) != 0) {
    produto_destruir(alvo);
    return(-1);
  }

  //Fazer mudança
  if (produto_post(database, alvo, silent) != 0) {
    produto_destruir(alvo);
    return(-1);
  }

  produto_destruir(alvo);
  return(0);
}
