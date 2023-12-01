#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define TAMANHO_MAX_HEAP 2000

//  FILA COM HEAP
typedef struct FPheap
{
    int tamanho;
    int items[TAMANHO_MAX_HEAP];
    int prioridades[TAMANHO_MAX_HEAP];
} FPheap;


FPheap *criar_FP_heap()
{
    FPheap *nova_fila = (FPheap*) malloc(sizeof(FPheap)); 
    nova_fila->tamanho = 0;
    return nova_fila;
}


int achar_no_pai(int i)
{
    return i/2;
}

int achar_filho_esquerda(int i)
{
    return i*2;
}

int achar_filho_direita(int i)
{
    return i*2+1;
}

void swap(int *a,int *b)
{
    int aux = *a;
    *a = *b;
    *b = aux;
    return;
}

int enfileirar_heap(FPheap *fila,int prioridade,int item)
{
    int comparacao = 0;

    if(fila->tamanho >= TAMANHO_MAX_HEAP)
    {
        printf("Heap overflow\n");
        return 0;
    }
    else
    {
        fila->prioridades[++fila->tamanho] = prioridade;
        fila->items[fila->tamanho] = item;
        
        int indice_chave = fila->tamanho;
        int indice_pai= achar_no_pai(fila->tamanho);

        //equanto nao chegar a raiz e  equanto este ser maior que o pai
        while (indice_pai >= 1 && 
                fila->prioridades[indice_chave] > fila->prioridades[indice_pai])
        {
            swap(&fila->prioridades[indice_chave],&fila->prioridades[indice_pai]);
            swap(&fila->items[indice_chave],&fila->items[indice_pai]);

            //atualizar os "status" do antigo nó pai agora trocado como filho;
            indice_chave = indice_pai;
            indice_pai = achar_no_pai(indice_chave);

            comparacao++;
        }
        
        return comparacao;
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////


//  FILA SEM HEAP
typedef struct NoFila
{
    int prioridade;
    int item;
    struct NoFila *next;
} NoFila;

typedef struct FilaDePrioridade
{
    NoFila *head;
} FilaDePrioridade;

FilaDePrioridade* criar_FP()
{
    FilaDePrioridade *novo_no = (FilaDePrioridade*) malloc(sizeof(FilaDePrioridade));

    novo_no->head = NULL;
    return novo_no;
}

int vazia(FilaDePrioridade* fp)
{
    return (fp->head == NULL);
}   

int enfileirar(FilaDePrioridade *fp, int prioridade,int item)
{
    int comparacoes = 0;
    NoFila *novo_no = (NoFila*) malloc(sizeof(NoFila));
    novo_no->prioridade = prioridade;
    novo_no->item = item;

    if(vazia(fp) || prioridade > fp->head->prioridade)
    {
        novo_no->next = fp->head;
        fp->head = novo_no;
        return 0;
    } 
    else
    {
        NoFila* aux = fp->head;
        while(aux->next != NULL && aux->next->prioridade > prioridade)
        {
            aux = aux->next;
            comparacoes++;
        }
        novo_no->next = aux->next;
        aux->next = novo_no;

        return comparacoes;
    }
}

void printar_fila_de_prioridades(FilaDePrioridade *fp)
{
    NoFila *aux = fp->head;
    while (aux != NULL)
    {
        printf("(i:%d, p:%d);\n",aux->item,aux->prioridade);
        aux = aux->next;
    }   
}

int main()
{
    FilaDePrioridade *fp_sem_heap = criar_FP();
    FPheap *pq_heap = criar_FP_heap();

    FILE *arquivo_entrada; //pointer para o arquivo input
    FILE *arquivo_saida; 

    char buffer[20];

    arquivo_entrada = fopen("entrada.txt","r");
    arquivo_saida = fopen("saida.txt","w");
    

    if(arquivo_entrada == NULL)
    {
        printf("\nArquivo de entrada não encontrado\n");
        exit(1);
    } 

    //leia o texto do arquivo e coloque em buffer
    while(fgets(buffer,sizeof(buffer),arquivo_entrada))
    {

        int prioridade = atoi(buffer);
        int comparacoes_sem_heap;
        int comparacoes_com_heap;
        
        comparacoes_sem_heap = enfileirar(fp_sem_heap,prioridade,1);
        
        comparacoes_com_heap = enfileirar_heap(pq_heap,prioridade,0);

        fprintf(arquivo_saida,"%d %d %d\n",prioridade,comparacoes_sem_heap,comparacoes_com_heap);
    }

    fclose(arquivo_entrada);
    fclose(arquivo_saida);
}