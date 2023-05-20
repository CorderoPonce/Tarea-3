#include "hashmap.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#define BARRA "------------------------------------------"

// Se define la estructura tarea en la que se guardarán los datos que se ingresen.
typedef struct Tarea{
  int prioridad;
  char nombre[31];
  List *precedencia; // Lista de tareas precedentes.
  int numPrecedentes; // Contador de tareas precedentes.
} Tarea;

// Función que muestra el menú de opciones.
void mostrarMenu(){
  puts(BARRA);
  puts("             MENÚ DE OPCIONES");
  puts(BARRA);
  printf("Seleccione una opción:\n\n1. Agregar tarea\n2. Ingresar tarea precedente\n3. Mostrar tareas\n4. Marcar tarea como completada\n0. Salir\n");
  puts(BARRA);
}

// Función anexa a agregarTarea que valida la prioridad de la tarea que se esta ingresando.
bool validarPrioridad(char *prioridadTemp){ // Recibe la prioridad como cadena para evitar errores.
  
  int largoCadena = strlen(prioridadTemp);
  if (largoCadena == 0) return false;
  for (int i = 0 ; i < largoCadena ; i++){ // Se recorre la cadena para ver si hay algún caracter que no sea número.
    if (isdigit(prioridadTemp[i]) == 0) return false;
  }
  return true; // Si no cayó en los casos de prueba previos, se retorna que la prioridad ingresada si es válida.
}

// Función anexa a agregarTarea que valida el nombre que se le quiere asignar a la tarea que se esta ingresando.
int validarNombre(List *l, HashMap *m, char *nombre) { // Recibe la lista de nombres, el mapa y el nombre que se quiere asignar a la tarea.
  for (char *a = first(l) ; a != NULL ; a = next(l)) {
    if (strcmp(a, nombre) == 0) return 1; // En caso de existir una tarea previamente ingresada con el mismo nombre que se quiere ingresar la nueva tarea, se retorna 1, que indicará a la función que recibe, el motivo del return.
  }
  
  int largoName = strlen(nombre);
  if (largoName > 30 || largoName < 1) {
    return 2; // Si el largo del nombre es mayor que 30 o menor que 1, la función retorna 2, que indicará a la función que recibe el motivo del return.
  }
  return 0; // Si no cayó en las pruebas previas, se retorna 0, que indica que el nombre que se quiere asignar a la nueva tarea es válido.
}

// Esta función se encarga de ordenar los valores en las listas de nombres y de precedencia (aunque ambas almacenarán sólo datos de tipo char) según prioridad y cantidad de tareas precedentes.
void ListaEnOrden(List *l, HashMap *m, Tarea *n) {
  if (l->size == 0) { // Si la lista a la que se le quiere ingresar un nuevo dato está vacía, se ingresa el nombre de la tarea y se retorna.
    pushBack(l, n->nombre);
    return;
  }

  char *compare = first(l);
  // Este for() se encarga de recorrer la lista a la que se le quiere ingresar un nuevo dato.
  for (char *current = first(l) ; current != NULL ; current = next(l)){
    Tarea * c = searchMap(m, current);
    if (c->numPrecedentes > n->numPrecedentes && strcmp(c->nombre, compare) == 0){ // Si el número de tareas precedentes del dato que se recorre es mayor que el de la tarea que se quiere ingresar y además es el primer dato de la lista, se ingresa el dato al inicio.
      pushFront(l, n->nombre);
      return;
    }
    if (c->numPrecedentes > n->numPrecedentes){ // En caso de que el número de tareas precedentes de la tarea que está recorriendo el for sea mayor al de la nueva tarea que queremos ingresar, significa que nos pasamos de la posición en la que debería ingresarse, por lo que se vuelve una posición atrás con prev() y se ingresa el nombre de la nueva tarea en la posición con pushCurrent.
      prev(l);
      pushCurrent(l, n->nombre);
      return;
    }
    if (c->numPrecedentes == n->numPrecedentes && c->prioridad >= n->prioridad){ // Si el número de tareas precedentes entre la nueva tarea y la tarea que está recorriendo el for son iguales y a su vez, la prioridad de la nueva tarea es menor o igual a la de la tarea que se esta recorriendo, se ingresa la nueva tarea en la posición.
      prev(l);
      pushCurrent(l, n->nombre);
      return;
    }
  }
  pushBack(l, n->nombre);
}

// Función que elimina un dato de una lista en base a la lista y nombre que recibe en el argumento.
bool deleteList(List *l, HashMap *m, char *nombre){
  bool elimino = false; // Se crea el booleano elimino y se inicializa en false, éste indicará si durante el uso de la función se eliminó algún dato.
  for (char *a = first(l) ; a != NULL ; a = next(l)){
    if (strcmp(a, nombre) == 0){ // Si encuentra coincidencias entre el dato de la lista y el nombre recibido lo elimina de la lista con popCurrent y actualiza al booleano elimino en true. 
      popCurrent(l);
      elimino = true;
    }
  }
  if (elimino == true) return true; // Si se eliminó algún dato durante las iteraciones, la función retorna true.
  else return false; // Si no se eliminó ningún dato durante las iteraciones, la función retorna false.
}

