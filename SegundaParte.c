#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define PrimeiraParte_H

#define MAX_RESOURCES 100

typedef struct {
    int resources[3];
    int importancia;
} Nave;

typedef struct {
    Nave** naves;
    int size;
    int capacity;
} FilaEspacial;

typedef struct {
    int count[MAX_RESOURCES][MAX_RESOURCES][MAX_RESOURCES];
} Permutations;

// Funções definidas em PrimeiraParte.c
Nave* lerNaveDoArquivo(FILE* arquivo);
int verificarDadosNave(Nave* nave);
Nave* removerNave(FilaEspacial* fila);
void mostrarFilaEspacial(FilaEspacial* fila);
void adicionarNaveComNovaImportancia(FilaEspacial* fila);
void verificarClandestinos(FilaEspacial* fila);
Nave* encontrarNavePorID(FilaEspacial* fila, int id);
void aumentarImportanciaEmergencia(FilaEspacial* fila, Nave* nav);

FilaEspacial* criarFilaEspacial(int capacity);
Nave* criarNave(int r1, int r2, int r3);
void inserirNave(FilaEspacial* fila, Nave* nave);
void atualizarPermutacoes(Permutations* permutations, Nave* nave);
int todasPermutacoesOcorreram(Permutations* permutations);

void atualizarPermutacoes(Permutations* permutations, Nave* nave) {
    // Ordena os recursos da nave em ordem crescente
    int resources[3];
    for (int i = 0; i < 3; i++) {
        resources[i] = nave->resources[i];
    }
    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (resources[j] < resources[i]) {
                int temp = resources[i];
                resources[i] = resources[j];
                resources[j] = temp;
            }
        }
    }

    // Atualiza o contador de permutações
    permutations->count[resources[0]][resources[1]][resources[2]]++;
}

int todasPermutacoesOcorreram(Permutations* permutations) {
    // Verifica se todas as permutações ocorreram
    for (int i = 0; i < MAX_RESOURCES; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            for (int k = 0; k < MAX_RESOURCES; k++) {
                if (i != j && i != k && j != k && permutations->count[i][j][k] == 0) {
                    return 0;
                }
            }
        }
    }

    // Se todas as permutações ocorreram, zera o contador de permutações
    for (int i = 0; i < MAX_RESOURCES; i++) {
        for (int j = 0; j < MAX_RESOURCES; j++) {
            for (int k = 0; k < MAX_RESOURCES; k++) {
                permutations->count[i][j][k] = 0;
            }
        }
    }

    return 1;
}


void MenuSegundaParte(FilaEspacial* fila, Permutations* permutations) {
    int opcao;
    do {
        printf("\nOpções:\n");
        printf("1. Inserir nave\n");
        printf("2. Inserir nave a partir de arquivo\n");
        printf("3. Remover nave\n");
        printf("4. Mostrar fila\n");
        printf("5. Adicionar nave com nova importância\n");
        printf("6. Verificar clandestinos\n");
        printf("7. Aumentar importância de nave em emergência\n");
        printf("8. Encontrar nave por ID\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); 
        switch (opcao) {
            case 1:
                // Inserir nave
                if (fila->size < fila->capacity) {
                    Nave* novaNave = criarNave(rand() % MAX_RESOURCES, rand() % MAX_RESOURCES, rand() % MAX_RESOURCES);
                    inserirNave(fila, novaNave);
                    atualizarPermutacoes(permutations, novaNave);
                    if (todasPermutacoesOcorreram(permutations)) {
                        printf("A abertura se expandirá.\n");
                    }
                } else {
                    printf("A fila está cheia.\n");
                }
                break;
            case 2:
              if (fila->size < fila->capacity) {
                    printf("Digite o nome do arquivo: ");
                    char nomeArquivo[100];
                    scanf("%s", nomeArquivo);
                    FILE* arquivo = fopen(nomeArquivo, "r");
                    if (arquivo != NULL) {
                        Nave* novaNave = lerNaveDoArquivo(arquivo);
                        fclose(arquivo);
                        
                        if (verificarDadosNave(novaNave)) {
                            inserirNave(fila, novaNave);
                            printf("Nave do arquivo inserida na fila com importância %d.\n", novaNave->importancia);
                        } else {
                            printf("Dados da nave do arquivo são inválidos. A nave não foi inserida na fila.\n");
                            free(novaNave); 
                        }
                    } else {
                        printf("Não foi possível abrir o arquivo.\n");
                    }
                } else {
                    printf("A fila espacial está cheia.\n");
                }
                break;
            case 3:
            Nave* navRemovida;
             navRemovida = removerNave(fila);
                if (navRemovida != NULL) {
                    printf("Nave removida com importância %d.\n", navRemovida->importancia);
                    free(navRemovida);
                } else {
                    printf("A fila espacial está vazia.\n");
                }
                break;
                
            case 4:
                 mostrarFilaEspacial(fila);
                break;
                break;
            case 5:
                adicionarNaveComNovaImportancia(fila);
                break;
                break;
            case 6:
                            verificarClandestinos(fila);
                break;
            case 7:
                // Aumentar importância de nave em emergência
                printf("Digite o ID da nave em emergência: ");
                int idNaveEmergencia;
                scanf("%d", &idNaveEmergencia);
                Nave* naveEmergencia = encontrarNavePorID(fila, idNaveEmergencia);
                if (naveEmergencia != NULL) {
                    aumentarImportanciaEmergencia(fila, naveEmergencia);
                    printf("Importância da nave em emergência aumentada para %d.\n", naveEmergencia->importancia);
                } else {
                    printf("Não foi encontrada nenhuma nave com o ID fornecido.\n");
                }
                break;
            case 8:
                // Encontrar nave por ID
                printf("Digite o ID da nave que deseja encontrar: ");
                int idNaveProcurada;
                scanf("%d", &idNaveProcurada);
                Nave* naveEncontrada = encontrarNavePorID(fila, idNaveProcurada);
                if (naveEncontrada != NULL) {
                    printf("A nave com ID %d foi encontrada.\n", idNaveProcurada);
                } else {
                    printf("Não foi encontrada nenhuma nave com o ID fornecido.\n");
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
    // Inicialize a fila e as permutações
    FilaEspacial* fila = criarFilaEspacial(10);
    Permutations permutations = {0};

    // Chame o menu da segunda parte
    MenuSegundaParte(fila, &permutations);

    // Libere a memória alocada para a fila e as naves
    for (int i = 0; i < fila->size; i++) {
        free(fila->naves[i]);
    }
    free(fila->naves);
    free(fila);
}

