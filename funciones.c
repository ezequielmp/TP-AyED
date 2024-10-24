#include "funciones.h"

/** Funciones CURL **/

size_t WriteCallback(char* contents, size_t tamanyo, size_t nmemb, void *userp)
{
    size_t totalSize = tamanyo * nmemb;

    memcpy(((tUserDataWriteCallback*)userp)->buffer, contents, totalSize);

    ((tUserDataWriteCallback*)userp)->buffer += totalSize;
    ((tUserDataWriteCallback*)userp)->cantBytesCargados += totalSize;

    return totalSize;
}

int iniciarCURL(CURL** curl)
{
    *curl = curl_easy_init();
    if(NULL == *curl)
    {
        fprintf(stderr, "Fallo curl_easy_init.\n");
        return 0;
    }
    return TODO_OK;
}

void configurarCURL(CURL* curl, void* dato)
{
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_URL, URL);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 25L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 12L);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, dato);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
}

int ejecutarSolicitud(CURL* curl)
{
    CURLcode rEjecutarSolicitud;
    if(CURLE_OK != (rEjecutarSolicitud = curl_easy_perform(curl)))
    {
        fprintf(stderr, "Fallo la solicitud: %s.\n", curl_easy_strerror(rEjecutarSolicitud));
        return 0;
    }
    return TODO_OK;
}

/** Lectura y muestra de la info del jugador **/

void mostrarJugador(const void* dato)
{
    printf("id: %d\n Nombre: %s\n", ((tJugador*)dato)->id, ((tJugador*)dato)->nya);
}

int ingresoDeNombresAListaSimple(t_lista* listaDeJugadores, unsigned* cantidadDeJugadores)
{
    tJugador jugador;
    unsigned ingresoNombres = 1;

    jugador.id = 0;
    *cantidadDeJugadores = 0;

    system("cls");
    printf("INGRESO DE NOMBRES DE LOS JUGADORES\n");
    do
    {
        printf("Ingrese '-' para finalizar\n");
        printf("\nIngrese nombre ID = %d:\t", jugador.id + 1);
        fflush(stdin);
        fgets(jugador.nya, TAM, stdin);

        *(jugador.nya) = ES_LETRA(*(jugador.nya)) ? A_MAYUS(*(jugador.nya)) : *(jugador.nya);

        if('-' == *(jugador.nya))
        {
            ingresoNombres = 0;
        }
        else
        {
            (jugador.id)++;
            (*cantidadDeJugadores) = jugador.id;
            insertarAlFinalEnListaSimple(listaDeJugadores, &jugador, sizeof(tJugador));
        }
        system("cls");
    }
    while(ingresoNombres);

    if(!*cantidadDeJugadores)
    {
        return 0;
    }

    return OK;
}

void detallesJuego(tConfig* config)
{
    char op;
    tConfig* elemento;

    printf("Niveles de dificultad.\n F - Facil\n M - Medio\n D - Dificil\n");

    do
    {
        printf("\nIngrese el nivel de dificultad: ");
        scanf("%c", &op);
        getchar();

        if (op != 'F' && op != 'M' && op != 'D') {
            printf("\nOpcion incorrecta\n");
        }
    }while(op != 'F' && op != 'M' && op != 'D');

    elemento = (tConfig*)(buscarDificultad(config, &op, sizeof(tConfig), cmpDificultad));

    printf("\n-------------------------------------------\n");
    printf("Configuracion de la partida: \n");
    printf("Nivel: %c\n", elemento->nivel);
    printf("Tiempo que se muestra la secuencia: %i\n", elemento->tiempoSecuenciaEnPantalla);
    printf("Tiempo para contestar: %u\n", elemento->tiempoContestar);
    printf("Cantidad de vidas: %u\n", elemento->cantVidas);
    printf("-------------------------------------------\n");
    system("pause");
    system("cls");
}

void juego(void* recursos, tConfig* config)
{
    if(!ingresoDeNombresAListaSimple(&(((tRecursosMenu*)recursos)->listaDeJugadores), &(((tRecursosMenu*)recursos)->cantidadDeJugadores)))
    {
        printf("No ingresaste jugadores.\n");
    }
    else
    {
        detallesJuego(config);
        mezclarListaSimpleAleatoriamente(&(((tRecursosMenu*)recursos)->listaDeJugadores), ((tRecursosMenu*)recursos)->cantidadDeJugadores);
        printf("\nLista de jugadores:\n");
        mostrarListaSimpleEnOrden(&(((tRecursosMenu*)recursos)->listaDeJugadores), mostrarJugador);
        system("pause");
        system("cls");
    }
}

