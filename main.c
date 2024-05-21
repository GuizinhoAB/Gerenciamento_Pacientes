#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Autor: Guilherme de Abreu
// RA: 22.222.028-7

// Estrutura para a data
struct Data {
  int dia;
  int mes;
  int ano;
};

// Estrutura para o registro do paciente
struct Registro {
  char nome[30];
  int idade;
  char RG[9];          // Aumentado para acomodar um caractere nulo
  struct Data entrada; // Incluindo a estrutura de data no registro
};

// Estrutura para o nó da Árvore Binária de Busca
struct EABB {
  struct Registro dados;
  struct EABB *filhoEsq;
  struct EABB *filhoDir;
};

// Estrutura para a Árvore Binária de Busca
struct ABB {
  struct EABB *raiz;
  int qtde;
};

// Estrutura para a célula da Lista Encadeada
typedef struct Celula {
  struct Registro valor;
  struct Celula *proximo;
} Celula;

// Estrutura para a Lista Encadeada
typedef struct Lista {
  int qtde;
  Celula *primeiro;
} Lista;

// Estrutura para a célula da Fila
typedef struct EFila {
  struct Registro dados;
  struct EFila *proximo;
} EFila;

// Estrutura para a Fila
typedef struct Fila {
  EFila *head;
  EFila *tail;
  int qtde;
} Fila;

//*********** ÁRVORE ***********//

// Função para inicializar a Árvore Binária de Busca (ABB)
struct ABB *inicializarABB() {
  // Aloca espaço para a árvore na memória
  struct ABB *arvore = (struct ABB *)malloc(sizeof(struct ABB));

  // Verifica se a alocação foi bem-sucedida
  if (arvore != NULL) {
    // Inicializa os campos da árvore
    arvore->raiz = NULL;
    arvore->qtde = 0;
  }

  // Retorna a árvore inicializada
  return arvore;
}

// Função auxiliar para criar um novo nó da EABB
struct EABB *criarEABB(struct Registro dados) {
  // Aloca espaço na memória para o novo nó da EABB
  struct EABB *novoNo = (struct EABB *)malloc(sizeof(struct EABB));

  // Verifica se a alocação foi bem-sucedida
  if (novoNo != NULL) {
    // Inicializa os campos do novo nó
    novoNo->dados = dados;
    novoNo->filhoEsq = NULL;
    novoNo->filhoDir = NULL;
  }

  // Retorna o novo nó da EABB
  return novoNo;
}

// Função auxiliar para inserir um novo nó na ABB
struct EABB *inserirEABB(struct EABB *raiz, struct Registro dados) {
  // Se a raiz for nula, cria um novo nó
  if (raiz == NULL) {
    return criarEABB(dados);
  }

  // Comparando pelos RGs
  if (strcmp(dados.RG, raiz->dados.RG) < 0) {
    raiz->filhoEsq = inserirEABB(raiz->filhoEsq, dados);
  } else if (strcmp(dados.RG, raiz->dados.RG) > 0) {
    raiz->filhoDir = inserirEABB(raiz->filhoDir, dados);
  }

  // Retorna a raiz da árvore atualizada
  return raiz;
}

// Função auxiliar para encontrar o nó mínimo na Árvore Binária de Busca
struct EABB *encontrarMinimo(struct EABB *raiz) {
  // Percorre a árvore até encontrar o nó mais à esquerda
  while (raiz->filhoEsq != NULL) {
    raiz = raiz->filhoEsq;
  }
  // Retorna o nó mínimo encontrado
  return raiz;
}

// Função auxiliar para remover um nó da Árvore Binária de Busca
struct EABB *removerEABB(struct EABB *raiz, char rg[9]) {
  // Verifica se a árvore está vazia
  if (raiz == NULL) {
    return raiz;
  }

