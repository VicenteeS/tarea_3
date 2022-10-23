#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"
#include "list.h"
#include "treemap.h"

int lower_than_string(void* , void*);
char* quitarSalto(char* linea);
void mostrarMenu();

typedef struct{
  char nombre[30];
  char fecha[30];
  char valoracion[30];
  char precio[30];
}Videojuego;


int main(void) {
  int opcion, auxOpcionI, j, cont;
  
  Videojuego *v;

  char *i, linea[30], *auxOpcionS;
  
  HashMap* mapaJuegos = createMap(100);
  HashMap* mapaFechas = createMap(100);
  List *listP, *listAUXp, *listV, *listAUXv, *auxListas, *listaFec, *lAuxF;
  PairTree *auxValoracion, *auxPrecio, *auxArbol;
  Pair *aux, *auxFec;
  
  
  TreeMap* mapPrecio = createTreeMap(lower_than_string);
  TreeMap* mapValoracion = createTreeMap(lower_than_string);

  auxListas = NULL;
  
  while(opcion!=0)
  {
    do{
      /*Menú que muestra en pantalla las opciones que podemos 
      realizar*/
     mostrarMenu();
  
      fflush(stdin);
      scanf("%i",&opcion);
      getchar(); 
      printf("\n");
    }while (opcion>10 && opcion<1);
  
    switch(opcion)
    {
      case 2://agregar
        v = (Videojuego*) malloc(sizeof(Videojuego));

        
        printf("Ingrese nombre del videojuego: ");
        fgets(v->nombre,30,stdin);
        strcpy(v->nombre, quitarSalto(v->nombre));
        
        printf("Ingrese año de salida: ");
        fgets(v->fecha,30,stdin);
        strcpy(v->fecha, quitarSalto(v->fecha));
        
        printf("Ingrese valoración del videojuego: ");
        fgets(v->valoracion, 30,stdin);
        strcpy(v->valoracion, quitarSalto(v->valoracion));
        
        printf("Ingrese precio del videojuego: ");
        fgets(v->precio, 30,stdin);
        strcpy(v->precio, quitarSalto(v->precio));


        
        aux  = searchMap(mapaJuegos, v->nombre);
        if(aux == NULL)
        {
          insertMap(mapaJuegos, v->nombre, v);
          
          cont = 0;
          //fecha dd/mm/aaaa
          for(j = 6; j<=9; j++)
          {
            strcpy(&linea[cont], &v->fecha[j]);
            cont++;
          }
          if(searchMap(mapaFechas, linea) == NULL)
          {
            listaFec = createList();
            pushFront(listaFec, v->nombre);

            insertMap(mapaFechas, linea, listaFec);
          }
          else
          {
            auxFec = searchMap(mapaFechas, linea);
            lAuxF = auxFec->value;

            pushFront(lAuxF, v->nombre);

            auxFec->value = lAuxF;
          }
          //Ingresar por Precio
          if(searchTreeMap(mapPrecio, v->precio) == NULL)
          {
           
            listP = createList();
            pushFront(listP, v->nombre);
            
            insertTreeMap(mapPrecio, v->precio, listP);
          }
          else
          {
            auxPrecio = searchTreeMap(mapPrecio, v->precio);
            
            listAUXp = auxPrecio->value;
            pushFront(listAUXp, v->nombre);
            
            auxPrecio->value = listAUXp;
          }
          
          
          
          if(searchTreeMap(mapValoracion, v->valoracion) == NULL)
          {
            listV = createList();
            pushFront(listV, v->nombre);
            
            insertTreeMap(mapValoracion, v->valoracion, listV);
          }
          else
          {
            auxValoracion = searchTreeMap(mapValoracion, v->valoracion);
            
            listAUXv = auxValoracion->value;
            pushFront(listAUXv, v->nombre);
            
            auxValoracion->value = listAUXv;
          }
          
          listAUXv = NULL;
          listAUXp = NULL;
          auxValoracion = NULL; 
          auxPrecio = NULL;
          listV = NULL;
          listP = NULL;
          auxFec = NULL;
          lAuxF = NULL;
          listaFec = NULL;
          v = NULL;
        } 
      break;
      case 3:
        printf("Mostrar de MENOR A MAYOR, ingrese: 0\n");
        printf("Mostrar de MAYOR A MENOR, ingrese: 1\n");
          
        fflush(stdin);
        scanf("%i",&auxOpcionI);
        getchar(); 
        printf("\n");
        
        for(auxArbol = firstTreeMap(mapPrecio); auxArbol != NULL; auxArbol = nextTreeMap(mapPrecio))
        {
          listAUXp = auxArbol->value;
          if(auxOpcionI == 0)
          {
            for(i = firstList(listAUXp); i != NULL; i = nextList(listAUXp))
            {
              aux  = searchMap(mapaJuegos, i);
              if(aux != NULL)
              {
                v = aux->value;
              
                printf("%s: %s\n", (char *)i, (char *)v->precio);
              }
            }
          }
          else 
          {
            if(auxOpcionI == 1)
            {
              for(i = firstList(listAUXp); i != NULL; i = nextList(listAUXp))
              {
                 
                if(auxListas == NULL)
                {
                  auxListas = createList();
                  pushFront(auxListas, i);
                }
                else
                {
                  pushFront(auxListas, i);
                }
              }
            }
          }
        }
        if(auxOpcionI == 1)
        {
          for(i = firstList(auxListas); i != NULL; i = nextList(auxListas))
          {
            aux  = searchMap(mapaJuegos, i);
            if(aux != NULL)
            {

              v = aux->value;

              printf("%s: %s\n", (char *)i, (char *)v->precio);
            }
          }
        }
        printf("\n***Datos Mostrados***\n");
        auxArbol = NULL;
        auxListas = NULL;
        listAUXp = NULL;  
      break;
      case 4:
        printf("Ingrese la valoración mínima: \n");
          
        fflush(stdin);
        fgets(auxOpcionS,30,stdin);
        strcpy(auxOpcionS, quitarSalto(auxOpcionS));
        printf("\n");


        
        for(auxArbol = upperBound(mapValoracion, auxOpcionS); auxArbol != NULL; auxArbol = nextTreeMap(mapValoracion))
        {
          listAUXv = auxArbol->value;
          for(i = firstList(listAUXv); i != NULL; i = nextList(listAUXv))
          {
            aux  = searchMap(mapaJuegos, i);
            if(aux != NULL)
            {
              
              v = aux->value;
              
              printf("%s: %s\n", (char *)i, (char *)v->valoracion);
            }
          }
        }
        listAUXv = NULL;
        auxArbol = NULL;
      break;
      case 5:
        printf("Ingrese un año: \n");
        fflush(stdin);
        fgets(linea,30,stdin);
        strcpy(linea, quitarSalto(linea));

        printf("Los juegos del año %s son:\n", (char *)linea);
        aux = searchMap(mapaFechas, linea);
        if(aux != NULL)
        {
          for(i = firstList(aux->value); i != NULL; i = nextList(aux->value))
          {
            printf("%s\n", (char *)i);
          }
        }
        printf("Juegos de este año mostrados\n");
      break;
    }
  }

  
  return 0;
}

int lower_than_string(void* key1, void* key2)
{
    char* k1=(char*) key1;
    char* k2=(char*) key2;
    if(strcmp(k1,k2)<0) return 1;
    return 0;
}

char* quitarSalto(char* linea)
{
  if ((strlen(linea) > 0) && (linea[strlen(linea) - 1] == '\n'))
  {
    linea[strlen(linea) - 1] = '\0';
  }
  return(linea);
}

void mostrarMenu()
{
  printf("\n Introduzca una opción (1-9)");
  printf("\n 1.Importar archivo de juegos");
  printf("\n 2.Agregar juego");
  printf("\n 3.Mostrar juegos por precio");
  printf("\n 4.Filtrar juegos por valoración");
  printf("\n 5.Mostrar juegos del año");
  printf("\n 6.Buscar juego");
  printf("\n 7.Exportar datos");
  printf("\n 0.Salir");
  printf("\n");

  return;
}