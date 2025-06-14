#include "funcs.h"

// Variaveis globais
int num_vendas_realizadas = 0;
int qtd_produtos = 0;

Produtos produtos[MAX];
Vendas lista_vendas[MAX];

void Leitura_aquivo(char *nome_arquvivo)
{
    FILE *arquivo;

    arquivo = fopen(nome_arquvivo, "r");

    if (arquivo == NULL)
    {
        printf("Erro nome do arquivo não digitado.\n");
    }
    else
    {
        fscanf(arquivo, "%d", &qtd_produtos);
        for (int i = 0; i < qtd_produtos; i++)
        {
            if (fscanf(arquivo, "%d", &produtos[i].cod_produto) != 1)
                break;
            if (fscanf(arquivo, " %[^\n]", produtos[i].nome) != 1)
                break;
            if (fscanf(arquivo, "%f", &produtos[i].preco) != 1)
                break;
            if (fscanf(arquivo, "%d", &produtos[i].qtd_estoque) != 1)
                break;
            fscanf(arquivo, "%*[^\n]");
        }
    }
    fclose(arquivo);
    printf("Arquivo lido com sucesso\n");
}

void Menu()
{
    printf("Menu principal \n");
    printf("[1] Cadastrar Venda\n");
    printf("[2] Listar vendas por data\n");
    printf("[3] Alterar estoque e preco de produto\n");
    printf("[4] Remover produto do estoque\n");
    printf("[5] Sair\n");
    printf("Digite uma Opcao: ");
}

void AdicionaItem(Vendas *vendas, int codproduto, float preco, int qtd)
{
    Item_Vendido *novoItem = (Item_Vendido *)malloc(sizeof(Item_Vendido));

    if (novoItem == NULL)
    {
        printf("Erro ao alocar memoria\n");
        return;
    }

    novoItem->cod_produto = codproduto;
    novoItem->preco_venda = preco;
    novoItem->qtd_Vendida = qtd;
    novoItem->prox = NULL;

    if (vendas->primeiro_item == NULL)
    {
        vendas->primeiro_item = novoItem;
    }
    else
    {
        Item_Vendido *atual = vendas->primeiro_item;
        while (atual->prox != NULL)
        {
            atual = atual->prox;
        }
        atual->prox = novoItem;
    }
    vendas->total_venda += (preco * qtd);
}

Produtos *Busca_Produto_por_Cod(int cod)
{
    for (int i = 0; i < qtd_produtos; i++)
    {
        if (produtos[i].cod_produto == cod)
        {
            return &produtos[i];
        }
    }
    return NULL;
}

void Data_HoraAtual(int *data, Horario_Venda *horario)
{
    time_t t = time(NULL);
    tm tm_local = *localtime(&t);

    *data = (tm_local.tm_year + 1900) * 10000 + (tm_local.tm_mon + 1) * 100 + tm_local.tm_mday;

    printf("[DEBUG] Data convertida para inteiro: %d\n\n", *data);

    horario->hora = tm_local.tm_hour;
    horario->minuto = tm_local.tm_min;
    horario->segundo = tm_local.tm_sec;
}

