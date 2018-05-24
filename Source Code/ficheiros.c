#include "ficheiros.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//TODO CLIENTES BANIDOS!!
ptrGuitarra importaGuitarras(ptrGuitarra listaGuitarras, int *total){
  FILE *f;
  f=fopen(NOME_FICHEIRO_GUITARRAS, "r");
  int id,precoAluguerDia,valor,estado;
  char nome[100];
  if(f==NULL){
    printf("Erro ao abrir o ficheiro guitarras.txt!\n");
    return listaGuitarras;
  }
  ptrGuitarra tmp;
  //TODO:Verificações ordem!
  while (fscanf(f," %d %d %d %d %[^\n]s",&id, &precoAluguerDia, &valor, &estado, nome)==5) {
    tmp=realloc(listaGuitarras, sizeof(guitarra)*(*total+1));
    if(tmp==NULL){
      printf("Erro de alocacao de memoria!!\n");
      return listaGuitarras;
    }
    listaGuitarras=tmp;
    listaGuitarras[*total].id=id;
    listaGuitarras[*total].precoAluguerDia=precoAluguerDia;
    listaGuitarras[*total].valor=valor;
    listaGuitarras[*total].estado=estado;
    strcpy(listaGuitarras[*total].nome, nome);
    (*total)++;
  }
  fclose(f);
  printf("Guitarras importadas com sucesso!\n");
  return listaGuitarras;
}

ptrCliente importaClientes(ptrCliente listaClientes,ptrGuitarra listaGuitarras, int total){
  FILE *f;
  f=fopen(NOME_FICHEIRO_CLIENTES, "r");
  if(f==NULL){
    printf("Erro ao abrir o ficheiro clientes.txt!\n");
    return listaClientes;
  }
  //TODO:ADD VERIFICACOES
  int nif,nAlugueresTotal;
  char nome[100];
  ptrCliente tmpCliente;
  ptrCliente listaTmp;
  ptrAluguer alugueresTmp;
  while (fscanf(f, " %d %d %[^\n]s", &nif, &nAlugueresTotal, nome)==3) {
    tmpCliente=malloc(sizeof(cliente));
    if(tmpCliente==NULL){
      printf("Erro de alocacao de memoria!!\n");
      return listaClientes;
    }
    //Preenche
    strcpy(tmpCliente->nome, nome);
    tmpCliente->nif=nif;
    tmpCliente->nAlugueresTotal=nAlugueresTotal;
    tmpCliente->nAlugueresAtual=0;
    tmpCliente->nEntregasAtrasadas=0;
    tmpCliente->nEntregasDanificadas=0;
    tmpCliente->prox=NULL;
    tmpCliente->alugueres=NULL;

    if(nAlugueresTotal>0){
      int id,estado,diaI,mesI,anoI,index;
      for(int i=0; i<nAlugueresTotal; i++) {
        fscanf(f, " %d %d %d %d %d",&id, &estado, &diaI, &mesI, &anoI);
        ptrAluguer tmpAluguer=malloc(sizeof(aluguer));
        if(tmpAluguer==NULL){
          printf("Erro de alocacao de memoria!!\n");
          return listaClientes;
        }
        //index fica a -1 se a guitarra não estiver no vetor
        index=devolveIndexGuitarra(listaGuitarras, total, id);
        //Verifica se a guitarra esta na lista de guitarras
        if(index != -1){
          tmpAluguer->guitarra=&listaGuitarras[index];
        }else{
          printf("A guitarra com o id %d, não está na lista de guitarras!\n",id);
          //Para corrigir a posição no ficheiro
          if(estado>0){
            int diaE,mesE,anoE;
            fscanf(f, " %d %d %d",&diaE, &mesE, &anoE);
          }
          continue;
        }
        tmpAluguer->diaInicio=diaI;
        tmpAluguer->mesInicio=mesI;
        tmpAluguer->anoInicio=anoI;
        tmpAluguer->estado=estado;
        tmpAluguer->prox=NULL;
        if(estado>0){
          int diaE,mesE,anoE;
          fscanf(f, " %d %d %d",&diaE, &mesE, &anoE);
          tmpAluguer->diaEntrega=diaE;
          tmpAluguer->mesEntrega=mesE;
          tmpAluguer->anoEntrega=anoE;
          int diasAtraso=calculaDiasAtraso(tmpAluguer->diaInicio, tmpAluguer->mesInicio, tmpAluguer->anoInicio, tmpAluguer->diaEntrega, tmpAluguer->mesEntrega, tmpAluguer->anoEntrega);
          if(diasAtraso>0)
            tmpCliente->nEntregasAtrasadas+=1;
          if(estado==ENTREGA_DANIFICADA)
            tmpCliente->nEntregasDanificadas+=1;
        }else{
          tmpCliente->nAlugueresAtual+=1;
        }

        //Verifica se é o primeiro aluguer do cliente
        if(tmpCliente->alugueres==NULL){
          tmpCliente->alugueres=tmpAluguer;
          alugueresTmp=tmpCliente->alugueres;
        }else{
          //Coloca o ultimo aluguer
          alugueresTmp->prox=tmpAluguer;
          //avança para o ultimo aluguer
          alugueresTmp=tmpAluguer;
        }
      }
    }
    //Verifica se é o primeiro cliente da lista
    if(listaClientes==NULL){
      listaClientes=tmpCliente;
      listaTmp=listaClientes;
    }else{
      //Não é preciso procurar por todos so clientes pois temos sempre o cliente anterior
      listaTmp->prox=tmpCliente;
      //Avança para o ultimo cliente
      listaTmp=tmpCliente;
    }
  }
  fclose(f);
  printf("Clientes importados com sucesso\n");
  return listaClientes;
}

void exportaDados(ptrCliente listaClientes,ptrGuitarra listaGuitarras, int total){
  FILE *f;
  //Exporta Guitarras
  f=fopen(NOME_FICHEIRO_GUITARRAS, "w");
  //Imprime guitarras
  for(int i=0; i<total; i++){
    fprintf(f, "%d %d %d %d %s\n", listaGuitarras[i].id, listaGuitarras[i].precoAluguerDia, listaGuitarras[i].valor, listaGuitarras[i].estado, listaGuitarras[i].nome);
  }
  fclose(f);

  //Exporta Clientes
  f=fopen(NOME_FICHEIRO_CLIENTES, "w");
  //Imprime clientes
  while (listaClientes!=NULL) {
    ptrAluguer tmpAluguer = listaClientes->alugueres;
    fprintf(f, "%d %d %s\n",listaClientes->nif, listaClientes->nAlugueresTotal, listaClientes->nome);
    while (tmpAluguer != NULL) {
      fprintf(f, "%d %d %d %d %d ",tmpAluguer->guitarra->id, tmpAluguer->estado, tmpAluguer->diaInicio, tmpAluguer->mesInicio, tmpAluguer->anoInicio);
      if(tmpAluguer->estado > 0)
        fprintf(f, "%d %d %d\n", tmpAluguer->diaEntrega, tmpAluguer->mesEntrega, tmpAluguer->anoEntrega);
      else
        fprintf(f, "\n");
      tmpAluguer=tmpAluguer->prox;
    }
    fprintf(f, "\n");
    listaClientes=listaClientes->prox;
  }
  fclose(f);

}
