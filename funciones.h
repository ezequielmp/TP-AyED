#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <curl/curl.h>
#include "./Biblioteca/include/generico.h"
#include "config.h"
#include <string.h>
#include <windows.h>
#include <time.h>

#define URL "https://www.random.org/integers/?num=100&min=0&max=3&col=1&base=10&format=plain&rnd=new"
#define ERROR_CURL -1
#define CANT_BYTES 30
#define TAM 100
#define TAM_LINEA 100
#define TAM_VECTOR_CONFIG 3
///
#define TAM_NOMBRE_REPORTE_ARCHIVO 100
///
#define ERR_ARCH_CONFIG 0
///
#define ERR_ARCH_REPORTE 0
///
#define FORMATO_NOMBRE_ARCH_REPORTE "informe-juego_%Y-%m-%d-%H-%M.txt"
///
#define ERR_FORMATO_ARCH 0
#define ERR_FORMATO_ARCH_DUPLICADO 0
#define SALIR 0
#define NOMBRE_ARCH_CONFIG 1
#define PERDER 3
#define VIDAS 2
#include "./Biblioteca/include/listaSimple/listaSimple.h"

typedef struct
{
    char* buffer;
    size_t cantBytesCargados;
} tUserDataWriteCallback;

typedef struct
{
    int id;
    char nya[TAM];
    int puntajeJugador;
} tJugador;

typedef struct
{
    t_lista listaDeJugadores;
    unsigned cantidadDeJugadores;
    char secuencia[TAM];
} tRecursosMenu;

size_t WriteCallback(char* contents, size_t tamanyo, size_t nmemb, void *userp);
int iniciarCURL(CURL** curl);
void configurarCURL(CURL* curl, void* dato);
int ejecutarSolicitud(CURL* curl);

void mostrarJugador(const void* dato);
void juego(void* recursos, tConfig* config);
int pedirNumeros(const char *url, char *buffer, unsigned cant);

void crearLoteDePrueba (const char* nombreArch);
void mostrarSecuencia(char *secuencia, unsigned cant, unsigned milisegundos);
int leerArchivoConfig (const char* nombreArch, tConfig* vecConfig);
void trozarArchivoVariable (char* s, tConfig* d);
void rondas(void *recursos, tConfig *dificultad);
void mostrarCaracter(const void* dato);
int cmpConfigNivel (const void* a, const void* b);
int ingresarSecuencia(t_lista *ingresos, unsigned cantIngresos, unsigned milisegundos, char *secuencia, unsigned *cantidadIngresos);
unsigned ingresarVidas(unsigned cantUsable, unsigned vidasDisp);
void transformarCad(char *cad, char *vec, unsigned cantElem, unsigned cantVec);

///
void escribirArchivoReporte(FILE* fpArch, const char* nombreJugador, t_lista* listaJugadores, unsigned ronda, const char* secuenciaMostrada, t_lista* respuestaJugador,
                            unsigned puntajeObtenidoPorPregunta, unsigned cantVidasUsadas, int puntajeMaximo);
int crearNombreArchivoReporte (char* nombreArchReporte);

#endif // FUNCIONES_H_INCLUDED
