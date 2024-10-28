#include "funciones.h"
#include "menu.h"
/// COMENTARIOS
/**
 FALTA PONER UN TOLOWER O TOUPPER EN LA FUNCION DE INGRESAR OPCION
 EN FUNCION BUSCARCLAVEVECTOR EN LA LINEA 17 HAY QUE PONER NO_ENCONTRADO
 */

int main(int argc, char* argv[])
{
    tRecursosMenu recursos;
    tConfig vectorConfig[TAM_VECTOR_CONFIG];
    char textoMenuPrincipal[][MAX_TAM_TEXTO] =
    {
      "Cmon-dice:",
      "[A] Jugar.",
      "[B] Salida."
    };
    unsigned cantidadDeRegistros = sizeof(textoMenuPrincipal) / MAX_TAM_TEXTO;

    crearLoteDePrueba(*(argv + NOMBRE_ARCH_CONFIG));
    crearListaSimple(&recursos.listaDeJugadores);

    //LEER ARCHIVO CONFIGURACIONES
    if(!leerArchivoConfig(*(argv + NOMBRE_ARCH_CONFIG), vectorConfig))
    {
        return ERR_ARCH_CONFIG;
    }

    menuNuevo(textoMenuPrincipal, cantidadDeRegistros, &recursos, vectorConfig, switchMenu);
    //menu(textoMenuPrincipal, cantidadDeRegistros, switchTextoMenu, &recursos, DESACTIVAR_AYUDA_AL_USUARIO);
    return 0;
}
