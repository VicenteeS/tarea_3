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
void mostrarDetalles(HashMap *, char *linea);
void *get_csv_field(char * tmp, int k);


typedef struct{
  char nombre[50];
  char fecha[50];
  char valoracion[50];
  char precio[50];
}Videojuego;


int main(void) {
  
  int opcion, auxOpcionI, j, cont, contVideojuegos = 0;
  void* auxi;
  char *i, linea[50], auxOpcionS[50], listaVideojuegos[50][100];
  char nombre[50], fecha[50], valo[50], precio[50];
  
  Videojuego *v;
  FILE *file;
  
  HashMap* mapaJuegos = createMap(100);
  HashMap* mapaFechas = createMap(100);
  
  TreeMap* mapPrecio = createTreeMap(lower_than_string);
  TreeMap* mapValoracion = createTreeMap(lower_than_string);
  
  
  TreeMap *listaFec, *lAuxF;
  List *listP, *listAUXp, *listV, *listAUXv, *auxListas;
  PairTree *auxValoracion, *auxPrecio, *auxArbol;
  Pair *aux, *auxFec;
  
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
      case 1:
        
        file = fopen ("videojuegos.csv", "r");
        
        char line[1024];
        int l, contador;
        contador = 0;

        fgets(line, 1023, file);

        
        while(fgets(line, 1023, file) != NULL)
        { // Se lee la linea
          
          for(l=0 ; l<4 ; l++)
          {
            
            auxi = get_csv_field(line, l);
           
            switch (l)
            {
              case 0: 
                strcpy(nombre, auxi);
                contador++;
              break;
              case 1: 
                strcpy(fecha, auxi);
                contador++;
              break;
              case 2:
                strcpy(valo,auxi);
                contador++;
              break;
              case 3:
                strcpy(precio, auxi);
                contador++;
              break;
            }
           
            if(contador == 4)
            {
              
              contador = 0;
              
              v = (Videojuego*) malloc(sizeof(Videojuego));

              strcpy(v->nombre, nombre);
              strcpy(v->fecha, fecha);
              strcpy(v->valoracion, valo);
              strcpy(v->precio, precio);
            
          
              aux  = searchMap(mapaJuegos, nombre);
              
              if(aux == NULL)
              {
                
                strcpy(listaVideojuegos[contVideojuegos], v->nombre);
                contVideojuegos++;
                
                insertMap(mapaJuegos, v->nombre, v);
                
              
            
                //Ingresar por fecha (año)
                cont = 0;
                for(j = 6; j<=9; j++)
                {
                  strcpy(&linea[cont], &v->fecha[j]);
                  cont++;
                }
                if(searchMap(mapaFechas, linea) == NULL)
                {
                  listaFec = createTreeMap(lower_than_string);
                  insertTreeMap(listaFec, v->valoracion, v->nombre);
    
                  insertMap(mapaFechas, linea, listaFec);
                }
                else
                {
                  auxFec = searchMap(mapaFechas, linea);
                  lAuxF = auxFec->value;
                
                  insertTreeMap(lAuxF, v->valoracion, v->nombre);

                  auxFec->value = lAuxF;
                }
          //Ingresar por precio
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

              //Ingresar por valoración
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
                //Reseteo para poder ingresar valores nuevo y que no se acumulen
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
            }
          }
        }    
        printf("~Archivo importado correctamente~\n");
      break;
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
          strcpy(listaVideojuegos[contVideojuegos] , v->nombre);
          contVideojuegos++;
          
          insertMap(mapaJuegos, v->nombre, v);

          //Ingresar por fecha (año)
          cont = 0;
          //fecha dd/mm/aaaa
          for(j = 6; j<=9; j++)
          {
            strcpy(&linea[cont], &v->fecha[j]);
            cont++;
          }
          if(searchMap(mapaFechas, linea) == NULL)
          {
            listaFec = createTreeMap(lower_than_string);
            insertTreeMap(listaFec, v->valoracion, v->nombre);

            insertMap(mapaFechas, linea, listaFec);
          }
          else
          {
            auxFec = searchMap(mapaFechas, linea);
            lAuxF = auxFec->value;
            
            insertTreeMap(lAuxF, v->valoracion, v->nombre);

            auxFec->value = lAuxF;
          }

          
          //Ingresar por precio
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
          
          
          //Ingresar por valoración
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
          //Reseteo para poder ingresar valores nuevo y que no se acumulen
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
        //Recorremos el árbol y si la opción ingresada es 0 lo mostramos de menor a mayor, en           caso que la opción ingresada sea 1 creamos una lista nueva y con pushfront la                 ordenamos de mayor a menor y luego lo recorremos para mostrarlo
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
        //Reseteo para poder ingresar valores nuevo y que no se acumulen
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


        //Utilizamos upperBound para buscar el dato y si no lo encontramos buscamos una clave           mayor, recorremos la lista y buscamos en el mapa para luego mostrar por pantalla
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
        //Reseteo para poder ingresar valores nuevo y que no se acumulen
        listAUXv = NULL;
        auxArbol = NULL;
      break;
      case 5:
        printf("Ingrese un año: \n");
        fflush(stdin);
        fgets(linea,30,stdin);
        strcpy(linea, quitarSalto(linea));
        printf("\n");
        
        printf("Los juegos mas valorados del año %s son:\n", (char *)linea);
// se busca en el mapa fecha, mientras haya un dato se recorre el mapa y se inserta, luego se recorre 3 veces y muestra los 3 mejores rankeados por año
        aux = searchMap(mapaFechas, linea);

        if(aux != NULL) 
        {
          lAuxF = aux->value;
          for(auxArbol = firstTreeMap(lAuxF); auxArbol != NULL; auxArbol = nextTreeMap(lAuxF))
          {
            if(auxListas == NULL)
            {
              auxListas = createList();
              pushFront(auxListas, (auxArbol));
            }
            else
            {
              pushFront(auxListas, auxArbol);
            }
             //printf("%s: %s\n", (char *)(auxArbol->value), (char *)(auxArbol->key));
          }
        }
        auxArbol = firstList(auxListas);

        auxFec = searchMap(mapaJuegos, auxArbol->value);
        v = auxFec->value;
        for(j = 0; j < 3 ; j++ )
        {
          if(v != NULL)
          {
            printf("%s: %s\n", (char *)(v->nombre), (char *)(v->valoracion));
          }
          
          auxArbol = nextList(auxListas);
          if(auxArbol == NULL)
          {
            break;
          }
          auxFec = searchMap(mapaJuegos, auxArbol->value);
          v = auxFec->value;
        }
        
        printf("\nJuegos de este año mostrados\n");
        
        //Reseteo para poder ingresar valores nuevo y que no se acumulen
        lAuxF = NULL;
        auxArbol = NULL;
        auxListas  = NULL;
        auxFec = NULL;
        
      break;
      case 6:
        printf("Ingrese el nombre del juego que desee: \n");

        fflush(stdin);
        fgets(linea,30,stdin);
        strcpy(linea, quitarSalto(linea));
        printf("\n");

        printf("Los detalles del juego son los siguientes: \n");
        mostrarDetalles(mapaJuegos, linea);
        /*
        printf("Modificar un dato del videojuego, ingrese 0: \n");
        printf("Eliminar el videojuego, ingrese 1: \n");

        fflush(stdin);
        scanf("%i",&auxOpcionI);
        getchar(); 
        printf("\n");

        if(auxOpcionI == 0)
        {
          
        }
        else
        {
          if(auxOpcionI == 1)
          {
            eraseMap(mapaJuegos,  linea);
            eraseMap(mapaFechas, linea);
  HashMap* mapaFechas = createMap(100);
  
  TreeMap* mapPrecio = createTreeMap(lower_than_string);
  TreeMap* mapValoracion = createTreeMap(lower_than_string);
          }
      
        }*/
      break;
      case 7:
        
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
//Funcion utilizada para quitar el salto de línea agregado en el fgets.
char* quitarSalto(char* linea)
{
  if ((strlen(linea) > 0) && (linea[strlen(linea) - 1] == '\n'))
  {
    linea[strlen(linea) - 1] = '\0';
  }
  return(linea);
}
//Función del menú
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

void mostrarDetalles(HashMap *mapaJuegos, char* linea)
{
  Pair *aux;
  Videojuego *v;

  aux = searchMap(mapaJuegos, linea);
  v = aux->value;
  
  printf("\nNombre: %s", v->nombre);
  printf("\nFecha: %s", v->fecha);
  printf("\nValoracion: %s", v->valoracion);
  printf("\nPrecio: $%s", v->precio);
  printf("\n");
  return;
}


void * get_csv_field (char * tmp, int k) 
{
  int open_mark = 0;
  char* ret=(char*) malloc (100*sizeof(char));
  int ini_i=0, i=0;
  int j=0;
  while(tmp[i+1]!='\0')
  {
    if(tmp[i]== '\"')
    {
      open_mark = 1-open_mark;
      if(open_mark) ini_i = i+1;
      i++;
      continue;
    }
    if(open_mark || tmp[i]!= ',')
    {
      if(k==j) ret[i-ini_i] = tmp[i];
      i++;
      continue;
    }
    if(tmp[i]== ',')
    {
      if(k==j)
      {
        ret[i-ini_i] = 0;
        return ret;
      }
      j++; ini_i = i+1;
    }
    i++;
  }
  if(k==j) 
  {
    ret[i-ini_i] = 0;
    return ret;
  }
  return NULL;
}

