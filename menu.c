#include "menu.h"
#include "funciones.h"

tConfig* detallesJuego(tConfig* config)
{
    char op;
    tConfig* elemento;

    printf("Niveles de dificultad.\n F - Facil\n M - Medio\n D - Dificil\n");

    do
    {
        printf("\nIngrese el nivel de dificultad: ");
        scanf("%c", &op);
        getchar();
        op = toupper(op);

        if (op != 'F' && op != 'M' && op != 'D') {
            printf("\nOpcion incorrecta\n");
        }
    }while(op != 'F' && op != 'M' && op != 'D');

    elemento = (tConfig*)(buscarDificultad(config, &op, sizeof(tConfig), 3, cmpDificultad));

    printf("\n-------------------------------------------\n");
    printf("Configuracion de la partida: \n");
    printf("Nivel: %c\n", elemento->nivel);
    printf("Tiempo que se muestra la secuencia: %i\n", elemento->tiempoSecuenciaEnPantalla);
    printf("Tiempo para contestar: %u\n", elemento->tiempoContestar);
    printf("Cantidad de vidas: %u\n", elemento->cantVidas);
    printf("Teclas validas: A, R, V, N y Z (Z es para retroceder)\n");
    printf("-------------------------------------------\n");
    system("pause");
    system("cls");

    return elemento;
}

void cargarOpcion(char* op)
{
    do
    {
        printf("\nIngrese una opcion: ");
        scanf("%c", op);
        getchar();

        if (*op != 'A' && *op != 'B') {
            printf("\nOpcion incorrecta\n");
        }
    }while(*op != 'A' && *op != 'B');
}

void switchMenu(const char opcion, void* recursos, tConfig* config)
{
    switch(opcion)
    {
    case 'A':
        juego(recursos, config);
        break;
    case 'B':
        system("cls");
        printf("Saliendo...\n");
        exit(1);
    }
}

void menuNuevo(char textoMenu[][MAX_TAM_TEXTO], unsigned cantidadDeRegistros, void* recursosMenu, tConfig* config, void selector(const char, void *, tConfig*))
{
    int i;
    char op;

    for(i=0; i < cantidadDeRegistros; i++)
    {
        printf("%s\n", *textoMenu);
        textoMenu++;
    }

    cargarOpcion(&op);

    selector(op, recursosMenu, config);
}

int cmpDificultad(const void* a, const void* b)
{
    return ((tConfig*)a)->nivel - *((char*)b);
}