void CadastrarVenda()
{
    printf("\n---- Cadastro de Vendas ----\n");

    Vendas Nova_venda;
    Nova_venda.primeiro_item = NULL;
    Nova_venda.total_venda = 0.0;
    Data_HoraAtual(&Nova_venda.data, &Nova_venda.horaVenda);
    char continuar = 's';
    int cod_produto;
    int qtd_desejada;
    Produtos *produtosEscolhidos;

    printf("Informe o seu CPF(sem ponto e sem traço): ");
    if (fgets(Nova_venda.cpf_cliente, TAM_CPF, stdin) == NULL)
    {
        printf("Por favor, Digite o CPF novamente: ");
        if (fgets(Nova_venda.cpf_cliente, TAM_CPF, stdin) == NULL)
        {
            printf("Erro ao ler CPF. Operação Cancelada.\n");
            return;
        }
    }

    Nova_venda.cpf_cliente[strcspn(Nova_venda.cpf_cliente, "\n")] = '\0';

    if (strlen(Nova_venda.cpf_cliente) == 0)
    {
        printf("CPF não informado. Operação cancelada. \n");
        return;
    }

    while (continuar == 's' || continuar == 'S')
    {
        for (int i = 0; i < qtd_produtos; i++)
        {
            if (produtos[i].qtd_estoque > 0)
            {
                printf(" Codigo: %d | Produto: %s |R$%.2f \n",
                       produtos[i].cod_produto,
                       produtos[i].nome,
                       produtos[i].preco);
            }
        }
        printf("======================================= \n");

        printf("Digite o codigo do produto desejado: ");
        scanf("%d", &cod_produto);

        produtosEscolhidos = Busca_Produto_por_Cod(cod_produto);

        if (produtosEscolhidos == NULL)
        {
            printf("Produtos com o codigo %d nao encontrado \n", cod_produto);
        }
        else if (produtosEscolhidos->qtd_estoque == 0)
        {
            printf("Produto %s sem estoque no momento. \n", produtosEscolhidos->nome);
        }
        else
        {
            printf("Produto : %s | Estoque: %d | Preco: R$%2.f \n",
                   produtosEscolhidos->nome, produtosEscolhidos->qtd_estoque,
                   produtosEscolhidos->preco);

            printf("Digite a quantidade desejada: ");
            scanf("%d", &qtd_desejada);

            if (qtd_desejada <= 0)
            {
                printf("Quantidade invalida \n");
            }

            if (qtd_desejada > produtosEscolhidos->qtd_estoque)
            {
                printf("Quantidade desejada maior que a quantidade no estoque %d.\n",
                       produtosEscolhidos->qtd_estoque);

                char escolha_estoqueExistente;
                printf("Deseja comprar apenas a quantidade que existe no estoque? (s/n) : ");
                scanf(" %c", &escolha_estoqueExistente);

                if (escolha_estoqueExistente == 's' || escolha_estoqueExistente == 'S')
                {
                    qtd_desejada = produtosEscolhidos->qtd_estoque;
                }
                else
                {
                    printf("Item nao adicionado ao carrinho\n");
                }
            }

            AdicionaItem(&Nova_venda, produtosEscolhidos->cod_produto, produtosEscolhidos->preco, qtd_desejada);
            produtosEscolhidos->qtd_estoque -= qtd_desejada; // atualiza o estoque
            printf("Item %s %d adicionado ao carrinho.\n", produtosEscolhidos->nome, qtd_desejada);
        }
        printf("Deseja continuar adicionando produtos? (s/n): ");
        scanf(" %c", &continuar);
    }

    if (Nova_venda.primeiro_item != NULL)
    {
        if (num_vendas_realizadas < MAX_VENDAS)
        {
            lista_vendas[num_vendas_realizadas] = Nova_venda;
            num_vendas_realizadas++;
            printf("\n Venda Finalizada\n");

            printf("Itens:\n");
            Item_Vendido *item_atual = Nova_venda.primeiro_item;
            while (item_atual != NULL)
            {
                Produtos *info_prod = Busca_Produto_por_Cod(item_atual->cod_produto);
                printf("- Cod:%d | Nome: %s | Qtd: %d | Preco Un.: R$%.2f | Total: R$%.2f \n ",
                       item_atual->cod_produto,
                       info_prod->nome,
                       item_atual->qtd_Vendida,
                       item_atual->preco_venda,
                       item_atual->preco_venda * item_atual->qtd_Vendida);
                item_atual = item_atual->prox;
            }
            printf("Total da Venda: R$%.2f\n", Nova_venda.total_venda);
        }
        else
        {
            printf("\nErro: Limite maximo de vendas atingido.\n");
            Item_Vendido *atual = Nova_venda.primeiro_item;
            Item_Vendido *prox;
            while (atual != NULL)
            {
                prox = atual->prox;
                free(atual);
                atual = prox;
            }
        }
    }
    else
        printf("Nenhum Item adicionado.\n");
    printf("=======================================\n");
}

void ListarVendasPorData()
{

    int dia, mes, ano;
    int data_busca;
    int vendas_encontradas = 0;

    printf("\n ---- Listar vendas por Data ----\n");

    if (num_vendas_realizadas == 0)
    {
        printf("Nenhuma venda foi realizada. \n");
        printf("---------------------------\n");
        return;
    }

    printf("Digite a data que deseja buscar(aaaa/mm/dd): ");

    if (scanf("%d/%d/%d", &ano, &mes, &dia) != 3)
    {
        printf("Formato de data invalido , Use o formato (aaaa/mm/dd) \n");
        return;
    }

    data_busca = ano * 10000 + mes * 100 + dia;
    printf("\n ---- Vendas realizada em %04d/%02d/%02d ---- \n", ano, mes, dia);

    for (int i = 0; i < num_vendas_realizadas; i++)
    {

        if (lista_vendas[i].data == data_busca)
        {
            printf("Hora: %02d:%02d:%02d | CPF:%s | Valor Total: R$%.2f\n",
                   lista_vendas[i].horaVenda.hora,
                   lista_vendas[i].horaVenda.minuto,
                   lista_vendas[i].horaVenda.segundo,
                   lista_vendas[i].cpf_cliente,
                   lista_vendas[i].total_venda);
            vendas_encontradas++;
        }
    }

    if (vendas_encontradas == 0)
    {
        printf("Nenhuma venda encontrada para a data informada. \n");
    }
    printf("--------------------------------\n");
}

