#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include <stdlib.h>
#include "config.h"
#include "./Biblioteca/include/listaSimple/listaSimple.h"
#define MAX_TAM_TEXTO 101

/** Funciones nuevas, reemplazando por un nuevo menu **/
void* buscarDificultad(void* v, const void* dato, size_t tam, unsigned ce, int(*cmp)(const void* a, const void* b));
tConfig* detallesJuego(tConfig* config);
void cargarOpcion(char* op);
void switchMenu(const char opcion, void* recursos, tConfig* config);
void menu(char textoMenu[][MAX_TAM_TEXTO], unsigned cantidadDeRegistros, void* recursosMenu, tConfig* config, void selector(const char, void *, tConfig*), void cargarOpcion(char*));
int cmpDificultad(const void* a, const void* b);

#endif // MENU_H_INCLUDED
