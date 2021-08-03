#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no {
    char** C;
    char** nomes;
    int* tamanhos;
    struct no** P;
    struct no* prox;
    struct no* ante;
    int folha;
    //folha=1 é folha
    //folha=0 nao é folha
    int n;
} no;

//07/08/2019 Horário: 22:52:05 a236ea905c9253eb79794faa66732541

no* busca_folha (no* x, char* c);
no* busca_no (no* x, char* range1, char* range2);
no* busca_no_intervalo (no* x, char* range1, char* range2);
void divisao_no (no* x);
no* criar_no ();
void inserir (no* node,char* nome,int tamanho,char* hash);
char* max(char* a, char* b);
char* min(char* a, char* b);

int k=0;
int divisoes=0;
no* raiz = NULL;

int main (int argc, char *argv[])
{
    int i,ordem,qntArquivos,qntOperacoes;
    char* operacao = (char*)malloc(8*sizeof(char));
    char* rangeOUconsulta = (char*)malloc(41*sizeof(char));

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    fscanf(input,"%d",&ordem);
    k=ordem;

    fscanf(input,"%d",&qntArquivos);
    char** nomes = (char**)malloc(qntArquivos*sizeof(char*));
    int* tamanhos = (int*)malloc(qntArquivos*sizeof(int));
    char** hashes = (char**)malloc(qntArquivos*sizeof(char*));

    for(i=0;i<qntArquivos;i++)
    {
        //printf("insersao antes\n");
        nomes[i]=(char*)malloc(31*sizeof(char));
        hashes[i]=(char*)malloc(51*sizeof(char));
        fscanf(input,"%s",nomes[i]);
        //printf("Arquivo %d --> nome: %s /",i,nomes[i]);
        fscanf(input,"%d",&tamanhos[i]);
        //printf(" tamanho: %d /",tamanhos[i]);
        fscanf(input,"%s",hashes[i]);
        //printf(" hash: %s\n",hashes[i]);
        inserir(raiz,nomes[i],tamanhos[i],hashes[i]);
        //printf("divisoes: %d\n",divisoes);
        //printf("(%d)\n",i);
    }
    printf("insersoes completas\n");

    fscanf(input,"%d",&qntOperacoes);

    no* nobusca = NULL;
    no* nobuscaIntervalo = NULL;
    //char* consulta = (char*)malloc(41*sizeof(char));
    char* range1 = (char*)malloc(41*sizeof(char));
    char* range2 = (char*)malloc(41*sizeof(char));
    char* nomeInsersao = (char*)malloc(41*sizeof(char));
    int tamanhoInsersao = 0;
    char* hashInsersao = (char*)malloc(41*sizeof(char));
    char* rangeAux;
    //printf("Operacoes:\n");


    for(i=0;i<qntOperacoes;i++)
    {
        fscanf(input,"%s",operacao);
        if(strcmp(operacao,"INSERT")==0)
        {
            //printf("Insersao\n");
            fscanf(input,"%s",nomeInsersao);
            fscanf(input,"%d",&tamanhoInsersao);
            fscanf(input,"%s",hashInsersao);
            //printf("Arquivo %d --> nome: %s /  tamanho: %d /  hash: %s\n",i,nomeInsersao,tamanhoInsersao,hashInsersao);
            inserir(raiz,nomeInsersao,tamanhoInsersao,hashInsersao);
            //printf("divisoes: %d\n",divisoes);
        }
        else
        {
            //printf("Select");
            fscanf(input,"%s",rangeOUconsulta);
            if(strcmp(rangeOUconsulta,"RANGE")==0)
            {
                //printf(" Range\n");
                fscanf(input,"%s",range1);
                fscanf(input,"%s",range2);
                if(strcmp(range1,range2) > 0)
                {
                    rangeAux = range1;
                    range1 = range2;
                    range2 = rangeAux;
                }
                fprintf(output,"[%s,%s]\n",range1,range2);
                //printf("[%s,%s]\n",range1,range2);
                nobuscaIntervalo = busca_no(raiz,range1,range2);
                //printf("2");
                if(nobuscaIntervalo == NULL)
                    fprintf(output,"-\n");

                /*else{
                    printf("nobusca--> folha: %d /// n: %d /// primeiro: %s /// ultimo: %s\n",nobuscaIntervalo->folha,nobuscaIntervalo->n,nobuscaIntervalo->C[0],nobuscaIntervalo->C[nobuscaIntervalo->n-1]);
                    printf("\tprox--> folha: %d /// n: %d /// %s /// ultimo: %s\n",nobuscaIntervalo->prox->folha,nobuscaIntervalo->prox->n,nobuscaIntervalo->prox->C[0],nobuscaIntervalo->prox->C[nobuscaIntervalo->prox->n-1]);
                    if(nobuscaIntervalo->prox->prox != NULL)
                    printf("\tprox prox--> folha: %d /// n: %d /// %s /// ultimo: %s\n\n",nobuscaIntervalo->prox->prox->folha,nobuscaIntervalo->prox->prox->n,nobuscaIntervalo->prox->prox->C[0],nobuscaIntervalo->prox->prox->C[nobuscaIntervalo->prox->prox->n-1]);
                }*/

                while(nobuscaIntervalo != NULL && strcmp(nobuscaIntervalo->C[0], range2) <= 0)
                {
                    for(int h=0;h<nobuscaIntervalo->n;h++)
                        fprintf(output,"%s:size=%d,hash=%s\n",nobuscaIntervalo->nomes[h],nobuscaIntervalo->tamanhos[h],nobuscaIntervalo->C[h]);
                    //printf("folha: %d\n",nobuscaIntervalo->folha);
                    nobuscaIntervalo = nobuscaIntervalo->prox;
                }
            }
            else
            {
                //printf(" exato\n");
                fprintf(output,"[%s]\n",rangeOUconsulta);
                //printf("Select:[%s] \n",consulta);
                nobusca=busca_folha(raiz,rangeOUconsulta);
                //nobusca=busca(raiz,rangeOUconsulta);
                if(nobusca != NULL)
                {
                    //printf("ENCONTRADO \n");
                    for(int j=0;j<nobusca->n;j++)
                    {
                        //fprintf -->
                        fprintf(output,"%s:size=%d,hash=%s\n",nobusca->nomes[j],nobusca->tamanhos[j],nobusca->C[j]);
                    }
                }
                else
                {
                    //printf("BUSCA NAO ENCONTRADA\n");
                    fprintf(output,"-\n");
                }
            }
        }
    }
    fclose(input);
    fclose(output);
    return 0;
}

