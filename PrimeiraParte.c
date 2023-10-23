

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura que representa um passageiro em uma espaçonave, com campos para nome, idade, planeta de origem e ID.
typedef struct {
    char nome[50];           
    int idadePassageiro;     
    char planetaOrigem[50];  
    int ID;                  
} Viajante;

// Estrutura que representa um item em uma espaçonave, identificada por ID.
typedef struct {
    int IDitem;              
  
} Item;

// Estrutura que representa uma espaçonave, incluindo sua importância, uma matriz de passageiros (viajantes) e uma variedade de itens (itens).
typedef struct {
    int importancia;               
    Viajante viajantes[10];         
    Item itens[4];                   
    int quantidadePassageiros;      
    int quantidadeRecursos;         
} Nave;

// Estrutura que representa a fila de espaço, contendo uma matriz de espaçonaves (ponteiros Nave) e informações relevantes sobre a fila.
typedef struct {
    Nave** fila;                 
    int tamanho;                   
    int capacidade;                 
} FilaEspacial;

Nave* criarNave();
FilaEspacial* criarFilaEspacial(int capacidade);
void inserirNave(FilaEspacial* fila, Nave* novaNave);
void mostrarFilaEspacial(FilaEspacial* fila);
int verificarDadosNave(Nave* nav);
void atribuirNovaImportancia(FilaEspacial* fila, Nave* nav);
int gerarNovaImportanciaUnica(FilaEspacial* fila);
void ajustarImportancia(FilaEspacial* fila, int posicao);
Nave* removerNave(FilaEspacial* fila);
void ajustarImportanciaAposRemocao(FilaEspacial* fila, int posicao);
Nave* lerNaveDoArquivo(FILE* arquivo);
void salvarFilaEmArquivo(FilaEspacial* fila, const char* nomeArquivo);
void carregarFilaDeArquivo(FilaEspacial* fila, const char* nomeArquivo);
void adicionarNaveComNovaImportancia(FilaEspacial* fila);
void verificarClandestinos(FilaEspacial* fila);
int ehClandestina(Nave* nav);
int temItemComAltoValor(Nave* nav);
void aumentarImportanciaEmergencia(FilaEspacial* fila, Nave* nav);
void preencherDadosNave(Nave* nav);
void preencherItensNave(Nave* nav);
Nave* encontrarNavePorID(FilaEspacial* fila, int ID);

// Aloca memória para uma nova espaçonave e inicializa seus campos.
Nave* criarNave() {
    Nave* novaNave = (Nave*)malloc(sizeof(Nave));
    if (!novaNave) {
        perror("Erro ao alocar memória para a nave");
        exit(1);
    }

    novaNave->importancia = 0;
    novaNave->quantidadePassageiros = 0;
    novaNave->quantidadeRecursos = 0;

    return novaNave;
}

// Cria uma fila de espaço com a capacidade fornecida e aloca memória para seus componentes.
FilaEspacial* criarFilaEspacial(int capacidade) {
    FilaEspacial* fila = (FilaEspacial*)malloc(sizeof(FilaEspacial));
    if (!fila) {
        perror("Erro ao criar filaEspacial");
        exit(1);
    }
    fila->fila = (Nave**)malloc(capacidade * sizeof(Nave*));
    if (!fila->fila) {
        perror("Erro ao alocar memória para a fila");
        free(fila); 
        exit(1);
    }
    fila->tamanho = 0;
    fila->capacidade = capacidade;
    
    return fila;
}

// Insere uma espaçonave na fila do espaço se houver espaço.
void inserirNave(FilaEspacial* fila, Nave* novaNave) {
    if (fila->tamanho >= fila->capacidade) {
        printf("A filaEspacial está cheia.\n");
        return;
    }

    int posicao = fila->tamanho;
    fila->fila[posicao] = novaNave;
    fila->tamanho++;

}

// Exibe informações sobre a espaçonave na fila de espaço.
void mostrarFilaEspacial(FilaEspacial* fila) {
    printf("Naves na filaEspacial:\n");
    
    for (int i = 0; i < fila->tamanho; i++) {
        printf("Nave %d\n", i + 1); 
        printf("importancia: %d\n", fila->fila[i]->importancia);
        
        for(int j=0;j<fila->fila[i]->quantidadePassageiros;j++){
            printf("Viajante %d: %s\n", j+1, fila->fila[i]->viajantes[j].nome);
            printf("Idade: %d\n", fila->fila[i]->viajantes[j].idadePassageiro);
            printf("Planeta de Origem: %s\n", fila->fila[i]->viajantes[j].planetaOrigem);
            printf("ID: %d\n", fila->fila[i]->viajantes[j].ID);
        }
        
        for(int k=0;k<fila->fila[i]->quantidadeRecursos;k++){
            printf("Item %d ID: %d\n", k+1, fila->fila[i]->itens[k].IDitem);
        }
        
        printf("\n");
     }
}

