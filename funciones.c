#include "funciones.h"
#include "menu.h"
#include "vector.h"
#define ES_LETRA(X) (((X) >= 'a' && (X) <= 'z') || ((X) >= 'A' && (X) <= 'Z'))
#define A_MAYUS(X) (((X) >= 'a' && (X) <= 'z') ? ((X) - ('a' - 'A')) : (X))

/** Funciones CURL **/

size_t WriteCallback(char* contents, size_t tamanyo, size_t nmemb, void *userp)
{
    size_t totalSize = tamanyo * nmemb;

    memcpy(((tUserDataWriteCallback*)userp)->buffer, contents, totalSize);

    ((tUserDataWriteCallback*)userp)->cantBytesCargados = totalSize;

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
            jugador.id++;
            *cantidadDeJugadores = jugador.id;
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

void transformarCad(char *cad, char *vec, unsigned cantElem, unsigned cantVec)
{
    int i;
    for(i = 0; i < cantElem; i++)
        *(cad + i) = *(vec + (*(cad + i) - '0') % cantVec);
}

void juego(void* recursos, tConfig* config)
{
    tConfig *dificultad;
    char colores[4] = {'R', 'N', 'A', 'V'};
    if(!ingresoDeNombresAListaSimple(&((tRecursosMenu*)recursos)->listaDeJugadores, &(((tRecursosMenu*)recursos)->cantidadDeJugadores)))
    {
        printf("No ingresaste jugadores.\n");
    }
    else
    {
        dificultad = detallesJuego(config);
        mezclarListaSimpleAleatoriamente(&(((tRecursosMenu*)recursos)->listaDeJugadores), ((tRecursosMenu*)recursos)->cantidadDeJugadores);
        printf("\nLista de jugadores:\n");
        mostrarListaSimpleEnOrden(&(((tRecursosMenu*)recursos)->listaDeJugadores), mostrarJugador);
        system("pause");
        system("cls");
        if(pedirNumeros(URL, ((tRecursosMenu*)recursos)->secuencia, 100) == ERROR_CURL)
        {
            fprintf(stderr, "Error al pedir informacion a la API\n");
            exit(1);
        }

        transformarCad(((tRecursosMenu*)recursos)->secuencia, colores, sizeof(((tRecursosMenu*)recursos)->secuencia), sizeof(colores));
        rondas(recursos, dificultad);
    }
}

void mostrarSecuencia(char *secuencia, unsigned cant, unsigned milisegundos)
{
    int i;

    printf("Secuencia: ");
    for(i = 0; i < cant; i++)
    {
        printf("%c\t", *secuencia);
        secuencia++;
        Sleep(milisegundos / cant);
    }
}

int ingresarSecuencia(t_lista *ingresos, unsigned cantIngresos, unsigned tiempo, char *secuencia, unsigned *cantidadIngresos)
{
    clock_t tiempoIni = clock();
    int incorrecto, tiempoPasado = 0;
    char respuesta;
    int correctas = *cantidadIngresos;
    printf("Ingrese la secuencia mostrada por pantalla letra por letra, una a la vez:\n");

    do
    {
        do
        {
            fflush(stdin);
            scanf("%c", &respuesta);
            incorrecto = respuesta != 'R' && respuesta != 'N' && respuesta != 'A' && respuesta != 'V' && respuesta != 'Z';
            if(incorrecto)
                printf("Caracter invalido, ingrese nuevamente.\n");
        }
        while(incorrecto && clock() - tiempoIni <= tiempo * 1000);
        if(!incorrecto && clock() - tiempoIni <= tiempo * 1000)
        {
            if(respuesta == 'Z')
                return VIDAS;
            if(*(secuencia + *cantidadIngresos) == toupper(respuesta))
            {
                correctas++;
            }
            (*cantidadIngresos)++;

            insertarPrimeroEnListaSimple(ingresos, &respuesta, sizeof(char));
        }

        tiempoPasado = clock() - tiempoIni <= tiempo * 1000;
    }
    while(tiempoPasado && *cantidadIngresos < cantIngresos);

    if(!tiempoPasado)
        return !*cantidadIngresos ? VIDAS : PERDER;
    if(*cantidadIngresos < cantIngresos)
        return PERDER;

    system("cls");
    return (correctas == cantIngresos ? TODO_OK : VIDAS);
}

unsigned ingresarVidas(unsigned cantUsable, unsigned vidasDisp)
{
    int vidasIngresadas;

    printf("Tiene la oportunidad de usar vidas para retroceder 1 jugada por vida. Tiene %u vidas disponibles ¿Cuantas vidas quiere usar?\n", vidasDisp);

    do
    {
        scanf("%d", &vidasIngresadas);
    }
    while(vidasIngresadas > vidasDisp || vidasIngresadas > cantUsable || vidasIngresadas <= 0);

    return vidasIngresadas;
}

/*int usarVidas(unsigned vidas, unsigned cantIngresos, unsigned ronda, char* secuencia)
{
    int i, vidasUsadas;
    while(result != TODO_OK && result != PERDER)
    {
        if(!vidas)
        {
            return PERDER;
        }
        else
        {
            vidasUsadas = ingresarVidas(cantIngresos + 1, vidas);

            for(i = 0; i < vidasUsadas; i++)
                sacarPrimeroEnListaSimple()
            }
    }
}*/

void rondas(void *recursos, tConfig *dificultad)
{
    int turno = 1, ronda = 1, result;
    char *secuencia = ((tRecursosMenu*)recursos)->secuencia;
    t_lista lista;
    unsigned cantIngresos;
    unsigned vidas = dificultad->cantVidas, vidasUsadas = 0;

    crearListaSimple(&lista);
    while(turno <= ((tRecursosMenu*)recursos)->cantidadDeJugadores)
    {
        system("cls");
        cantIngresos = 0;
        vaciarListaSimple(&lista);
        printf("Turno %d | ronda %d:\n", turno, ronda);
        mostrarSecuencia(secuencia, ronda, (dificultad->tiempoSecuenciaEnPantalla + ronda) * 1000);
        system("cls");
        result = ingresarSecuencia(&lista, ronda, dificultad->tiempoContestar, secuencia, &cantIngresos);

        if(result == VIDAS)
        {
            if(!vidas)
                result = PERDER;

            while(result == VIDAS && !vidasUsadas && vidas)
            {
                vidasUsadas = ingresarVidas(cantIngresos + 1, vidas);

                while(vidasUsadas && sacarPrimeroEnListaSimple(&lista, NULL, 0))
                {
                    vidasUsadas--;
                    vidas--;
                    cantIngresos--;
                }

                if(!vidasUsadas)
                    result = ingresarSecuencia(&lista, ronda, dificultad->tiempoContestar, secuencia, &cantIngresos);
            }
            if(vidasUsadas)
            {
                vidasUsadas = 0;
                vidas--;
            }
        }

        if(result == PERDER)
        {
            printf("No tiene mas vidas, usted ha perdido\n");
            system("pause");
            system("cls");

            secuencia += ronda; ///VER QUE NO NOS QUEDEMOS SIN NUMEROS
            turno++;
            ronda = 1;
            vidas = dificultad->cantVidas;
        }
        else
        {
            if(result == TODO_OK)
            {
                ronda++;
            }
        }
    }

    //mostrarListaSimpleEnOrden(&lista, mostrarCaracter);
}

void eliminarCaracter(char *cad, char caracter, unsigned longitud)
{
    char *escrit = cad;
    int borrar = 0;

    while(longitud--)
    {
        if(*cad == caracter)
        {
            borrar = 1;
        }
        else
        {
            if(borrar)
            {
                *escrit = *cad;
                escrit++;
            }
        }

        cad++;
        if(!borrar)
            escrit++;
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

int pedirNumeros(const char *url, char *buffer, unsigned cant)
{
    CURL* curl;
    tUserDataWriteCallback dato;

    dato.buffer = malloc(cant * 2);
    dato.cantBytesCargados = 0;
    if(NULL == dato.buffer)
    {
        fprintf(stderr,"No pude reservar memoria.");
        return ERROR_CURL;
    }

    if(!iniciarCURL(&curl))//RESERVO RECURSOS PARA REALIZAR CONFIGURACIONES DE REQUEST
    {
        free(dato.buffer);
        return ERROR_CURL;
    }

    //CONFIGURACION DE LOS RECURSOS RESERVADOS PARA HACER LA REQUEST
    configurarCURL(curl, &dato.buffer);

    //SE HACE LA REQUEST Y SE REALIZA MANEJO DE ERRORES POR SI FALLA
    if(!ejecutarSolicitud(curl))
    {
        curl_easy_cleanup(curl);    //LIBERO RECURSOS RESERVADOS
        free(dato.buffer);
        return ERROR_CURL;
    }

    eliminarCaracter(dato.buffer, '\n', dato.cantBytesCargados);
    memcpy(buffer, dato.buffer, cant);
    free(dato.buffer);
    return dato.cantBytesCargados / 2;
}

void mostrarCaracter(const void* dato)
{
    printf("%c", *(char*)dato);
}