void crearLoteDePrueba (const char* nombreArch)
{
    FILE* fp;
    tConfig vectorConfig[] =
    {
        {'F', 10, 20, 3},
        {'M', 8, 15, 2},
        {'D', 5, 10, 0}
    };
    int n = sizeof(vectorConfig)/sizeof(*vectorConfig);
    int i;

    if(!abrirArchivo(&fp, nombreArch, "wt"))
    {
        return;
    }

    for(i = 0; i < n; i++)
    {
        fprintf(fp, "%c|%u|%u|%u\n",
                vectorConfig[i].nivel,
                vectorConfig[i].tiempoSecuenciaEnPantalla,
                vectorConfig[i].tiempoContestar,
                vectorConfig[i].cantVidas);
    }
    fclose(fp);
}

int cmpConfigNivel (const void* a, const void* b)
{
    return ((tConfig*)a)->nivel - ((tConfig*)b)->nivel;
}

int cmpDificultad(const void* a, const void* b)
{
    return ((tConfig*)a)->nivel - *((char*)b);
}

int insertarAlFinalVector (void* v, const void* d, int ce, size_t tam)
{
    void* ult = v + ce * tam;

    memcpy(ult, d, tam);
    return TODO_OK;
}

int leerArchivoConfig (const char* nombreArch, tConfig* vecConfig)
{
    FILE* fp;
    char linea[TAM_LINEA];
    tConfig config;
    int ce = 0;
    size_t tamConfig = sizeof(tConfig);

    if(!abrirArchivo(&fp, nombreArch, "rt"))
    {
        return ERR_ARCH_CONFIG;
    }

    while(fgets(linea, TAM_LINEA, fp))
    {
        trozarArchivoVariable(linea, &config);
        if(config.cantVidas < 0 || config.cantVidas > 5
           || config.tiempoContestar < 1 || config.tiempoContestar > 20
           || config.tiempoSecuenciaEnPantalla < 1 || config.tiempoSecuenciaEnPantalla > 20
           || (toupper(config.nivel) != 'F' && toupper(config.nivel) != 'M' && toupper(config.nivel) != 'D'))
        {
            fclose(fp);
            return ERR_FORMATO_ARCH;
        }

        if(!buscarPorClaveVector(vecConfig, &config, ce, tamConfig, cmpConfigNivel))
        {
            fclose(fp);
            return ERR_FORMATO_ARCH_DUPLICADO;
        }
        insertarAlFinalVector(vecConfig, &config, ce, tamConfig);
        ce++;
    }

    fclose(fp);
    return TODO_OK;
}

void* buscarDificultad(void* v, const void* dato, size_t tam, int(*cmp)(const void* a, const void* b))
{
    int i;
    void* elemento = v;

    for(i=0; i<TAM_VECTOR_CONFIG; i++)
    {
        if(cmp(elemento, dato) == 0)
            return elemento;
        elemento+=tam;
    }
}


int buscarPorClaveVector (void* v, const void* d, int ce, size_t tam, int (*cmp)(const void* x, const void* y))
{
    void* i = v;
    void* ult = v + (ce - 1) * tam;

    if(ce == 0)
        return NO_DUPLICADO;

    while(i <= ult && cmp(d, i) != 0)
        i += tam;

    if(i <= ult && cmp(d, i) == 0)
        return DUPLICADO;

    return NO_DUPLICADO;
}

void trozarArchivoVariable (char* s, tConfig* d)
{
    char* aux = strchr(s, '\n');
    *aux = '\0';

    /** Cant. Vidas */
    aux = strrchr(s, '|');
    d->cantVidas = atoi(aux + 1);
    *aux = '\0';

    /** Tiempo contestar */
    aux = strrchr(s, '|');
    d->tiempoContestar = atoi(aux + 1);
    *aux = '\0';

    /** Tiempo secuencia */
    aux = strrchr(s, '|');
    d->tiempoSecuenciaEnPantalla = atoi(aux + 1);
    *aux = '\0';

    /** Nivel */
    sscanf(s, "%c", &d->nivel);
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


void switchMenu(char opcion, void* recursos, tConfig* config)
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

void menuNuevo(char textoMenu[][MAX_TAM_TEXTO], unsigned cantidadDeRegistros, void* recursosMenu, tConfig* config)
{
    int i;
    char op;

    for(i=0; i<cantidadDeRegistros; i++)
    {
        printf("%s\n", *textoMenu);
        textoMenu++;
    }

    cargarOpcion(&op);

    switchMenu(op, recursosMenu, config);

}
