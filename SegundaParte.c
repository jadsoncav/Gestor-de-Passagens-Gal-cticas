#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PrimeiraParte_H

#define maximoRecursos 100

// Definindo a estrutura Espaconave
typedef struct {
    int suprimentos[3]; 
    int relevancia; 
} Espaconave;

// Definindo a estrutura FilaEspacial
typedef struct {
    Espaconave** espaconaves; 
    int dimensao; 
    int habilidade; 
} FilaEspacial;

// Definindo a estrutura Rearranjos
typedef struct {
    int count[maximoRecursos][maximoRecursos][maximoRecursos]; 
} Rearranjos;

// Protótipos de função
Espaconave* lerEspaconaveDoArquivo(FILE* arquivo); 
int verificarDadosEspaconave(Espaconave* espaconave); 
Espaconave* removerEspaconave(FilaEspacial* fila); 
void mostrarFilaEspacial(FilaEspacial* fila); 
void adicionarEspaconaveComNovaRelevencia(FilaEspacial* fila); 
void verificarClandestinos(FilaEspacial* fila); 
Espaconave* encontrarEspaconavePorID(FilaEspacial* fila, int id); 
void aumentarRelevanciaEmergencia(FilaEspacial* fila, Espaconave* nav); 

FilaEspacial* criarFilaEspacial(int habilidade); 
Espaconave* criarEspaconave(int r1, int r2, int r3);
void inserirEspaconave(FilaEspacial* fila, Espaconave* espaconave);
void atualizarRearranjos(Rearranjos* rearranjos, Espaconave* espaconave);
int todosRearranjosOcorreram(Rearranjos* rearranjos); 

// Atualiza o contador de rearranjos após a inserção de uma nova espaçonave na fila espacial
void atualizarRearranjos(Rearranjos* rearranjos, Espaconave* espaconave) {
    int suprimentos[3];
    for (int i = 0; i < 3; i++) {
        suprimentos[i] = espaconave->suprimentos[i];
    }
    for (int i = 0; i < 3; i++) {
       for (int j = i + 1; j < 3; j++) {
    if (suprimentos[j] < suprimentos[i]) {
        int temp = suprimentos[i];
        suprimentos[i] = suprimentos[j];
        suprimentos[j] = temp;
    }
}

        }
    rearranjos->count[suprimentos[0]][suprimentos[1]][suprimentos[2]]++;
}

// Verifica se todos os rearranjos ocorreram e zera o contador de rearranjos se todos ocorreram
int todosRearranjosOcorreram(Rearranjos* rearranjos) {
    for (int i = 0; i < maximoRecursos; i++) {
        for (int j = 0; j < maximoRecursos; j++) {
            for (int k = 0; k < maximoRecursos; k++) {
                if (i != j && i != k && j != k && rearranjos->count[i][j][k] == 0) {
                    return 0;
                }
            }
        }
    }
    for (int i = 0; i < maximoRecursos; i++) {
        for (int j = 0; j < maximoRecursos; j++) {
            for (int k = 0; k < maximoRecursos; k++) {
                rearranjos->count[i][j][k] = 0;
            }
        }
    }
    return 1;
}
void MenuSegundaParte(FilaEspacial* fila, Rearranjos* rearranjos) {
    int opcao;
    do {
        printf("\nOpções:\n");
        printf("1. Inserir espaçonave\n"); 
        printf("2. Inserir espaçonave a partir de arquivo\n"); 
        printf("3. Remover espaçonave\n"); 
        printf("4. Mostrar fila\n"); 
        printf("5. Adicionar espaçonave com nova relevencia\n"); 
        printf("6. Verificar clandestinos\n"); 
        printf("7. Aumentar relevencia de espaçonave em emergência\n");
        printf("8. Encontrar espaçonave por ID\n"); 
        printf("0. Sair\n"); 
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();
        switch (opcao) {
            case 1:
                if (fila->dimensao < fila->habilidade) {
                    Espaconave* novaEspaconave = criarEspaconave(rand() % maximoRecursos, rand() % maximoRecursos, rand() % maximoRecursos);
                    inserirEspaconave(fila, novaEspaconave);
                    atualizarRearranjos(rearranjos, novaEspaconave);
                    if (todosRearranjosOcorreram(rearranjos)) {
                        printf("A abertura se expandirá.\n");
                    }
                } else {
                    printf("A fila está cheia.\n");
                }
                break;
            case 2:
                if (fila->dimensao < fila->habilidade) {
                    printf("Digite o nome do arquivo: ");
                    char nomeArquivo[100];
                    scanf("%s", nomeArquivo);
                    FILE* arquivo = fopen(nomeArquivo, "r");
                    if (arquivo != NULL) {
                        Espaconave* novaEspaconave = lerEspaconaveDoArquivo(arquivo);
                        fclose(arquivo);
                        if (verificarDadosEspaconave(novaEspaconave)) {
                            inserirEspaconave(fila, novaEspaconave);
                            printf("Espaconave do arquivo inserida na fila com relevencia %d.\n", novaEspaconave->relevancia);
                        } else {
                            printf("Dados da espaçonave do arquivo são inválidos. A espaconave não foi inserida na fila.\n");
                            free(novaEspaconave);
                        }
                    } else {
                        printf("Não foi possível abrir o arquivo.\n");
                    }
                } else {
                    printf("A fila espacial está cheia.\n");
                }
                break;
            case 3:
                Espaconave* navRemovida;
                navRemovida = removerEspaconave(fila);
                if (navRemovida != NULL) {
                    printf("Espaconave removida com relevencia %d.\n", navRemovida->relevancia);
                    free(navRemovida);
                } else {
                    printf("A fila espacial está vazia.\n");
                }
                break;
            case 4:
                mostrarFilaEspacial(fila); 
                break;
            case 5:
                adicionarEspaconaveComNovaRelevencia(fila); 
                break;
            case 6:
                verificarClandestinos(fila); 
                break;
            case 7:
                printf("Digite o ID da espaçonave em emergência: ");
                int idNaveEmergencia;
                scanf("%d", &idNaveEmergencia);
                Espaconave* naveEmergencia = encontrarEspaconavePorID(fila, idNaveEmergencia);
                if (naveEmergencia != NULL) {
                    aumentarRelevanciaEmergencia(fila, naveEmergencia); 
                    printf("Relevencia da espaçonave em emergência aumentada para %d.\n", naveEmergencia->relevancia);
                } else {
                    printf("Não foi encontrada nenhuma espaçonave com o ID fornecido.\n");
                }
                break;case 8:
               
                printf("Digite o ID da espaçonave que deseja encontrar: ");
                int idEspaconaveProcurada;
                scanf("%d", &idEspaconaveProcurada);
                Espaconave* espaconaveEncontrada = encontrarEspaconavePorID(fila, idEspaconaveProcurada);
                if (espaconaveEncontrada != NULL) {
                    printf("A espaçonave com ID %d foi encontrada.\n", idEspaconaveProcurada);
                } else {
                    printf("Não foi encontrada nenhuma espaçonave com o ID fornecido.\n");
                }
                break;
            case 0:
               
                printf("\nSaindo do programa.\n");
                break;
            default:
              
                printf("\nOpção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);
}

void SegundaParte() {

    FilaEspacial* fila = criarFilaEspacial(10);
    Rearranjos rearranjos = {0};


    MenuSegundaParte(fila, &rearranjos);

 
    for (int i = 0; i < fila->dimensao; i++) {
        free(fila->espaconaves[i]);
    }
    free(fila->espaconaves);
    free(fila);
}

