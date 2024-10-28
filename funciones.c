#include "funciones.h"

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

void juego(void* recursos)
{
    if(!ingresoDeNombresAListaSimple(&(((tRecursosMenu*)recursos)->listaDeJugadores), &(((tRecursosMenu*)recursos)->cantidadDeJugadores)))
    {
        printf("No ingresaste jugadores.\n");
    }
    else
    {
        mezclarListaSimpleAleatoriamente(&(((tRecursosMenu*)recursos)->listaDeJugadores), ((tRecursosMenu*)recursos)->cantidadDeJugadores);
        printf("Lista de jugadores:\n");
        mostrarListaSimpleEnOrden(&(((tRecursosMenu*)recursos)->listaDeJugadores), mostrarJugador);
        system("pause");
        system("cls");
    }
}

void switchTextoMenu(int opcion, void* recursosMenu)
{
    switch(opcion)
    {
    case 'A':
        juego(recursosMenu);
        break;
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