// Verifica se a importância de uma espaçonave está dentro de um intervalo válido (1 a 100).
int verificarDadosNave(Nave* nav) {

   if (nav->importancia < 1 || nav->importancia > 100) {
       return 0; 
   }

   return 1; 
}

// Gera e atribui um novo valor de importância aleatório a uma espaçonave, garantindo que ela seja única na fila.
void atribuirNovaImportancia(FilaEspacial* fila, Nave* nav) {

   int novaImportancia = rand() % 100 + 1;

   for (int i = 0; i < fila->tamanho; i++) {
       if (fila->fila[i]->importancia == novaImportancia) {
           novaImportancia = gerarNovaImportanciaUnica(fila);
           break;
       }
   }

   nav->importancia = novaImportancia;
}

// Gera um novo valor de importância único para uma espaçonave.
int gerarNovaImportanciaUnica(FilaEspacial* fila) {
   int novaImportancia;
   int emUso; 
   
   do {
       novaImportancia = rand() % 100 + 1;
       emUso = 0; 
       
       for (int i = 0; i < fila->tamanho; i++) {
           if (fila->fila[i]->importancia == novaImportancia) {
               emUso = 1;
               break;
           }
       }
   } while (emUso);

   return novaImportancia;
}

// Ajusta a importância de uma espaçonave na fila após a inserção.
void ajustarImportancia(FilaEspacial* fila, int posicao) {
    Nave** array = fila->fila;
    int pai = (posicao - 1) / 2;

    while (posicao > 0 && array[posicao]->importancia > array[pai]->importancia) {
        
        Nave* temp = array[posicao];
        array[posicao] = array[pai];
        array[pai] = temp;

        posicao = pai;
        pai = (posicao - 1) / 2;
    }
}

// Remove a espaçonave com a maior importância da fila e ajusta a fila de acordo.
Nave* removerNave(FilaEspacial* fila) {
    if (fila->tamanho == 0) {
        printf("A filaEspacial está vazia.\n");
        return NULL;
    }

    Nave* naveRemovida = fila->fila[0];
    fila->fila[0] = fila->fila[fila->tamanho - 1];
    fila->tamanho--;

    ajustarImportanciaAposRemocao(fila, 0);

    return naveRemovida;
}

// Ajusta a importância da espaçonave na fila após a remoção.
void ajustarImportanciaAposRemocao(FilaEspacial* fila, int posicao) {
    Nave** array = fila->fila;
    int maiorImportancia = posicao;
    int esquerda = 2 * posicao + 1;
    int direita = 2 * posicao + 2;

    if (esquerda < fila->tamanho && array[esquerda]->importancia > array[maiorImportancia]->importancia) {
        maiorImportancia = esquerda;
    }

    if (direita < fila->tamanho && array[direita]->importancia > array[maiorImportancia]->importancia) {
        maiorImportancia = direita;
    }

    if (maiorImportancia != posicao) {
        // Troca a nave com a de maior importancia
        Nave* temp = array[posicao];
        array[posicao] = array[maiorImportancia];
        array[maiorImportancia] = temp;

        ajustarImportanciaAposRemocao(fila, maiorImportancia);
    }
}

// Lê os dados de uma espaçonave de um arquivo e cria uma nova espaçonave a partir dele.
Nave* lerNaveDoArquivo(FILE* arquivo) {
    char linha[100];
    Nave* nav = (Nave*)malloc(sizeof(Nave));
    
    if (!nav) {
        perror("Erro ao alocar memória para a nave");
        exit(1);
    }

    nav->importancia = 0;
    nav->quantidadePassageiros = 0; 
    nav->quantidadeRecursos = 0;    

    while (fgets(linha, sizeof(linha), arquivo)) {
        if (strstr(linha, "importancia:") != NULL) {
            sscanf(linha, "importancia: %d", &(nav->importancia));
        } else if (strstr(linha, "Viajante:") != NULL) {
            if (nav->quantidadePassageiros < 10) {
                Viajante* viajante = &(nav->viajantes[nav->quantidadePassageiros]);
                sscanf(linha, "Viajante: %s %d %s %d", viajante->nome, &(viajante->idadePassageiro), viajante->planetaOrigem, &(viajante->ID));
                nav->quantidadePassageiros++;
            }
        } else if (strstr(linha, "Item:") != NULL) {
            if (nav->quantidadeRecursos < 4) {
                Item* item = &(nav->itens[nav->quantidadeRecursos]);
                sscanf(linha, "Item: %d", &(item->IDitem));
                nav->quantidadeRecursos++;
            }
        }
    }

    if (nav->importancia == 0) {
        free(nav);
        return NULL;
    }

    return nav;
}