void inserir (no* node,char* nome,int tamanho,char* hash)
{
    no* aux = node;

    if(raiz == NULL){
        no* temp = NULL;
        temp = criar_no(k);
        strcpy(temp->C[0],hash);
        strcpy(temp->nomes[0], nome);
        temp->tamanhos[0] = tamanho;
        temp->folha=1;
        temp->n = 1;
        temp->prox = NULL;
        temp->ante = NULL;
        raiz = temp;
        return;
    }
    //printf("222\n");
    int i=0,j=0,b=0;
    if(aux->folha == 0)
    { //NAO EH FOLHA
        //printf("NAO EH NO FOLHA\n");
        while(strcmp(hash,aux->C[i]) >= 0 && i < aux->n)
            i++;
        inserir(aux->P[i],nome,tamanho,hash);
    }
    else
    { //EH FOLHA
        //NO SO PODE SER INSERIDO NA FOLHA
        //printf("EH NO FOLHA\n");
        if(aux==raiz)
        {
            if(aux->n == k-1)
            {
                //printf("EH NO FOLHA RAIZ E ESTA CHEIO\n");
                //printf("a\n");
                //NO raiz esta cheio
                //INSERE NO INDICE EXTRA
                //encontra o lugar para o hash
                while(strcmp(hash,aux->C[i]) >= 0 && i < aux->n)
                    i++;
                //passa todos da direita para o lado usando o indice extra
                for(j=k-2;j>=i;j--)
                {
                    strcpy(aux->C[j+1], aux->C[j]);
                    strcpy(aux->nomes[j+1], aux->nomes[j]);
                    aux->tamanhos[j+1] = aux->tamanhos[j];
                }
                //copia os dados
                strcpy(aux->C[i], hash);
                strcpy(aux->nomes[i], nome);
                aux->tamanhos[i] = tamanho;
                aux->n++;
                //divide o no
                divisao_no(aux);
                divisoes--;
                raiz = aux;
                //repete a chave no no folha passando para a direita
                for(j=aux->P[1]->n-1;j>=0;j--)
                {
                    strcpy(aux->P[1]->C[j+1], aux->P[1]->C[j]);
                    strcpy(aux->P[1]->nomes[j+1], aux->P[1]->nomes[j]);
                    aux->P[1]->tamanhos[j+1] = aux->P[1]->tamanhos[j];
                }
                strcpy(aux->P[1]->C[0], aux->C[0]);
                strcpy(aux->P[1]->nomes[0], aux->nomes[0]);
                aux->P[1]->tamanhos[0] = aux->tamanhos[0];
                aux->P[1]->n++;
                //atualiza se eh folha
                raiz->P[0]->folha = 1;
                raiz->P[1]->folha = 1;
                raiz->folha = 0;
                //ponteiro para o prox
                raiz->prox = NULL;
                raiz->ante = NULL;
                raiz->P[0]->prox = raiz->P[1];
                raiz->P[0]->ante = NULL;
                raiz->P[1]->ante = raiz->P[0];
                raiz->P[1]->prox = NULL;
                return;
            }
            else
            {
                //printf("EH NO FOLHA RAIZ E NAO ESTA CHEIO\n");
                //no raiz nao esta cheio
                //acha o lugar para encaixar
                while(strcmp(hash,aux->C[i]) >= 0 && i < aux->n)
                    i++;
                //empurra todos da direita para o lado
                for(j=aux->n-1;j>=i;j--)
                {
                    strcpy(aux->C[j+1], aux->C[j]);
                    strcpy(aux->nomes[j+1], aux->nomes[j]);
                    aux->tamanhos[j+1] = aux->tamanhos[j];
                }
                //copia os dados
                strcpy(aux->C[i], hash);
                strcpy(aux->nomes[i], nome);
                aux->tamanhos[i] = tamanho;
                aux->n++;
                return;
            }
        }
        else
        {
            //NO NAO E A RAIZ
            if(aux->n == k-1)
            {
                //printf("EH NO FOLHA NAO RAIZ E ESTA CHEIO\n");
                //NO NAO RAIZ ESTA CHEIO
                //encontra o lugar para o hash
                while(strcmp(hash,aux->C[i]) >= 0 && i < aux->n)
                    i++;
                //passa todos da direita para o lado usando o indice extra
                for(j=k-2;j>=i;j--)
                {
                    strcpy(aux->C[j+1], aux->C[j]);
                    strcpy(aux->nomes[j+1], aux->nomes[j]);
                    aux->tamanhos[j+1] = aux->tamanhos[j];
                }
                //copia os dados
                strcpy(aux->C[i], hash);
                strcpy(aux->nomes[i], nome);
                aux->tamanhos[i] = tamanho;
                aux->n++;
                //divide o no
                divisao_no(aux);
                //repete a chave no no folha passando para a direita
                for(j=aux->P[1]->n-1;j>=0;j--)
                {
                    strcpy(aux->P[1]->C[j+1], aux->P[1]->C[j]);
                    strcpy(aux->P[1]->nomes[j+1], aux->P[1]->nomes[j]);
                    aux->P[1]->tamanhos[j+1] = aux->P[1]->tamanhos[j];
                }
                strcpy(aux->P[1]->C[0], aux->C[0]);
                strcpy(aux->P[1]->nomes[0], aux->nomes[0]);
                aux->P[1]->tamanhos[0] = aux->tamanhos[0];
                aux->P[1]->n++;
                //atualiza se e folha
                aux->P[0]->folha = 1;
                aux->P[1]->folha = 1;
                aux->folha = 0;
                //atualiza ponteiro para o prox
                if(aux->ante != NULL)
                    aux->ante->prox = aux->P[0];
                if(aux->prox != NULL)
                    aux->prox->ante = aux->P[1];
                aux->P[0]->prox = aux->P[1];
                aux->P[1]->prox = aux->prox;
                aux->P[0]->ante = aux->ante;
                aux->P[1]->ante = aux->P[0];
                aux->prox = NULL;
                aux->ante = NULL;
                return;
            }
            else
            {
                //printf("EH NO FOLHA NAO RAIZ E NAO ESTA CHEIO\n");
                //no NAO raiz nao esta cheio
                //acha o lugar para encaixar
                while(strcmp(hash,aux->C[i]) >= 0 && i < aux->n)
                    i++;
                //empurra todos da direita para o lado
                for(j=aux->n-1;j>=i;j--)
                {
                    strcpy(aux->C[j+1], aux->C[j]);
                    strcpy(aux->nomes[j+1], aux->nomes[j]);
                    aux->tamanhos[j+1] = aux->tamanhos[j];
                }
                //copia os dados
                strcpy(aux->C[i], hash);
                strcpy(aux->nomes[i], nome);
                aux->tamanhos[i] = tamanho;
                aux->n++;
                //nao precisa atualizar os ponteiros filho pq so insere chaves na raiz
                return;
            }
        }
    }
    //printf("17\n");
    //atualiza todos os pais na volta da recursao
    if(divisoes > 0)
    {
        divisoes--;
        int m=0;
        if(aux->n == k-1)
        {
            //printf("NO CHEIO NA VOLTA DA DIVISAO\n");
            //no cheio na volta da divisao
            //encontra o lugar para o hash
            while(strcmp(aux->P[i]->C[0],aux->C[m]) >= 0 && m < aux->n)
                m++;
            //passa todos da direita para o lado usando o indice extra
            for(j=k-2;j>=m;j--)
            {
                strcpy(aux->C[j+1], aux->C[j]);
                strcpy(aux->nomes[j+1], aux->nomes[j]);
                aux->tamanhos[j+1] = aux->tamanhos[j];
            }
            //copia os dados
            strcpy(aux->C[m], aux->P[i]->C[0]);
            strcpy(aux->nomes[m], aux->P[i]->nomes[0]);
            aux->tamanhos[m] = aux->P[i]->tamanhos[0];
            aux->n++;
            //atualiza os ponteiros, usando o ponteiro extra
            for(j=k;j>i;j--)
                aux->P[j] = aux->P[j-1];
            aux->P[i+1] = aux->P[i]->P[1];
            aux->P[i] = aux ->P[i]->P[0];
            //atualiza os nos prox
            /*if(aux->P[i]->folha == 1)
            {
                if(i > 0)
                    aux->P[i-1]->prox = aux->P[i];
                else if (aux->P[0]->ante != NULL)
                    aux->P[0]->ante->prox = aux->P[0];
            }*/
            //divide o no
            divisao_no(aux);
            if(aux==raiz){
                //printf("\ndivisao da raiz na volta\n");
                divisoes--;
            }
            return;
        }
        else
        {
            //printf("NO COM ESPACO NA VOLTA DA DIVISAO\n");
            //volta da divisao com o no com espaco disponivel
            //empurra todas as chaves da direita de i , no ponteiro pai, para o lado
            for(j=aux->n-1;j>=i;j--)
            {
                strcpy(aux->C[j+1], aux->C[j]);
                strcpy(aux->nomes[j+1], aux->nomes[j]);
                aux->tamanhos[j+1] = aux->tamanhos[j];
            }
            //copia o X para o no.
            strcpy(aux->C[i], aux->P[i]->C[0]);
            strcpy(aux->nomes[i], aux->P[i]->nomes[0]);
            aux->tamanhos[i] = aux->P[i]->tamanhos[0];
            aux->n++;
            //atualiza os ponteiros
            for(j=k-1;j>i+1;j--)
                aux->P[j] = aux->P[j-1];
            aux->P[i+1] = aux->P[i]->P[1];
            aux->P[i] = aux ->P[i]->P[0];
            //atualiza os ponteiros da lista de folhas
            /*if(aux->P[i]->folha == 1)
            {
                if(i > 0)
                    aux->P[i-1]->prox = aux->P[i];
                else if (aux->P[0]->ante != NULL)
                    aux->P[0]->ante->prox = aux->P[0];
            }*/
            return;
        }
    }
    /*temp = busca(raiz,hash);
    if(temp != NULL){
        printf("Chave (hash) ja existe");
        return raiz;
    }*/
    return;
}

