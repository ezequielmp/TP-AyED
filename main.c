#include "funciones.h"

int main(int argc, char* argv[])
{
    CURL* curl;
    tUserDataWriteCallback dato;
    char* iniBuffer;
    tRecursosMenu recursos;
    tConfig vectorConfig[TAM_VECTOR_CONFIG];
    char textoMenuPrincipal[][MAX_TAM_TEXTO] =
    {
      "Cmon-dice:",
      "[A] Jugar.",
      "[B] Salida."
    };
    unsigned cantidadDeRegistros = sizeof(textoMenuPrincipal) / MAX_TAM_TEXTO;

    crearLoteDePrueba(argv[NOMBRE_ARCH_CONFIG]);

    crearListaSimple(&recursos.listaDeJugadores);

    dato.buffer = malloc(CANT_BYTES);
    iniBuffer = dato.buffer;
    if(NULL == dato.buffer)
    {
        fprintf(stderr,"No pude reservar memoria.");
        return -1;
    }

    if(!iniciarCURL(&curl))//RESERVO RECURSOS PARA REALIZAR CONFIGURACIONES DE REQUEST
    {
        free(iniBuffer);
        return -2;
    }

    //CONFIGURACION DE LOS RECURSOS RESERVADOS PARA HACER LA REQUEST
    configurarCURL(curl, &(dato.buffer));

    //SE HACE LA REQUEST Y SE REALIZA MANEJO DE ERRORES POR SI FALLA
    if(!ejecutarSolicitud(curl))
    {
        curl_easy_cleanup(curl);    //LIBERO RECURSOS RESERVADOS
        free(iniBuffer);
        return -3;
    }
    ///AGREGAR BARRA /0 AL FINAL
    fprintf(stdout, "%s", iniBuffer); //VERIFICACION VISUAL DE QUE SE RECIBIO CORRECTAMENTE LA INFO[Ya lo puedo usar como una cadena]
    system("pause");
    system("cls");

    //LEER ARCHIVO CONFIGURACIONES
    if(!leerArchivoConfig(argv[NOMBRE_ARCH_CONFIG], vectorConfig))
    {
        return ERR_ARCH_CONFIG;
    }

    menu(textoMenuPrincipal, cantidadDeRegistros, switchTextoMenu, &recursos, DESACTIVAR_AYUDA_AL_USUARIO);

    curl_easy_cleanup(curl);    //LIBERO RECURSOS RESERVADOS
    free(iniBuffer);
    return 0;
}
