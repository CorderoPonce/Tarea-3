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

bool validarPrioridad(char *prioridadTemp){
  
  int largoCadena = strlen(prioridadTemp);
  if (largoCadena == 0) return false;
  for (int i = 0 ; i < largoCadena ; i++){
    if (isalpha(prioridadTemp[i])) return false;
  }
  return true;
}

int validarNombre(List *l, HashMap *m, char *nombre) {
  for (char *a = first(l) ; a != NULL ; a = next(l)) {
    if (strcmp(a, nombre) == 0) return 1;
  }
  
  int largoName = strlen(nombre); 
  if (largoName > 31 || largoName < 1) {
    return 2;
  }
  return 0;
}

void agregarTarea(List *l, HashMap *m){
  char nombre[31];
  puts("Ingrese el nombre de la tarea");
  scanf(" %[^\n]", nombre);

  int validacion;
  while (validacion != 0){
    validacion = validarNombre(l, m, nombre);
    
    if (validacion == 1) {
      puts("El nombre ingresado ya existe. Intente nuevamente");
      scanf(" %[^\n]", nombre);
    }
    if (validacion == 2) {
      puts("Ingrese un nombre válido (hasta 30 caracteres)");
      scanf(" %[^\n]", nombre);
    }
  }

  char prioridadTemp[31];
  puts("Ingrese la prioridad de la tarea");
  scanf(" %[^\n]", prioridadTemp);

  while (validarPrioridad(prioridadTemp) == false){
    puts("Se debe ingresar un número");
    scanf(" %[^\n]", prioridadTemp);
  }
  int prioridad = atoi(prioridadTemp);
  
  Tarea *n = (Tarea *) malloc (sizeof(Tarea));
  strcpy(n->nombre, nombre);
  n->prioridad = prioridad;
  n->numPrecedentes = 0;
  n->precedencia = createList();

  insertMap(m, n->nombre, n);
  //ListaEnOrden(l, m, n);
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
      agregarTarea(l, m);
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