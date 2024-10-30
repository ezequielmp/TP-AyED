#include "funciones.h"
#include "menu.h"
#include <unistd.h>
#define ARCHIVO_CONFIG "config.txt"

int main()
{
    tRecursosMenu recursos;
    tConfig vectorConfig[TAM_VECTOR_CONFIG];
    char textoMenuPrincipal[][MAX_TAM_TEXTO] =
    {
      "Cmon-dice:",
      "[A] Jugar.",
      "[B] Salida."
    };
    recursos.colores[0] = 'R';
    recursos.colores[1] = 'A';
    recursos.colores[2] = 'N';
    recursos.colores[3] = 'V';
    unsigned cantidadDeRegistros = sizeof(textoMenuPrincipal) / MAX_TAM_TEXTO;

    if(access(ARCHIVO_CONFIG, F_OK) != 0)
        crearLoteDePrueba(ARCHIVO_CONFIG);

    //LEER ARCHIVO CONFIGURACIONES

    crearListaSimple(&recursos.listaDeJugadores);

    if(!leerArchivoConfig(ARCHIVO_CONFIG, vectorConfig))
    {
        printf("El archivo de configuracion no contiene un formato valido. Revise el archivo README para mas informacion.");
        return ERR_ARCH_CONFIG;
    }
    menu(textoMenuPrincipal, cantidadDeRegistros, &recursos, vectorConfig, switchMenu, cargarOpcion);
    return 0;
}
