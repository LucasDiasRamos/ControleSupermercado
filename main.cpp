#include "funcs.h"
#include <locale.h>

int main()
{
    //setlocale(LC_ALL,"");

    char nome_arquivo[MAX];
    int opcao_menu;

    printf("Digite o nome do arquivo para ser lido: ");
    scanf(" %[^\n]", nome_arquivo);

    leitura_aquivo(nome_arquivo);

    do
    {
        menu();
        scanf("%d", &opcao_menu);
        if(opcao_menu < 1){
            printf("Entrada invalida. Porfavor digite novamente.\n");
            opcao_menu = 0;
            continue;
        }

        switch (opcao_menu)
        {
        case 1:
            CadastrarVenda();
            break;
        case 2:
            listarVendasPorData();
            break;

        case 3:
            printf("Alterar estoque - Não implementado");
            break;
        case 4:
            printf("Remover - Não implementado");
            break;
        case 5:
            printf("Saindo");
            break;

        default:
            printf("Opção invalida. Digite novamente\n");
            break;
        }
    } while (opcao_menu != 5);

    return 0;
}
