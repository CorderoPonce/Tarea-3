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

void ListaEnOrden(List *l, HashMap *m, Tarea *n) {
  if (l->size == 0) {
    pushBack(l, n->nombre);
    return;
  }

  void *current = first(l);
  Tarea *c = searchMap(m, current);
  
  if (n->numPrecedentes > 0) { // SI TIENE PRECEDENCIA
    while (current != NULL) {
      if (c->numPrecedentes == n->numPrecedentes && n->prioridad <= c->prioridad) {
        prev(l);
        pushCurrent(l, n->nombre);
        return;
      }
      else {
        if (c->numPrecedentes > n->numPrecedentes) {
        prev(l);
        pushCurrent(l, n->nombre);
        return;
        }
      }
      current = next(l);
      if (current != NULL) c = searchMap(m, current);
    }
    pushBack(l, n->nombre);
  }
  else { // SI NO TIENE PRECEDENCIA
    if (n->prioridad <= c->prioridad &&  c->numPrecedentes == 0){
    pushFront(l, n->nombre);
    return;
    }

    while (current != NULL && c->prioridad < n->prioridad && c->numPrecedentes == 0) {
      current = next(l);
      if (current != NULL) c = searchMap(m, current);
    }

    if (current == NULL) {
      pushBack(l, n->nombre);
    } else {
      prev(l);
      pushCurrent(l, n->nombre);
    } 
  }
}

bool deleteList(List *l, HashMap *m, char *nombre){
  bool elimino = false;
  for (char *a = first(l) ; a != NULL ; a = next(l)){
    if (strcmp(a, nombre) == 0){
      popCurrent(l);
      elimino = true;
    }
  }
  if (elimino == true) return true;
  else return false;
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
  ListaEnOrden(l, m, n);
}

void establecerPrecedencia(List *l, HashMap *m) {
  if (l->size == 0){
    puts("* No se han registrado tareas");
    return;
  }
  if (l->size == 1){
    puts("* Se ha ingresado sólo una tarea");
    return;
  }
  
  char tarea1[31];
  char tarea2[31];

  puts("Ingrese la tarea a la que asignar:");
  scanf(" %[^\n]", tarea1);
  
  Tarea *casilla1 = searchMap(m, tarea1);
  if (casilla1 == NULL) {
    puts("* La tarea ingresada no existe!");
    return;
  }

  puts("Ingrese el nombre de la tarea precedente:");
  scanf(" %[^\n]", tarea2);
  
  Tarea *casilla2 = searchMap(m, tarea2);
  if (casilla2 == NULL) {
    puts("* La tarea ingresada no existe!");
    return;
  }
  
  casilla1->numPrecedentes++;
  ListaEnOrden(casilla1->precedencia, m, casilla2);
  deleteList(l, m, casilla1->nombre);
  ListaEnOrden(l, m, casilla1);
}

void mostrarTareas(List *l, HashMap *m){
  if (l->size == 0){
    puts("* No se han registrado tareas");
    return;
  }
  int cont = 1;

  puts(BARRA);
  puts("Lista de tareas ordenadas:\n");
  for (char *a = first(l) ; a != NULL ; a = next(l)){
    Tarea *casilla = searchMap(m, a);
    
    printf("%u. %s (Prioridad: %i)", cont, casilla->nombre, casilla->prioridad);

    if (first(casilla->precedencia) != NULL){
      char *b = first(casilla->precedencia);
      char *compare = first(casilla->precedencia);
      printf(" - Precedente: ");
      while (b != NULL){
        if (strcmp(b, compare) == 0){
          printf("%s", b);
        }
        else printf(" - %s", b);
        b = next(casilla->precedencia);
      }  
    }
    printf("\n");
    cont++;
  }
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
      establecerPrecedencia(l, m);
      puts("* Tarea precendete agragada con éxito");
    }
    if (num == 3){
      mostrarTareas(l, m);
    }
    if (num == 4){
      //marcarTarea(l, m);
    }
  }
}