// Función que elimina un dato dentro de la listas de precedencia de las tareas en base al nombre que recibe en el argumento.
void deletePrece(List *l, HashMap *m, char *nombre) {
  Tarea *casilla;
  long current;

  for (Pair *a = firstMap(m); a != NULL; a = nextMap(m)) {
    current = m->current; // Se guarda el current del mapa en una variable nueva, ya que al usar searchMap en funciones posteriores este índice se altera, por lo que se usa al final de cada iteración para retornar m->current a la posición en la que estaba.
    casilla = a->value;

    if (casilla->numPrecedentes > 0) { // Se comprueba que el dato que se está recorriendo posea lista de tareas precedentes.
      if (deleteList(casilla->precedencia, m, nombre) == true) { // Se buscará en la lista de precedentes coincidencias con el nombre a eliminar, en caso de haberse eliminado un dato de la lista de precedencia se elimina el dato en la lista de nombres para reingresarse pero en el nuevo orden que debería estar.
        casilla->numPrecedentes--;
        deleteList(l, m, casilla->nombre);
        ListaEnOrden(l, m, casilla);
        m->current = current;
      }
    }
  }
}

// Función anexa a marcarTarea que comprueba si existen relaciones de precedencias entre el nombre de la tarea buscada y las demás tareas.
bool buscarRelacionPre(List *l, HashMap *m, char *nombre){
  for (Pair *a = firstMap(m) ; a != NULL ; a = nextMap(m)){
    Tarea *casilla = a->value;
    for (char *b = first(casilla->precedencia) ; b != NULL ; b = next(casilla->precedencia)){ // Se recorre la lista de precedencias de la tarea.
      if (strcmp(b, nombre) == 0) return true; // Si se encuentra el nombre buscado en la lista de tareas precedentes de alguna tarea, se retorna true indicando que existen relaciones de precedencia.
    }
  }
  return false; // Se retorna falso si no se encontraron redes de precedencia entre la tarea buscada y las demás.
}