  // Encontrar o nó a ser removido
  if (strcmp(rg, raiz->dados.RG) < 0) {
    raiz->filhoEsq = removerEABB(raiz->filhoEsq, rg);
  } else if (strcmp(rg, raiz->dados.RG) > 0) {
    raiz->filhoDir = removerEABB(raiz->filhoDir, rg);
  } else {
    // Nó encontrado, realizar a remoção
    if (raiz->filhoEsq == NULL) {
      struct EABB *temp = raiz->filhoDir;
      free(raiz);
      return temp;
    } else if (raiz->filhoDir == NULL) {
      struct EABB *temp = raiz->filhoEsq;
      free(raiz);
      return temp;
    }

    // Nó com dois filhos, encontrar o sucessor in-order (mínimo no filho
    // direito)
    struct EABB *temp = encontrarMinimo(raiz->filhoDir);

    // Copiar os dados do sucessor in-order para este nó
    raiz->dados = temp->dados;

    // Remover o sucessor in-order
    raiz->filhoDir = removerEABB(raiz->filhoDir, temp->dados.RG);
  }

  // Retorna a raiz após a remoção
  return raiz;
}

// Função auxiliar para atualizar um nó na Árvore Binária de Busca
struct EABB *atualizarEABB(struct EABB *raiz, char rg[9],
                           struct Registro novosDados) {
  // Verifica se a árvore está vazia
  if (raiz == NULL) {
    return raiz;
  }

  // Encontrar o nó a ser atualizado
  if (strcmp(rg, raiz->dados.RG) < 0) {
    raiz->filhoEsq = atualizarEABB(raiz->filhoEsq, rg, novosDados);
  } else if (strcmp(rg, raiz->dados.RG) > 0) {
    raiz->filhoDir = atualizarEABB(raiz->filhoDir, rg, novosDados);
  } else {
    // Nó encontrado, realizar a atualização
    raiz->dados = novosDados;
    // Exibe mensagem de sucesso
    printf("\nDados atualizados com sucesso!\n");
  }

  // Retorna a raiz após a atualização
  return raiz;
}

// Função auxiliar para preencher um array com os registros da árvore
void preencherArray(struct EABB *raiz, struct Registro *registros,
                    int *indice) {
  if (raiz != NULL) {
    preencherArray(raiz->filhoDir, registros, indice);

    registros[*indice] = raiz->dados;
    (*indice)++;

    preencherArray(raiz->filhoEsq, registros, indice);
  }
}

// Função para mostrar registros ordenados por um critério específico
void mostrarRegistrosOrdenados(struct ABB *arvore, int criterio) {
  // Verifica se a árvore está vazia
  if (arvore == NULL || arvore->raiz == NULL) {
    printf("A árvore está vazia.\n");
    return;
  }

  // Cria um array temporário para armazenar os registros
  struct Registro registros[arvore->qtde];
  int indice = 0;

  // Preenche o array com os registros da árvore
  preencherArray(arvore->raiz, registros, &indice);

  // Ordena o array de registros pelo critério especificado
  for (int i = 0; i < arvore->qtde - 1; i++) {
    for (int j = 0; j < arvore->qtde - i - 1; j++) {
      int comparacao = 0;

      switch (criterio) {
      case 1:
        comparacao = registros[j].entrada.ano - registros[j + 1].entrada.ano;
        break;
      case 2:
        comparacao = registros[j].entrada.mes - registros[j + 1].entrada.mes;
        break;
      case 3:
        comparacao = registros[j].entrada.dia - registros[j + 1].entrada.dia;
        break;
      case 4:
        comparacao = registros[j].idade - registros[j + 1].idade;
        break;
      default:
        printf("Critério de ordenação inválido.\n");
        break;
      }

      if (comparacao > 0) {
        // Troca os dados dos registros no array
        struct Registro temp = registros[j];
        registros[j] = registros[j + 1];
        registros[j + 1] = temp;
      }
    }
  }

  // Exibe os registros ordenados pelo critério
  printf("\nRegistros ordenados:\n");
  for (int i = 0; i < arvore->qtde; i++) {
    if (criterio == 1) {
      printf("\nNome: %s\nIdade: %d\nAno: %d\n", registros[i].nome,
             registros[i].idade, registros[i].entrada.ano);
    } else if (criterio == 2) {
      printf("\nNome: %s\nIdade: %d\nMês: %d\n", registros[i].nome,
             registros[i].idade, registros[i].entrada.mes);
    } else if (criterio == 3) {
      printf("\nNome: %s\nIdade: %d\nDia: %d\n", registros[i].nome,
             registros[i].idade, registros[i].entrada.dia);
    } else if (criterio == 4) {
      printf("\nNome: %s\nIdade: %d\nAno: %d\n", registros[i].nome,
             registros[i].idade, registros[i].entrada.ano);
    }
  }
}

