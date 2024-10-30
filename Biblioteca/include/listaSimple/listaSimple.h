#ifndef LISTASIMPLE_H_INCLUDED
#define LISTASIMPLE_H_INCLUDED

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <time.h>
#define NO_PUDE_INSERTAR 0
#define NO_HAY_ELEMENTOS 0
#define NO_ENCONTRADO 0
#define OK 1
#define NO_MEM 0

#define MENOR(X,Y) (((X)>(Y))? (Y):(X))

typedef struct s_nodo
{
    struct s_nodo* sig;
    void* dato;
    unsigned tam;
}t_nodo;

typedef t_nodo* t_lista;


void crearListaSimple(t_lista* pl);
int insertarAlFinalEnListaSimple(t_lista* pl, const void* dato, unsigned tam);
int sacarPrimeroEnListaSimple(t_lista* pl, void* dato, unsigned tam);
void mostrarListaSimpleEnOrden(const t_lista* pl, void (*mostrar)(const void* dato));
void vaciarListaSimple(t_lista *pl);
int listaSimpleVacia(const t_lista* pl);
//int insertarOrdenadoSinDuplicados(t_lista* pl, const void* dato, unsigned tam, int (*comparar)(const void* a, const void* b));
int insertarOrdenadoEnListaSimple(t_lista* pl, const void* dato, unsigned tam, int (*comparar)(const void* a, const void* b));
int insertarPrimeroEnListaSimple(t_lista *pl, const void *dato, unsigned tam);
void insertarArchivoBinarioEnListaSimple(FILE* pf, t_lista* pl, void* dato, unsigned tam, int (*comparar)(const void* a, const void* b));
int buscarPorIndiceEnListaSimple(t_lista *pl, void *dato, unsigned tam, unsigned indice);
void swapDeNodos(t_nodo** nodo1, t_nodo** nodo2);
t_nodo** direccionDeSigNodoAnteriorAlAzar(t_nodo** base, unsigned cantidadDeElementos);
void mezclarListaSimpleAleatoriamente(t_lista* pl, unsigned cantidadDeElementos);
void mapInversoListaSimpleParam(const t_lista* pl, void accion(const void*, void*), const void* param1);
void dejarSoloClave(t_lista *pl, const void *dato, int cmp(const void*, const void*));
int ActualizarPorIndiceEnListaSimple(t_lista *pl, void *dato, unsigned tam, unsigned indice);
void mapInversoListaSimple(const t_lista* pl, void accion(const void*));
void mapListaSimpleParam(const t_lista* pl, void accion(const void*, void*), const void* param);

#endif // LISTASIMPLE_H_INCLUDED