// Salva a fila de espaço em um arquivo.
void salvarFilaEmArquivo(FilaEspacial* fila, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "w");
    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo para salvar a fila.\n");
        return;
    }

    for (int i = 0; i < fila->tamanho; i++) {
        Nave* nav = fila->fila[i];
        fprintf(arquivo, "importancia: %d\n", nav->importancia);
        for (int j = 0; j < nav->quantidadePassageiros; j++) {
            Viajante* viajante = &(nav->viajantes[j]);
            fprintf(arquivo, "Viajante: %s %d %s %d\n", viajante->nome, viajante->idadePassageiro, viajante->planetaOrigem, viajante->ID);
        }
        for (int j = 0; j < nav->quantidadeRecursos; j++) {
            Item* item = &(nav->itens[j]);
            fprintf(arquivo, "Item: %d\n", item->IDitem);
        }
    }

    fclose(arquivo);
    printf("FilaEspacial salva no arquivo.\n");
}

// Carrega a fila de espaço de um arquivo.
void carregarFilaDeArquivo(FilaEspacial* fila, const char* nomeArquivo) {
    FILE* arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo para carregar a fila.\n");
        return;
    }

    while (1) {
        Nave* novaNave = lerNaveDoArquivo(arquivo);
        if (novaNave == NULL) {
            break;
        }

        inserirNave(fila, novaNave);
        printf("Nave do arquivo inserida na fila com importancia %d.\n", novaNave->importancia);
    }

    fclose(arquivo);
    printf("FilaEspacial carregada do arquivo.\n");
}

// Adiciona uma nova espaçonave com importância e dados gerados aleatoriamente.
void adicionarNaveComNovaImportancia(FilaEspacial* fila) {
    if (fila->tamanho >= fila->capacidade) {
        printf("A filaEspacial está cheia.\n");
        return;
    }

    Nave* novaNave = criarNave();
    preencherDadosNave(novaNave);
    preencherItensNave(novaNave);
    atribuirNovaImportancia(fila, novaNave);
    inserirNave(fila, novaNave);
    printf("Nave inserida na fila com nova importancia aleatória: %d.\n", novaNave->importancia);
}

// Verifica a existência de naves espaciais clandestinas e reatribui a sua importância.
void verificarClandestinos(FilaEspacial* fila) {
    for (int i = 0; i < fila->tamanho; i++) {
        Nave* nav = fila->fila[i];
        if (ehClandestina(nav)) {
            printf("Nave com importancia %d é clandestina e terá sua importancia realocada.\n", nav->importancia);
            atribuirNovaImportancia(fila, nav);
        }
    }
}

// Verifica se uma espaçonave é considerada clandestina (tem menos de 3 passageiros e um item com um valor alto).
int ehClandestina(Nave* nav) {

   return (nav->quantidadePassageiros < 3) && temItemComAltoValor(nav);
}

// Verifica se uma espaçonave tem um item com um valor acima de 50.
int temItemComAltoValor(Nave* nav) {
   for (int i = 0; i < nav->quantidadeRecursos; i++) {
       if (nav->itens[i].IDitem > 50) {
           return 1;
       }
   }
   return 0;
}

// Aumenta a importância de uma nave espacial em situações de emergência.
void aumentarImportanciaEmergencia(FilaEspacial* fila, Nave* nav) {
    int novaimportancia = nav->importancia + 20; 

    if (novaimportancia > 100) {
        novaimportancia = 100;
    }
    nav->importancia = novaimportancia;

    ajustarImportanciaAposRemocao(fila, 0);
}

