#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED
#include <stdlib.h>
#include <string.h>
#define DUPLICADO 0
#define NO_DUPLICADO 1
#define NO_ENCONTRADO 0
#define TODO_OK 1

int insertarAlFinalVector (void* v, const void* d, int ce, size_t tam);
int buscarPorClaveVector (void* v, const void* d, int ce, size_t tam, int (*cmp)(const void* x, const void* y));

#endif // VECTOR_H_INCLUDED