// Função para mostrar registros ordenados por ano de registro
void mostrarPorAno(struct ABB *arvore) {
  mostrarRegistrosOrdenados(arvore,
                            1); // O critério 1 representa a ordenação por ano
}

void mostrarPorMes(struct ABB *arvore) { mostrarRegistrosOrdenados(arvore, 2); }

void mostrarPorDia(struct ABB *arvore) { mostrarRegistrosOrdenados(arvore, 3); }

void mostrarPorIdade(struct ABB *arvore) {
  mostrarRegistrosOrdenados(arvore, 4);
}

//*********** LISTA ***********//

// Função para inicializar uma Lista Encadeada
Lista *inicializa_lista() {
  // Aloca espaço para a lista na memória
  Lista *l = malloc(sizeof(Lista));

  // Inicializa os campos da lista
  l->primeiro = NULL;
  l->qtde = 0;

  return l;
}

// Função para cadastrar um paciente na Lista Encadeada
void cadastrarPaciente(Lista *lista, struct Registro novoRegistro) {
  // Aloca espaço para a nova célula na memória
  Celula *novaCelula = malloc(sizeof(Celula));

  // Preenche os campos da nova célula
  novaCelula->valor = novoRegistro;

  // Insere a nova célula no início da lista
  novaCelula->proximo = lista->primeiro;
  lista->primeiro = novaCelula;

  // Incrementa a quantidade de elementos na lista
  lista->qtde++;
}

// Função para inserir manualmente um paciente na Lista Encadeada e na Árvore
void inserirManualmente(Lista *lista, struct ABB *arvore) {
  // Estrutura para armazenar os dados do novo paciente
  struct Registro novoRegistro;

  // Solicita e recebe os dados do paciente
  printf("Digite o nome do paciente: ");
  scanf("%29s", novoRegistro.nome);

  printf("Digite a idade do paciente: ");
  scanf("%d", &novoRegistro.idade);

  printf("Digite o RG do paciente: ");
  scanf("%8s", novoRegistro.RG);

  printf("Digite o dia, mês e ano de entrada (DD MM AAAA): ");
  scanf("%02d %02d %04d", &novoRegistro.entrada.dia, &novoRegistro.entrada.mes,
        &novoRegistro.entrada.ano);

  // Cadastra na Lista Encadeada
  cadastrarPaciente(lista, novoRegistro);

  // Insere na Árvore Binária de Busca
  arvore->raiz = inserirEABB(arvore->raiz, novoRegistro);
  arvore->qtde++;

  printf("Paciente cadastrado com sucesso!\n");
}

// Consulta e exibição do paciente por RG
void mostrarListaPorRG(Lista *lista, char RG[9]) {
  // Inicializa o ponteiro para percorrer a lista
  Celula *atual = lista->primeiro;

  // Percorre a lista em busca do paciente com o RG fornecido
  while (atual != NULL) {
    if (strcmp(atual->valor.RG, RG) == 0) {
      // Imprime os dados do paciente
      printf("\nNome: %s\n", atual->valor.nome);
      printf("Idade: %d\n", atual->valor.idade);
      printf("RG: %s\n", atual->valor.RG);
      printf("Data de entrada: %02d/%02d/%04d\n", atual->valor.entrada.dia,
             atual->valor.entrada.mes, atual->valor.entrada.ano);
      return; // Se encontrou o paciente, encerra a função
    }
    atual = atual->proximo;
  }

  // Se o loop terminou e não encontrou o paciente
  printf("Paciente não encontrado.\n");
}

// Função para consultar paciente por RG
void consultarPorRG(Lista *lista) {
  // Variável para armazenar o RG a ser consultado
  char rgConsulta[9];

  // Solicita o RG ao usuário
  printf("Digite o RG do paciente a ser consultado: ");
  scanf("%s", rgConsulta);

  // Chama a função de consulta e exibição
  mostrarListaPorRG(lista, rgConsulta);
}

