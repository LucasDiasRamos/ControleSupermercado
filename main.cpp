#include "funcs.h"
#include <locale.h>

int main()
{
    // setlocale(LC_ALL,"");

    char nome_arquivo[MAX];
    int opcao_menu;
    

    printf("Digite o nome do arquivo para ser lido: ");
    scanf(" %[^\n]", nome_arquivo);

    limpar_buffer_stdin();

    Leitura_aquivo(nome_arquivo);

    do
    {
        Menu();
        scanf("%d", &opcao_menu);
        if (opcao_menu < 1)
        {
            printf("Entrada invalida. Porfavor digite novamente.\n");
            opcao_menu = 0;
            continue;
        }

        limpar_buffer_stdin();
        switch (opcao_menu)
        {
        case 1:
            CadastrarVenda();
            break;
        case 2:
            ListarVendasPorData();
            break;

        case 3:
            AlterarPrecoEstoque();
            break;
        case 4:
            RemoveProduto();
            break;
        case 5:
            SairDoPrograma(nome_arquivo);
            break;

        default:
            printf("Opção invalida. Digite novamente\n");
            break;
        }
    } while (opcao_menu != 5);

    return 0;
}
