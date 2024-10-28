#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <curl/curl.h>
#include "./Biblioteca/include/listaSimple/listaSimple.h"
#include "./Biblioteca/include/menu/menu.h"
#include "./Biblioteca/include/generico.h"

#define URL "https://www.random.org/integers/?num=10&min=0&max=3&col=1&base=10&format=plain&rnd=new"

#define CANT_BYTES 50
#define TAM 100
#define TAM_LINEA 100
#define TAM_VECTOR_CONFIG 3

#define TODO_OK 1
#define ERR_ARCH_CONFIG 0
#define ERR_FORMATO_ARCH 0
#define ERR_FORMATO_ARCH_DUPLICADO 0
#define DUPLICADO 0
#define NO_DUPLICADO 1

#define NOMBRE_ARCH_CONFIG 1

typedef struct
{
    char* buffer;
    size_t cantBytesCargados;
} tUserDataWriteCallback;

typedef struct
{
    int id;
    char nya[TAM];
} tJugador;

typedef struct
{
    t_lista listaDeJugadores;
    unsigned cantidadDeJugadores;
} tRecursosMenu;

typedef struct
{
    char nivel;
    unsigned int tiempoSecuenciaEnPantalla;
    unsigned int tiempoContestar;
    unsigned int cantVidas;
} tConfig;

size_t WriteCallback(char* contents, size_t tamanyo, size_t nmemb, void *userp);
int iniciarCURL(CURL** curl);
void configurarCURL(CURL* curl, void* dato);
int ejecutarSolicitud(CURL* curl);

void mostrarJugador(const void* dato);
void juego(void* recursos);
void switchTextoMenu(int opcion, void* recursosMenu);

void crearLoteDePrueba (const char* nombreArch);

int leerArchivoConfig (const char* nombreArch, tConfig* vecConfig);
void trozarArchivoVariable (char* s, tConfig* d);
int insertarAlFinalVector (void* v, const void* d, int ce, size_t tam);
int buscarPorClaveVector (void* v, const void* d, int ce, size_t tam, int (*cmp)(const void* x, const void* y));
int cmpConfigNivel (const void* a, const void* b);

#endif // FUNCIONES_H_INCLUDED