// Função para mostrar todos os pacientes na Lista Encadeada
void mostrarLista(Lista *lista) {
  // Inicializa o ponteiro para percorrer a lista
  Celula *atual = lista->primeiro;

  // Percorre a lista e imprime os dados de cada paciente
  while (atual != NULL) {
    printf("\nNome: %s\n", atual->valor.nome);
    printf("Idade: %d\n", atual->valor.idade);
    printf("RG: %s\n", atual->valor.RG);
    printf("Data de entrada: %02d/%02d/%04d\n", atual->valor.entrada.dia,
           atual->valor.entrada.mes, atual->valor.entrada.ano);

    atual = atual->proximo;
  }
}

// Função para atualizar os dados de um paciente na Lista Encadeada e na Árvore
void atualizarPaciente(Lista *lista, struct ABB *arvore, char rg[9],
                       struct Registro novosDados) {
  // Atualiza na Lista Encadeada
  Celula *atualLista = lista->primeiro;
  while (atualLista != NULL) {
    if (strcmp(atualLista->valor.RG, rg) == 0) {
      atualLista->valor = novosDados;
      break;
    }
    atualLista = atualLista->proximo;
  }

  // Atualiza na Árvore Binária de Busca
  arvore->raiz = atualizarEABB(arvore->raiz, rg, novosDados);
}

// Função para atualizar os dados de um paciente manualmente
void atualizarDadosManualmente(Lista *lista, struct ABB *arvore) {
  // Declaração das variáveis
  char rgAtualizar[9];
  struct Registro novosDados;

  // Solicitação dos novos dados ao usuário
  printf("Digite o RG do paciente a ser atualizado: ");
  scanf("%s", rgAtualizar);

  printf("Digite o novo nome do paciente: ");
  scanf("%s", novosDados.nome);

  printf("Digite a nova idade do paciente: ");
  scanf("%d", &novosDados.idade);

  printf("Digite o novo RG do paciente: ");
  scanf("%s", novosDados.RG);

  printf("Digite o novo dia, mês e ano de entrada (formato: DD MM AAAA): ");
  scanf("%d %d %d", &novosDados.entrada.dia, &novosDados.entrada.mes,
        &novosDados.entrada.ano);

  // Chama a função para atualizar os dados na Lista Encadeada e na Árvore
  atualizarPaciente(lista, arvore, rgAtualizar, novosDados);
}

// Função para remover um paciente da Lista Encadeada e da Árvore Binária
void removerPaciente(Lista *lista, struct ABB *arvore, char rg[9]) {
  // Remove na Lista Encadeada
  Celula *atualLista = lista->primeiro;
  Celula *anteriorLista = NULL;

  while (atualLista != NULL) {
    if (strcmp(atualLista->valor.RG, rg) == 0) {
      if (anteriorLista == NULL) {
        lista->primeiro = atualLista->proximo;
      } else {
        anteriorLista->proximo = atualLista->proximo;
      }
      free(atualLista);
      lista->qtde--;
      printf("Paciente removido da lista com sucesso!\n");
      break;
    }
    anteriorLista = atualLista;
    atualLista = atualLista->proximo;
  }

  // Remove na Árvore Binária de Busca
  arvore->raiz = removerEABB(arvore->raiz, rg);
  printf("Paciente removido da árvore com sucesso!\n");
}

// Função para remover um paciente manualmente da Lista Encadeada e da Árvore
void removerManualmente(Lista *lista, struct ABB *arvore) {
  // Declaração da variável
  char rgRemover[9];

  // Solicitação do RG ao usuário
  printf("Digite o RG do paciente a ser removido: ");
  scanf("%s", rgRemover);

  // Chama a função para remover o paciente da Lista Encadeada e da Árvore
  removerPaciente(lista, arvore, rgRemover);

  // Exibe mensagem de sucesso
  printf("\nPaciente removido com sucesso!\n");
}

//*********** FILA ***********//

// Função para inicializar a fila de pacientes
void inicializarFila(Fila *fila) {
  fila->head = NULL;
  fila->tail = NULL;
  fila->qtde = 0;
}

