/******************************************
  Lucas Mateus Dias Ramos
  Trabalho Alg.Prog. 2 Produtos
  Professor(a): Graziela Santos de Araujo
  Universidade Federal do Mato Grosso do Sul

*******************************************/

Esté programa realiza a leitura de um arquivo txt com as informações de produtos e baseado nesse arquivo são feitas funções de venda de produtos 
controle de estoque e alteração de estoque e gerado um relatorio de vendas do dia escolhido pelo usuario.

foi usado a biblioteca time.h para puxar a data e a hora diretamente do OS do usuario

gcc funcs.cpp main.cpp -o Produtos -Wall 

função limpa_buffer_stdin foi criada para evitar os erros de scanf quando se existia um \n antes da entrada do usuario.