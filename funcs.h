#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100
#define TAM_CPF 50
#define MAX_VENDAS 1000

struct Produtos
{
    int cod_produto;
    char nome[MAX];
    float preco;
    int qtd_estoque;
};

struct Item_Vendido
{
    int cod_produto;
    float preco_venda;
    int qtd_Vendida;
    Item_Vendido *prox;
};

struct Horario_Venda
{
    int hora;
    int minuto;
    int segundo;
};

struct Vendas
{
    int data;
    Horario_Venda horaVenda;
    char cpf_cliente[TAM_CPF];
    Item_Vendido *primeiro_item;
    float total_venda;
};


Produtos* Busca_Produto_por_Cod(int cod);

void leitura_aquivo(char *nome_arquvivo);
void menu();
void adicionaItem(Vendas *vendas, int codproduto, float preco, int qtd);
void Data_HoraAtual(int* data, Horario_Venda*horario);
void CadastrarVenda();
void listarVendasPorData();
void limpar_buffer_stdin();