// Função para enfileirar um paciente na Fila
void enfileirarPaciente(Fila *fila, Lista *lista) {
  // Declaração da variável
  char rg[9];

  // Solicitação do RG ao usuário
  printf("Digite o RG do paciente a ser enfileirado: ");
  scanf("%s", rg);

  // Procura o paciente na Lista Encadeada
  Celula *atual = lista->primeiro;
  while (atual != NULL) {
    if (strcmp(atual->valor.RG, rg) == 0) {
      // Cria uma nova célula na Fila e a preenche com os dados do paciente
      EFila *novaCelula = (EFila *)malloc(sizeof(EFila));
      if (novaCelula == NULL) {
        printf("Erro ao alocar memória.\n");
        return;
      }

      novaCelula->dados = atual->valor;
      novaCelula->proximo = NULL;

      // Adiciona a nova célula na Fila
      if (fila->qtde == 0) {
        fila->head = novaCelula;
        fila->tail = novaCelula;
      } else {
        fila->tail->proximo = novaCelula;
        fila->tail = novaCelula;
      }

      fila->qtde++;
      printf("Paciente enfileirado com sucesso!\n");
      return;
    }
    atual = atual->proximo;
  }

  // Se o loop terminou e não encontrou o paciente
  printf("Paciente com RG %s não encontrado na lista.\n", rg);
}

// Função para desenfileirar paciente da Fila e removê-lo da Lista Encadeada
void desenfileirarPaciente(Fila *fila, Lista *lista) {
  // Verifica se a fila está vazia
  if (fila->qtde == 0) {
    printf("A fila está vazia.\n");
    return;
  }

  // Remove o primeiro paciente da Fila
  EFila *temp = fila->head;
  fila->head = fila->head->proximo;

  // Remove o paciente da Lista Encadeada
  Celula *anterior = NULL;
  Celula *atual = lista->primeiro;
  while (atual != NULL) {
    if (strcmp(atual->valor.RG, temp->dados.RG) == 0) {
      if (anterior == NULL) {
        lista->primeiro = atual->proximo;
      } else {
        anterior->proximo = atual->proximo;
      }

      free(atual);
      break;
    }

    anterior = atual;
    atual = atual->proximo;
  }

  // Libera a memória da célula removida da Fila
  free(temp);

  // Atualiza a quantidade de pacientes na Fila
  fila->qtde--;

  // Se a Fila ficou vazia, atualiza o ponteiro da cauda
  if (fila->qtde == 0) {
    fila->tail = NULL;
  }

  // Exibe mensagem de sucesso
  printf("Paciente desenfileirado com sucesso!\n");
}

// Função para mostrar a Fila de Atendimento
void mostrarFila(Fila *fila) {
  // Inicializa o ponteiro para percorrer a Fila
  EFila *atual = fila->head;

  // Exibe o cabeçalho da Fila
  printf("\n===Fila de Atendimento===\n\n");

  // Percorre a Fila e exibe os pacientes
  while (atual != NULL) {
    printf("%s - %d anos\n", atual->dados.nome, atual->dados.idade);
    atual = atual->proximo;
  }

  // Exibe o fim da Fila
  printf("\n===Fim da fila===\n");
}

//*********** DADOS EM ARQUIVOS ***********//

// Função para salvar os dados dos pacientes em um arquivo
void salvarDados(Lista *lista, const char *nomeArquivo) {
  // Abre o arquivo para escrita
  FILE *arquivo = fopen(nomeArquivo, "w");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo para escrita.\n");
    return;
  }

  // Inicializa o ponteiro para percorrer a lista
  Celula *atual = lista->primeiro;

  // Percorre a lista e escreve os dados no arquivo
  while (atual != NULL) {
    fprintf(arquivo, "%s %d %s %d %d %d\n", atual->valor.nome,
            atual->valor.idade, atual->valor.RG, atual->valor.entrada.dia,
            atual->valor.entrada.mes, atual->valor.entrada.ano);

    atual = atual->proximo;
  }

  // Exibe mensagem de sucesso
  printf("Dados salvos com sucesso!\n");

  // Fecha o arquivo
  fclose(arquivo);
}

