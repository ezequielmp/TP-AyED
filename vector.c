#include "vector.h"

int insertarAlFinalVector (void* v, const void* d, int ce, size_t tam)
{
    void* ult = v + ce * tam;

    memcpy(ult, d, tam);
    return TODO_OK;
}

int buscarPorClaveVector (void* v, const void* d, int ce, size_t tam, int (*cmp)(const void* x, const void* y))
{
    void* i = v;
    void* ult = v + (ce - 1) * tam;

    if(ce == 0)
        return NO_ENCONTRADO;

    while(i <= ult && cmp(d, i) != 0)
        i += tam;

    if(i <= ult && cmp(d, i) == 0)
        return DUPLICADO;

    return NO_DUPLICADO;
}

void* buscarDificultad(void* v, const void* dato, size_t tam, unsigned ce, int(*cmp)(const void* a, const void* b))
{
    int i;
    void* elemento = v;

    for(i=0; i<ce; i++)
    {
        if(cmp(elemento, dato) == 0)
            return elemento;
        elemento+=tam;
    }
    return NULL;
}