no* busca_folha (no* x, char* c)
{
    no* r = NULL;
    int i = 0;
    while(i < x->n && strcmp(c, x->C[i]) >= 0)
        i++;
    if(x->folha == 0){
        r = busca_folha(x->P[i],c);
    }
    else{
        if(strcmp(c, x->C[i-1])==0)
            r=x;
    }
    return r;
}

no* busca_no (no* x, char* range1, char* range2)
{
    no* r = NULL;
    if(x != NULL){
        int i = 0;
        while(i < x->n && strcmp(range1, x->C[i]) >= 0)
            i++;
        if(x->folha == 0){
            r = busca_no(x->P[i],range1,range2);
        }
        else{
            if(strcmp(x->C[x->n-1],range1) < 0 || strcmp(x->C[0],range2) > 0)
                return NULL;
            if(i==0 && strcmp(range2,x->C[0]) < 0)
                return NULL;
            if(i==0 && strcmp(range2,x->C[0]) >= 0)
                return x;
            if(strcmp(range1,x->C[i-1]) == 0)
                return x;
            if(i==x->n)
                return NULL;
            if(strcmp(range2,x->C[i]) < 0)
                return NULL;
            else
                r=x;
        }
    }
    return r;
}

no* busca_no_intervalo (no* x, char* range1, char* range2)
{
    no* r = NULL;
    char* ini = min(range1,range2);
    char* fim = max(range1,range2);
    int i = 0;
    while(i < x->n && strcmp(ini, x->C[i]) >= 0)
        i++;
    if(x->folha == 0)
        r=busca_no_intervalo(x->P[i],ini,fim);
    else
    {
        if(strcmp(x->C[x->n-1],ini) < 0 || strcmp(x->C[0],fim) > 0)
            return NULL;
        if(strcmp(fim,x->C[i]) < 0)
            return NULL;
        if(i==x->n)
            return NULL;
        else
            return x;
    }
    return r;
}