// Função para carregar os dados dos pacientes de um arquivo
void carregarDados(Lista *lista, struct ABB *arvore, const char *nomeArquivo) {
  // Abre o arquivo para leitura
  FILE *arquivo = fopen(nomeArquivo, "r");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo para leitura.\n");
    return;
  }

  // Estrutura para armazenar os dados do paciente
  struct Registro novoRegistro;

  // Lê os dados do arquivo e cadastra na lista e na árvore
  while (fscanf(arquivo, "%29s %d %8s %d %d %d", novoRegistro.nome,
                &novoRegistro.idade, novoRegistro.RG, &novoRegistro.entrada.dia,
                &novoRegistro.entrada.mes, &novoRegistro.entrada.ano) == 6) {
    // Cadastra na lista
    cadastrarPaciente(lista, novoRegistro);

    // Insere na Árvore Binária de Busca
    arvore->raiz = inserirEABB(arvore->raiz, novoRegistro);
    arvore->qtde++;
  }

  // Exibe mensagem de sucesso
  printf("Dados carregados com sucesso!\n");

  // Fecha o arquivo
  fclose(arquivo);
}

// Função com as minhas informações
void sobre() {

  printf("\nNome: Guilherme de Abreu\n");
  printf("Ciclo: 4°\n");
  printf("Curso: Ciência da Computação\n");
  printf("Disciplina: Estrutra de Dados\n");
  printf("Data: 13/11/2023\n");
};

// Função do Menu Principal
void menuOpcoes(Lista *lista, Fila *filaAtendimento, struct ABB *arvore) {
  int opcao;

  do {
    printf("\n======== Menu de Opções ========\n");
    printf("\n=========== CADASTRO ===========\n");
    printf("1. Inserir manualmente\n");
    printf("2. Atualizar dados manualmente\n");
    printf("3. Remover paciente\n");
    printf("4. Consultar por RG\n");
    printf("5. Mostrar lista de pacientes\n");
    printf("\n====== CARREGAR / SALVAR =======\n");
    printf("6. Salvar dados dos pacientes\n");
    printf("7. Carregar dados dos pacientes\n");
    printf("\n========= ATENDIMENTO ==========\n");
    printf("8. Enfileirar paciente\n");
    printf("9. Desenfileirar paciente\n");
    printf("10. Mostrar fila de pacientes\n");
    printf("\n=========== PESQUISA ===========\n");
    printf("11. Mostrar Registros por Ano\n");
    printf("12. Mostrar Registros por Mês\n");
    printf("13. Mostrar Registros por Dia\n");
    printf("14. Mostrar Registros por Idade\n");
    printf("\n============= SOBRE ============\n");
    printf("15. Sobre\n");
    printf("\n============== SAIR ============\n");
    printf("0. Sair\n");
    printf("\n================================\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
    case 1:
      inserirManualmente(lista, arvore);
      break;
    case 2:
      atualizarDadosManualmente(lista, arvore);
      break;
    case 3:
      removerManualmente(lista, arvore);
      break;
    case 4:
      consultarPorRG(lista);
      break;
    case 5:
      mostrarLista(lista);
      break;
    case 6:
      salvarDados(lista, "dados_pacientes.txt");
      break;
    case 7:
      carregarDados(lista, arvore, "dados_para_carregar.txt");
      break;
    case 8:
      enfileirarPaciente(filaAtendimento, lista);
      break;
    case 9:
      desenfileirarPaciente(filaAtendimento, lista);
      break;
    case 10:
      mostrarFila(filaAtendimento);
      break;
    case 11:
      mostrarPorAno(arvore);
      break;
    case 12:
      mostrarPorMes(arvore);
      break;
    case 13:
      mostrarPorDia(arvore);
      break;
    case 14:
      mostrarPorIdade(arvore);
      break;
    case 15:
      sobre();
      break;
    case 0:
      printf("Operação finalizada!\n");
      break;
    default:
      printf("Opção inválida. Tente novamente.\n");
    }
  } while (opcao != 0);
}

int main() {
  // Inicializa a lista de pacientes
  Lista *listaPacientes = inicializa_lista();

  // Inicializa a fila de atendimento
  Fila filaAtendimento;
  inicializarFila(&filaAtendimento);

  // Inicializa a Árvore Binária de Busca (ABB)
  struct ABB *arvore = inicializarABB();

  // Exibe o menu de opções
  menuOpcoes(listaPacientes, &filaAtendimento, arvore);

  return 0;
}