void limpar_buffer_stdin()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void AlterarPrecoEstoque()
{

    printf("\n ---- Alterar Estoque e Preco de Produtos ---- \n");

    int cod_busca;
    Produtos *produto_encontrado;
    float novo_preco;
    int qtd_adicionar;

    printf("Digite p codigo do produto que deseja alterar: ");
    scanf("%d", &cod_busca);

    produto_encontrado = Busca_Produto_por_Cod(cod_busca);

    if (produto_encontrado == NULL)
    {
        printf("ERRO: Código: %d nao foi encontrado.\n", cod_busca);
    }
    else
    {
        printf("\nProduto Encontrado\n");
        printf("- Nome: %s\n", produto_encontrado->nome);
        printf("- Preco atual: R$%.2f\n", produto_encontrado->preco);
        printf("Estoque atual: %d unidades\n\n", produto_encontrado->qtd_estoque);

        printf("Digite o novo preco do produto: ");

        if (scanf("%f", &novo_preco) != 1 || novo_preco < 0)
        {
            printf("Preco invalido. Operacao cancelada\n");
            limpar_buffer_stdin();
            return;
        }
        limpar_buffer_stdin();

        printf("Digite a quantidade a ser adicionada ao estoque: ");
        if (scanf("%d", &qtd_adicionar) != 1 || qtd_adicionar < 0)
        {
            printf("Quantidade invalida. Operacao cancelada.\n");
            limpar_buffer_stdin();
            return;
        }
        limpar_buffer_stdin();

        produto_encontrado->preco = novo_preco;
        produto_encontrado->qtd_estoque += qtd_adicionar;

        printf("\nProduto atualizado com sucesso\n");
        printf("Novo Preco R$%.2f\n", produto_encontrado->preco);
        printf("Nova Quantidade Estoque: %d unidade\n", produto_encontrado->qtd_estoque);
    }
    printf("----------------------------------------------------\n");
}

void RemoveProduto()
{

    printf("\n ---- Remover Produto do Estoque ----\n");

    int cod_busca;
    int indice_produto = -1; // usando -1 pra vacilitar a remoção
    int produto_vendido = 0;
    char escolha;

    printf("Digite o codigo do produto que deseja remover: ");
    if (scanf("%d", &cod_busca) != 1)
    {
        printf("Codigo invalido\n");
        limpar_buffer_stdin();
        return;
    }

    limpar_buffer_stdin();

    // Faz a busca do produto em produtos
    for (int i = 0; i < qtd_produtos; i++)
    {
        if (produtos[i].cod_produto == cod_busca)
        {
            indice_produto = i;
            break;
        }
    }

    if (indice_produto == -1)
    {
        printf("Erro: Produto com o codigo %d nao encontrado.\n", cod_busca);
        printf("---------------------------\n");
        return;
    }

    printf("Produto encontrado: %s\n", produtos[indice_produto].nome);

    // Faz a validação de o produto foi vendido alguma vez
    for (int i = 0; i < num_vendas_realizadas; i++)
    {
        Item_Vendido *item_atual = lista_vendas[i].primeiro_item;
        while (item_atual != NULL)
        {
            if (item_atual->cod_produto == cod_busca)
            {
                produto_vendido = 1;
                break;
            }
            item_atual = item_atual->prox;
        }
        if (produto_vendido)
        {
            break;
        }
    }

    if (produto_vendido)
    {
        printf("Erro: O produto '%s' nao pode ser removido pois ja foi utilizado em uma venda \n", produtos[indice_produto].nome);
    }
    else
    {
        printf("Deseja realmente remover '%s' do estoque (s/n): ", produtos[indice_produto].nome);
        scanf(" %c", &escolha);
        limpar_buffer_stdin();

        if (escolha == 's' || escolha == 'S')
        {
            for (int i = indice_produto; i < qtd_produtos - 1; i++)
            {
                produtos[i] = produtos[i + 1];
            }
            qtd_produtos--;

            printf("Produto removido\n");
        }
        else
        {
            printf("Operacao cancelada\n");
        }
    }

    printf("-----------------------------------\n");
}