void divisao_no (no* x)
{
    divisoes++;
    int i = 0;
    no* y = criar_no(k);
    no* z = criar_no(k);

    for(i=0;i<(k/2);i++){
        strcpy(y->C[i], x->C[i]);
        strcpy(y->nomes[i], x->nomes[i]);
        y->tamanhos[i] = x->tamanhos[i];
        y->P[i] = x->P[i];
        y->n++;
    }
    y->P[i] = x->P[i];

    for(i=(k/2)+1;i < x->n;i++){
        strcpy(z->C[i-(k/2)-1], x->C[i]);
        strcpy(z->nomes[i-(k/2)-1], x->nomes[i]);
        z->tamanhos[i-(k/2)-1] = x->tamanhos[i];
        z->P[i-(k/2)-1] = x->P[i];
        z->n++;
    }
    z->P[i-(k/2)-1] = x->P[i];

    strcpy(x->C[0], x->C[k/2]);
    strcpy(x->nomes[0], x->nomes[k/2]);
    x->tamanhos[0] = x->tamanhos[k/2];
    x->P[0] = y;
    x->P[1] = z;
    x->n = 1;
    x->folha = 0;
}

no* criar_no ()
{
    //k = tamanho do array de chaves  (+ 1 para a chave extra) = ordem
    int i=0;
    no* temp = NULL;
    temp = (no*)malloc(sizeof(no));

    temp->C = (char**)malloc(k*sizeof(char*));
    for(i=0;i<k;i++){
        temp->C[i] = (char*)malloc(41*sizeof(char));
    }

    temp->nomes = (char**)malloc(k*sizeof(char*));
    for(i=0;i<k;i++){
        temp->nomes[i] = (char*)malloc(31*sizeof(char));
    }

    temp->tamanhos = (int*)malloc(k*sizeof(int));

    temp->P = (no**)malloc((k+1)*sizeof(no*));

    temp->n = 0;

    temp->prox = NULL;
    temp->ante = NULL;

    return temp;
}

char* max(char* a, char* b)
{
    if(strcmp(a,b)>0){
        return a;
    }
    else{
        return b;
    }
}

char* min(char* a, char* b)
{
    if(strcmp(a,b)<0){
        return a;
    }
    else{
        return b;
    }
}
