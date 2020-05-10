#include <stdio.h>

void clientes();
void produtos();
void estoque();

int main(){
    int opcao, op;
    printf("Bem vindo ao sistema de gerencia do Mercadinho Big Bom");
    printf("UMA EXPLOSAO DE OFERTAS!!!");
    while(1){
        printf("Digite 1 para cadastro de cliente\nDigite 2 para cadastro de produto\nDigite 3 para controle de estoque");
        scanf("%d", &opcao);
        if(opcao == 1)
            clientes();
        else if(opcao == 2)
            produtos();
        else if(opcao == 3)
            estoque();
        else{
            printf("Deseja fazer mais alguma operação? (Digite s/n)");
            scanf("%c", &op);
            if(op == 'n')
                return 0;
        }
    }
}

void clientes(){
    int cpf, opcao;
    char nome[100], email[50];

    printf("--Cadastro de Clientes--");
    printf("Digite o nome do cliente");
    scanf("%s", &nome);
    while(1){
        printf("Digite o CPF do cliente");
        scanf("%s", &cpf);
        if(/*CPF já cadastrado na base de dados*/){
            printf("CPF ja cadastrado, tentar outro CPF? (Digite s/n)");
            scanf("%d", &opcao);
            if(opcao == 'n')
                return;
        }
        else
            break;
    }
    while(1){
        printf("Digite o email do cliente (ate o @)");
        scanf("%s", &email);
        if(/*Email já cadastrado na base de dados*/){
            printf("Email ja cadastrado, tentar outro email? (Digite s/n)");
            scanf("%c", &opcao);
            if(opcao == 'n')
                return;
        }
        else
            break;
    }
    //Salvar dados na base de clientes
}

void produtos(){
    int quantidade, codigo;
    char nome[50], opcao;
    float valor;

    printf("--Cadastro de Produtos--");
    printf("Digite o nome do produto");
    scanf("%s", &nome);
    if(/*Nome já existe na base de dados*/){
        printf("Produto ja cadastrado, deseja acessar o estoque? (Digite s/n)");
        scanf("%c", &opcao);
        if(opcao == 's')
            estoque();
        return;
    }
    printf("Digite o valor do produto");
    scanf("%f", &valor);
    printf("Digite a quantidade do novo produto");
    scanf("%d", &quantidade);
    printf("Digite o codigo de barras do produto");
    scanf("%d", &codigo);
    if(/*Código de barras já cadastrado na base de dados*/){
        printf("Codigo ja cadastrado, deseja acessar o estoque? (Digite s/n)");
        scanf("%c", opcao);
        if(opcao == 's')
            estoque();
        return;
    }
    else
        //Salvar dados na base de produtos
    return;
}

void estoque(){
    int opcao, codigo, quantia;
    char op;

    printf("--Controle de Estoque--");
    printf("Digite 1 para adicionar ao estoque\nDigite 2 para remover do estoque");
    scanf("%d", &opcao);
    while(1){
        printf("Digite o codigo de barras do produto");
        scanf("%d", &codigo);
        if(/*Produto não existe na base de dados*/){
            printf("Produto nao existente, deseja colocar outro codigo? (Digite s/n)")
            scanf("%c", &op);
            if(op == 'n')
                return;
        }
        else
            break;
    }
    if(opcao == 1){
        printf("Digite a quantia a ser adicionada ao estoque");
        scanf("%d", &quantia);
        //Salvar na base de produtos
        return;
    }
    else{
        printf("Digite q quantia a ser removida do estoque");
        while(1){
            scanf("%d", &quantia);
            if(/*Quantidade digitada maior que a disponivel no estoque desse produto*/)
                printf("A quantidade no estoque e %d, quanto deseja remover?", quantia)  //Quantia a ser mostrada tem que ser tirada da base
            break;
        }
        //Salvar na base de produtos
        return;
    }
}
