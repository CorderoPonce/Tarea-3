#include <stdlib.h>
#include <stdio.h>

#ifndef _list_h
#define _list_h

typedef struct List List;
typedef struct Tarea Tarea;
typedef struct n node;
typedef List Stack;

struct List{
    node* first;
    node* last;
    node* current;
    int size;
};

typedef struct n{
    void* data;
    struct n* next;
    struct n* prev;
} node;

/* list operations */

List* createList();
void clean(List* list);
int is_empty(List* list);
void* front(List* list);
void* first(List* list);
void* next(List* list);
void* last(List* list);
void* prev(List* list);
void popFront(List* list);
void popBack(List* list);
void pushFront(List* list, void* data);
void pushBack(List* list, void* data);
void pushCurrent(List* list, void* data);
void popCurrent(List* list);
int get_size();


/* stack operations */
Stack* createStack();
void pop(Stack* s);
void* top(Stack* s);
void push(Stack* s, void* data);




#endif /* _list_h */
