#include "hashmap.h"
#include "list.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BARRA "---------------------------------"

typedef struct Tarea{
  int prioridad;
  char nombre[31];
  List *precedencia;
  int numPrecedentes;
} Tarea;

void mostrarMenu(){
  puts(BARRA);
  puts("        MENÚ DE OPCIONES");
  puts(BARRA);
  printf("Seleccione una opción:\n\n1. Agregar tarea\n2. Ingresar tarea precedente\n3. Mostrar tareas\n4. Marcar tarea como completada\n0. Salir\n");
  puts(BARRA);
}

int main(){
  int num;
  List *l = createList();
  HashMap *m = createMap(100);

  while(true) {
    mostrarMenu();

    if ((m->size/m->capacity)*100 > 70){
      enlarge(m);
    }
    
    scanf("%i", &num);
    while (num > 6 || num < 0){
      printf("Ingrese un número válido\n");
      scanf("%i", &num);
    }

    if (num == 0){
      puts(BARRA);
      printf("        FIN DEL PROGRAMA\n");
      puts(BARRA);
      free(m);
      return 0;
    }
    if (num == 1){
      //agregarTarea(l, m);
      puts("* Tarea agregada con éxito");
    }
    if (num == 2){
      //establecerPrecedencia(l, m);
      puts("* Tarea precendete agragada con éxito");
    }
    if (num == 3){
      //mostrarTareas(l, m);
    }
    if (num == 4){
      //marcarTarea(l, m);
    }
    if (num == 5){
      //mostrarLista(l);
    }
    if (num == 6){
      //mostrarMapa(m);
    }
  }
}