// Recebe a entrada do usuário para preencher os dados de uma espaçonave.
void preencherDadosNave(Nave* nav) {
    printf("importancia da nave (de 1 a 100): ");
    scanf("%d", &(nav->importancia));

    do {
        printf("Número de viajantes (até 10): ");
        scanf("%d", &(nav->quantidadePassageiros));
        if (nav->quantidadePassageiros < 0 || nav->quantidadePassageiros > 10) {
            printf("O número de viajantes deve estar entre 0 e 10. Tente novamente.\n");
        }
    } while (nav->quantidadePassageiros < 0 || nav->quantidadePassageiros > 10);

    for (int i = 0; i < nav->quantidadePassageiros; i++) {
        Viajante* viajante = &(nav->viajantes[i]);
        printf("Dados do viajante %d:\n", i + 1);

        printf("Nome do viajante %d: ", i + 1);
        scanf("%s", viajante->nome);

        printf("Idade do viajante %d: ", i + 1);
        scanf("%d", &(viajante->idadePassageiro));

        printf("Planeta de origem do viajante %d: ", i + 1);
        scanf("%s", viajante->planetaOrigem);

        int duplicado;
        do {
            printf("Identificador único do viajante %d: ", i + 1);
            scanf("%d", &(viajante->ID));

            duplicado = 0;
            for (int j = 0; j < i; j++) {
                if (viajante->ID == nav->viajantes[j].ID) {
                    duplicado = 1;
                    printf("Identificador já existe. Escolha outro.\n");
                    break;
                }
            }
        } while (duplicado);
    }
}

// Recebe a entrada do usuário para preencher os dados dos itens em uma espaçonave.
void preencherItensNave(Nave* nav) {
    
    do {
        printf("Número de itens (até 4): ");
        scanf("%d", &(nav->quantidadeRecursos));
        if (nav->quantidadeRecursos < 0 || nav->quantidadeRecursos > 4) {
            printf("O número de itens deve estar entre 0 e 4. Tente novamente.\n");
        }
    } while (nav->quantidadeRecursos < 0 || nav->quantidadeRecursos > 4);

    for (int i = 0; i < nav->quantidadeRecursos; i++) {
        Item* item = &(nav->itens[i]);
        printf("Dados do item %d:\n", i + 1);

        int duplicado;
        do {
            printf("Identificador único do item %d: ", i + 1);
            scanf("%d", &(item->IDitem));

            duplicado = 0;
            for (int j = 0; j < i; j++) {
                if (item->IDitem == nav->itens[j].IDitem) {
                    duplicado = 1;
                    printf("Identificador já existe. Escolha outro.\n");
                    break;
                }
            }
        } while (duplicado);
    }
}

// Procura uma espaçonave na fila por seu ID.
Nave* encontrarNavePorID(FilaEspacial* fila, int ID) {
    for (int i = 0; i < fila->tamanho; i++) {
        Nave* nav = fila->fila[i];
        for (int j = 0; j < nav->quantidadePassageiros; j++) {
            if (nav->viajantes[j].ID == ID) {
                return nav;
            }
        }
    }
    return NULL; 
}

// Menu interativo que permite ao usuário realizar várias operações relacionadas a essa fila.
void exibirMenu(FilaEspacial* fila) {
    int opcao;
    Nave* navRemovida = NULL; 
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
        printf("9. Salvar fila em arquivo\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); 
        switch (opcao) {
            case 1:
                if (fila->tamanho < fila->capacidade) {
                    Nave* novaNave = criarNave();
                    preencherDadosNave(novaNave);
                    preencherItensNave(novaNave);
                    atribuirNovaImportancia(fila, novaNave);
                    inserirNave(fila, novaNave);
                    printf("Nave inserida na fila com nova importância %d.\n", novaNave->importancia);
                } else {
                    printf("A fila espacial está cheia.\n");
                }
                break;

            case 2:
                if (fila->tamanho < fila->capacidade) {
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

            case 5:
                adicionarNaveComNovaImportancia(fila);
                break;

            case 6:
                verificarClandestinos(fila);
                break;

            case 7:
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
                printf("Digite o ID da nave que deseja encontrar: ");
                int idNaveProcurada;
                scanf("%d", &idNaveProcurada);
                Nave* naveEncontrada = encontrarNavePorID(fila, idNaveProcurada);
                if (naveEncontrada !=  NULL) {
                    printf("A nave foi encontrada.\n");
                } else {
                    printf("Não foi encontrada nenhuma nave com o ID fornecido.\n");
            }
            break;

            case 9:
                printf("Digite o nome do arquivo para salvar a fila: ");
                char nomeArquivo[100];
                scanf("%s", nomeArquivo);
                salvarFilaEmArquivo(fila, nomeArquivo);
            break;

            case 0:
                printf("Saindo do programa.\n");
            break;

            default:
              printf("\nOpção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);
}

void PrimeiraParte() {
    srand(time(NULL));

    int capacidadeFila = 10;
    const char* nomeArquivo = "entrada.txt";

    FilaEspacial* fila = criarFilaEspacial(capacidadeFila);
    carregarFilaDeArquivo(fila, nomeArquivo);  

    exibirMenu(fila);

    for (int i = 0; i < fila->tamanho; i++) {
        free(fila->fila[i]);
    }
    free(fila->fila);
    free(fila);
}