// Función que agrega tareas.
void agregarTarea(List *l, HashMap *m){
  char nombre[31];
  puts("Ingrese el nombre de la tarea");
  scanf(" %[^\n]", nombre);

  int validacion;
  while (validacion != 0){ // Se comienza una iteración que finaliza cuando validarNombre supere todas las pruebas, es decir, el nombre ingresado sea válido.
    validacion = validarNombre(l, m, nombre);
    if (validacion == 1) { // Si validarNombre retorna 1 es porque el nombre ingresado es nombre de otra tarea previamente ingresada.
      puts("El nombre ingresado ya existe. Intente nuevamente");
      scanf(" %[^\n]", nombre);
    }
    if (validacion == 2) { // Si validarNombre retorna 2 es porque el largo del nombre es mayor de 30.
      puts("Ingrese un nombre válido (hasta 30 caracteres)");
      scanf(" %[^\n]", nombre);
    }
  }

  char prioridadTemp[31];
  puts("Ingrese la prioridad de la tarea");
  scanf(" %[^\n]", prioridadTemp); // Se lee la prioridad de la tarea en una cadena, para verificar la validez de lo ingresado en la función validarPrioridad.

  while (validarPrioridad(prioridadTemp) == false){ // Se itera mientras que la prioridad ingresada no sea un número válido.
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
  ListaEnOrden(l, m, n); // Se inserta en orden de prioridad a la lista de nombres de tareas.
}

// Función que establece relaciones de precedencia entre dos tareas.
void establecerPrecedencia(List *l, HashMap *m) {
  if (l->size == 0){ // En caso de no haberse ingresado tareas con las que trabajar se inhabilita el paso por la función, imprime un mensaje y retorna.
    puts("* No se han registrado tareas");
    return;
  }
  if (l->size == 1){ // Si no hay al menos dos tareas (una para agregarle tarea precedente y otra que se agregue) se imprime el mensaje y retorna.
    puts("* Se requieren de al menos dos tareas para esta funcionalidad (!)");
    return;
  }
  
  char tarea1[31];
  char tarea2[31];
  Tarea *casilla1;
  Tarea *casilla2;

  // Ciclo while infinito del que se sale si ambos nombres de tareas existen y no son iguales.
  while (true){
    puts("Ingrese la tarea a la que asignar:");
    scanf(" %[^\n]", tarea1);
  
    casilla1 = searchMap(m, tarea1);
    if (casilla1 == NULL) {
      puts("* La tarea ingresada no existe (!)");
      return;
    }

    puts("Ingrese el nombre de la tarea precedente:");
    scanf(" %[^\n]", tarea2);

    if (strcmp(tarea1, tarea2) == 0){ // Se comprueba si las dos tareas ingresadas no son la misma, en caso de haberse ingresado dos veces la misma tarea se imprime el mensaje y se vuelve a iterar.
      puts("* Se deben ingresar dos tareas diferentes (!)\n");
    }
    else {
      casilla2 = searchMap(m, tarea2);
      if (casilla2 == NULL) {
        puts("* La tarea ingresada no existe (!)");
        return;
      }
      break; // Si los nombres de tareas ingresados son válidos se rompe el ciclo while infinito.
    }
  }
  
  casilla1->numPrecedentes++; // Se aumenta el número de tareas precedentes de la primera tarea ingresada.
  ListaEnOrden(casilla1->precedencia, m, casilla2); // Se ingresa la segunda tarea a la lista de tareas precedentes de la primera tarea ingresada.
  deleteList(l, m, casilla1->nombre); // Se borra la primera tarea de la lista de nombres en orden, para reingresarla en el nuevo orden que debería estar.
  ListaEnOrden(l, m, casilla1);
  puts("* Tarea precedente agregada con éxito");
}

// Función que muestra todas las tareas en orden de prioridad y número de tareas precedentes.
void mostrarTareas(List *l, HashMap *m){
  if (l->size == 0){ // En caso de no haberse ingresado tareas con las que trabajar se inhabilita el paso por la función, imprime un mensaje y retorna.
    puts("* No se han registrado tareas");
    return;
  }
  int cont = 1;

  puts(BARRA);
  puts("Lista de tareas ordenadas:\n");
  for (char *a = first(l) ; a != NULL ; a = next(l)){ // Se recorre la lista de nombres que está en orden de prioridad y número de tareas precedentes.
    Tarea *casilla = searchMap(m, a);
    
    printf("%u. %s (Prioridad: %i)", cont, casilla->nombre, casilla->prioridad);

    if (casilla->numPrecedentes != 0){ // Se comprueba si la tarea tiene tareas precedentes para imprimir.
      char *b = first(casilla->precedencia);
      char *compare = first(casilla->precedencia); // Se crea esta variable comprobar si el dato que se quiere escribir es el primero para imprimirlo de determinada forma, usa una variable aparte para no utilizar la función first, porque altera el current y genera un bucle infinito.
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

void marcarTarea(List *l, HashMap *m){
  if (l->size == 0){ // En caso de no haberse ingresado tareas con las que trabajar se inhabilita el paso por la función, imprime un mensaje y retorna.
    puts("* No se han registrado tareas");
    return;
  }

  char nombre[31];
  puts("Ingrese el nombre de la tarea completada");
  scanf(" %[^\n]", nombre);

  Tarea *casilla = searchMap(m, nombre);
  if (casilla == NULL){
    puts("* La tarea ingresada no existe (!)");
    return;
  }

  if (casilla->numPrecedentes != 0 || buscarRelacionPre(l, m, casilla->nombre) == true){ // En caso de que la tarea que se quiere eliminar tenga relaciones de precedencia con otras tareas se imprimirá un mensaje de advertencia al usuario.
    int seleccion;
    puts("¿Está seguro de eliminar la tarea? (1 sí | 2 no)");
    scanf("%i", &seleccion);
    while (seleccion > 2 || seleccion < 1){
      puts("Ingrese un número válido");
      puts("¿Está seguro de eliminar la tarea? (1 sí | 2 no)");
      scanf("%i", &seleccion);
    }

    if (seleccion == 1){ // Si se seleccionó 1 en el menú de opciones (eliminar el dato):
      deletePrece(l, m, nombre); // Función que elimina el dato de listas de precedencia de otras tareas en caso de estar.
      deleteList(l, m, nombre); // Función que elimina el dato de la lista de nombres en orden.
      eraseMap(m, nombre); // Se elimina el dato del mapa.
      puts("* La tarea ha sido eliminada");
      return;
    }
    else{ // Si se seleccionó 2 en el menú de opciones (no eliminar el dato):
      puts("* La tarea NO ha sido eliminada");
      return;
    }
  }
  else { // Si la tarea no tiene relaciones de precedencia con otras tareas simplemente se elimina.
    deletePrece(l, m, nombre); // Función que elimina el dato de redes de precedencia de otras tareas en caso de estar.
    deleteList(l, m, nombre); // Función que elimina el dato de la lista de nombres en orden.
    eraseMap(m, nombre); // Se elimina el dato del mapa.
    puts("* La tarea ha sido eliminada");
    return;
  }
}

int main(){
  int num; // Variable que almacena la opción del usuario.
  List *l = createList(); // Se crea la lista de nombres, está almacenará los nombres de las variables que se vayan ingresando al mapa en orden de prioridad y numero de tareas precedentes que posea.
  HashMap *m = createMap(100);

  // Se comienza está iteración infinita que sólo se detendrá cuando el usuario ingrese 0 para salir.
  while(true) {
    mostrarMenu();

    // Se comprueba que la capacidad del mapa no sea superior al 70%, en caso de serlo, se agranda el tamaño del mapa.
    if ((m->size/m->capacity)*100 >= 70){
      enlarge(m);
    }

    scanf("%i", &num);
    while (num > 4 || num < 0){
      printf("Ingrese un número válido\n");
      scanf("%i", &num);
    }

    if (num == 0){
      puts(BARRA);
      printf("             FIN DEL PROGRAMA\n");
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
    }
    if (num == 3){
      mostrarTareas(l, m);
    }
    if (num == 4){
      marcarTarea(l, m);
    }
  }
}