#include "alugueres.h"
#include "guitarras.h"
#include <stdio.h>
#include <stdlib.h>

//TODO:ADICONAR UMA ID UNICOOO!!

void preencheGuitarra(ptrGuitarra guitarra, int total){
  guitarra->id = total;
  guitarra->estado = DISPONIVEL;
  printf("Introduza o nome da guitarra\n");
  scanf(" %[^\n]s", guitarra->nome);

  printf("Introduza o preco do aluguer por dia\n");
  scanf(" %d", &guitarra->precoAluguerDia);

  printf("Introduza o valor da guitarra\n");
  scanf(" %d", &guitarra->valor);
}

ptrGuitarra adicionaGuitarra(ptrGuitarra guitarras, int *total){
  ptrGuitarra tmp;
  tmp=realloc(guitarras, sizeof(guitarra)*(*total+1));
  if(tmp==NULL){
    printf("Erro de alocacao de memoria!!\n");
    return guitarras;
  }
  guitarras=tmp;
  preencheGuitarra(&guitarras[*total], *total);
  (*total)++;

  return guitarras;
}

void listarGuitarras(ptrGuitarra guitarras, int total){
  for(int i=0; i<total; i++){
    printf("Nome da guitarra: %s\n", guitarras[i].nome);
    printf("Estado: ");
    //Verifica se está indesponível
    if(guitarras[i].estado)
      guitarras[i].estado==ALUGADA ? printf("Alugada\n") : printf("Danificada\n");
    else
      printf("Disponivel\n");

    printf("ID da guitarra: %d\n", guitarras[i].id);
    printf("Preco do aluguer da guitarra por dia: %d\n", guitarras[i].precoAluguerDia);
    printf("Valor da Guitarra: %d\n\n\n", guitarras[i].valor);
  }
}

void listarGuitarrasDisponiveis(ptrGuitarra guitarras, int total){
  if(guitarras==NULL){
    printf("Não existem guitarras!!\n\n");
    return;
  }
  printf("Guitarras disponiveis:\n");
  for(int i=0; i<total; i++){
    if(guitarras[i].estado==DISPONIVEL){
      printf("Nome da guitarra: %s\n", guitarras[i].nome);
      printf("ID da guitarra: %d\n", guitarras[i].id);
      printf("Preco do aluguer da guitarra por dia: %d\n", guitarras[i].precoAluguerDia);
      printf("Valor da Guitarra: %d\n\n\n", guitarras[i].valor);
    }
  }
}

void listarGuitarrasAlugadas(ptrGuitarra guitarras, int total){
  if(guitarras==NULL){
    printf("Não existem guitarras!!\n\n");
    return;
  }
  printf("Guitarras Alugadas:\n");
  for(int i=0; i<total; i++){
    if(guitarras[i].estado==ALUGADA){
      printf("Nome da guitarra: %s\n", guitarras[i].nome);
      printf("ID da guitarra: %d\n", guitarras[i].id);
      printf("Preco do aluguer da guitarra por dia: %d\n", guitarras[i].precoAluguerDia);
      printf("Valor da Guitarra: %d\n\n\n", guitarras[i].valor);
    }
  }
}

//devolve -1 se não existir
int devolveIndexGuitarra(ptrGuitarra listaGuitarras, int total, int id){
  for(int i=0; i<total; i++){
    if(listaGuitarras[i].id==id)
      return i;
  }
  return -1;
}

int verificaGuitarrasDisponiveis(ptrGuitarra listaGuitarras, int total){
  int count=0;
  for(int i=0; i<total; i++){
    if(listaGuitarras[i].estado==DISPONIVEL)
      count++;
  }
  return count;
}
