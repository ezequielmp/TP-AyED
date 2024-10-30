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
    unsigned ingresoNombres = 1, cont;

    jugador.id = 0;
    *cantidadDeJugadores = 0;

    system("cls");
    printf("INGRESO DE NOMBRES DE LOS JUGADORES\n");
    do
    {
        printf("Ingrese '-' para finalizar\n");
        printf("\nIngrese nombre ID = %d:\t", jugador.id + 1);
        fflush(stdin);
        fgets(jugador.nya, TAM_NOM, stdin);

        *(jugador.nya) = ES_LETRA(*(jugador.nya)) ? A_MAYUS(*(jugador.nya)) : *(jugador.nya);

        if('-' == *(jugador.nya))
        {
            ingresoNombres = 0;
        }
        else
        {
            if(*jugador.nya == '\n')
            {
                printf("No ha ingresado un nombre valido.\n");
                system("pause");
            }
            else
            {
                cont = 0;
                jugador.id++;
                *cantidadDeJugadores = jugador.id;
                while(cont < TAM_NOM && *(jugador.nya + cont) != '\n')
                    cont++;
                *(jugador.nya + cont) = '\0';
                insertarAlFinalEnListaSimple(listaDeJugadores, &jugador, sizeof(tJugador));
            }
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
    int result;
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

        result = pedirNumeros(URL, ((tRecursosMenu*)recursos)->secuencia, TAM);
        if(result == ERROR_CURL)
        {
            fprintf(stderr, "Error al pedir informacion a la API\n");
            exit(1);
        }
        ((tRecursosMenu*)recursos)->longitudSecuencia = result;
        transformarCad(((tRecursosMenu*)recursos)->secuencia, colores, result, sizeof(colores));
        rondas(recursos, dificultad);
    }
}

void mostrarSecuencia(t_lista *secuencia, unsigned cant, unsigned milisegundos)
{
    int i;
    char secuenciaChar;

    printf("Secuencia: ");
    for(i = 0; i < cant; i++)
    {
        buscarPorIndiceEnListaSimple(secuencia, &secuenciaChar, sizeof(char), i);
        printf("%c\t", secuenciaChar);
        Sleep(milisegundos / cant);
    }
}

int ingresarSecuencia(t_lista *ingresos, unsigned cantIngresos, unsigned tiempo, t_lista *secuencia, unsigned *cantidadIngresos)
{
    clock_t tiempoIni = clock();
    int incorrecto, tiempoPasado = 0, i;
    char respuesta, caracterSecuencia;
    int correctas = 0;

    if(*cantidadIngresos)
    {
        printf("Sus ingresos anteriores:\n");
        for(i = 0; i < *cantidadIngresos; i++)
        {
            buscarPorIndiceEnListaSimple(ingresos, &respuesta, sizeof(char), *cantidadIngresos - i - 1);
            buscarPorIndiceEnListaSimple(secuencia, &caracterSecuencia, sizeof(char), i);

            printf("%c", respuesta);

            if(respuesta == caracterSecuencia)
                correctas++;
        }
        printf("\n");
    }
    printf("Ingrese la secuencia mostrada por pantalla letra por letra, una a la vez:\n");

    do
    {
        do
        {
            fflush(stdin);
            scanf("%c", &respuesta);
            respuesta = toupper(respuesta);
            incorrecto = respuesta != 'R' && respuesta != 'N' && respuesta != 'A' && respuesta != 'V' && respuesta != 'Z';
            if(incorrecto)
                printf("Caracter invalido, ingrese nuevamente.\n");
        }
        while(incorrecto && clock() - tiempoIni <= tiempo * 1000);
        if(!incorrecto && clock() - tiempoIni <= tiempo * 1000)
        {
            if(respuesta == 'Z')
                return VIDAS;
            buscarPorIndiceEnListaSimple(secuencia, &caracterSecuencia, sizeof(char), *cantidadIngresos);

            if(caracterSecuencia == toupper(respuesta))
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
    int vidasIngresadas, ingresoInorrecto;

    printf("Tiene la oportunidad de usar vidas para retroceder 1 jugada por vida. Tiene %u vidas disponibles ¿Cuantas vidas quiere usar?\n", vidasDisp);

    do
    {
        scanf("%d", &vidasIngresadas);
        ingresoInorrecto = vidasIngresadas > vidasDisp || vidasIngresadas > cantUsable || vidasIngresadas <= 0;
        if(ingresoInorrecto)
            printf("Ingreso invalido, pruebe nuevamente:\n");
    }
    while(ingresoInorrecto);

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

int cmpPuntajes(const void* a, const void* b)
{
    return ((tJugador*)a)->puntajeJugador - ((tJugador*)b)->puntajeJugador;
}

void escribirChar(const void* dato1, void *parametro)
{
    fprintf((void*)dato1, "%c", *(char*)parametro);
}

void escribirNombre(const void* param1, void* dato)
{
    fprintf((void*)param1, "%s\n", ((tJugador*)dato)->nya);
}

int usarVidas(t_lista *ingresos, unsigned ronda, unsigned *vidasRestantes, unsigned *vidasUsadas, unsigned *cantIngresos, t_lista *secuencia, tConfig *dificultad)
{
    int result, i, vidasPorCiclo;
    *vidasUsadas = 0;

    do
    {
        if(!*vidasRestantes)
            return PERDER;

        vidasPorCiclo = ingresarVidas(*cantIngresos + 1, *vidasRestantes);
        *vidasRestantes -= vidasPorCiclo;
        *vidasUsadas += vidasPorCiclo;

        if(vidasPorCiclo == *cantIngresos + 1)
        {
            *cantIngresos = 0;
            return VIDAS;
        }

        for(i = 0; i < vidasPorCiclo; i++)
        {
            sacarPrimeroEnListaSimple(ingresos, NULL, 0);
            (*cantIngresos)--;
        }

        result = ingresarSecuencia(ingresos, ronda, dificultad->tiempoContestar + ronda, secuencia, cantIngresos);
    }
    while(result == VIDAS);
    return result;
}

int solicitarAPI(char *url, char *secuencia, unsigned cant, char *colores, unsigned cantColores)
{
    int result = pedirNumeros(url, secuencia, cant);
    if(result == ERROR_CURL)
    {
        fprintf(stderr, "Error al pedir informacion a la API\n");
        return ERROR_CURL;
    }
    transformarCad(secuencia, colores, result, cantColores);
    return result;
}

void rondas(void *recursos, tConfig *dificultad)
{
    int turno = 1, ronda = 1, result;
    char colores[4] = {'R', 'N', 'A', 'V'};
    ///
    int cantPuntajeRonda = 0;
    int cantPuntajeRondaTotalJugador = 0;
    int puntajeMaximo = 0;
    char nombreArchivo[TAM_NOMBRE_REPORTE_ARCHIVO];
    tJugador jugadorActual;
    ///

    char *secuencia = ((tRecursosMenu*)recursos)->secuencia;
    t_lista lista, listaSecuencia;
    unsigned cantIngresos;
    unsigned vidas = dificultad->cantVidas, vidasUsadas = 0;

    ///
    crearNombreArchivoReporte(nombreArchivo);
    FILE* fp;
    if(!abrirArchivo(&fp, nombreArchivo, "wt"))
    {
        return;
    }
    ///
    buscarPorIndiceEnListaSimple(&((tRecursosMenu*)recursos)->listaDeJugadores, &jugadorActual, sizeof(tJugador), 0);
    crearListaSimple(&lista);
    crearListaSimple(&listaSecuencia);
    insertarAlFinalEnListaSimple(&listaSecuencia, secuencia, sizeof(char));
    while(turno <= ((tRecursosMenu*)recursos)->cantidadDeJugadores)
    {
        system("cls");
        cantIngresos = 0;
        printf("Turno %d | ronda %d:\n", turno, ronda);
        mostrarSecuencia(&listaSecuencia, ronda, (dificultad->tiempoSecuenciaEnPantalla + ronda) * 1000);
        system("cls");
        result = ingresarSecuencia(&lista, ronda, dificultad->tiempoContestar, &listaSecuencia, &cantIngresos);

        if(result == VIDAS)
            result = usarVidas(&lista, ronda, &vidas, &vidasUsadas, &cantIngresos, &listaSecuencia, dificultad);

        if(result != VIDAS)
        {
            if(result == PERDER)
            {
                printf("No puede usar vidas y su ingreso fue incorrecto, usted ha perdido\n");
                system("pause");
                system("cls");
                ///
                escribirArchivoReporte(fp, jugadorActual.nya, ronda, &listaSecuencia, &lista, 0, vidasUsadas);
                jugadorActual.puntajeJugador = cantPuntajeRondaTotalJugador;
                if(cantPuntajeRondaTotalJugador > puntajeMaximo)
                {
                    puntajeMaximo = cantPuntajeRondaTotalJugador;
                    ActualizarPorIndiceEnListaSimple(&((tRecursosMenu*)recursos)->listaDeJugadores, &jugadorActual, sizeof(tJugador), turno - 1);
                }

                fprintf(fp, "Puntaje final del jugador %s: %d\n", jugadorActual.nya, cantPuntajeRondaTotalJugador);

                cantPuntajeRonda = 0;
                cantPuntajeRondaTotalJugador = 0;
                ///

                ((tRecursosMenu*)recursos)->longitudSecuencia -= ronda;
                secuencia += ronda; ///VER QUE NO NOS QUEDEMOS SIN NUMEROS
                turno++;
                ronda = 1;
                vidas = dificultad->cantVidas;
                vaciarListaSimple(&listaSecuencia);
                buscarPorIndiceEnListaSimple(&((tRecursosMenu*)recursos)->listaDeJugadores, &jugadorActual, sizeof(tJugador), turno - 1);
            }
            else
            {
                if(result == TODO_OK)
                {
                    cantPuntajeRonda = (vidasUsadas > 0) ? 1 : 3; // +1 si uso una vida o +3 si ingreso la secuencia sin errores
                    cantPuntajeRondaTotalJugador += cantPuntajeRonda; // Sumo la cantidad de puntos totales por ronda
                    escribirArchivoReporte(fp, jugadorActual.nya, ronda, &listaSecuencia, &lista, cantPuntajeRonda, vidasUsadas);
                    ///
                    ronda++;
                    secuencia++;
                    ((tRecursosMenu*)recursos)->longitudSecuencia--;
                    if(((tRecursosMenu*)recursos)->longitudSecuencia <= 0)
                    {
                        if((((tRecursosMenu*)recursos)->longitudSecuencia = solicitarAPI(URL, (((tRecursosMenu*)recursos)->secuencia), TAM, colores, sizeof(colores))) == ERROR_CURL)
                        {
                            printf("Ocurrio un error al solicitar la API");
                            fclose(fp);
                            exit(-1);
                        }
                        secuencia = ((tRecursosMenu*)recursos)->secuencia;
                    }
                }
            }
            if(insertarAlFinalEnListaSimple(&listaSecuencia, secuencia, sizeof(char)) == NO_PUDE_INSERTAR)
            {
                printf("Memoria insuficiente");
                fclose(fp);
                exit(-1);
            }
            vidasUsadas = 0;
        }
        vaciarListaSimple(&lista);
    }
    ///
    printf("Juego terminado");

    if(puntajeMaximo != 0)
    {
        jugadorActual.puntajeJugador = puntajeMaximo;
        dejarSoloClave(&((tRecursosMenu*)recursos)->listaDeJugadores, &jugadorActual, cmpPuntajes);
        printf("\nGANADORES DEL JUEGO: \n");
        mostrarListaSimpleEnOrden(&((tRecursosMenu*)recursos)->listaDeJugadores, mostrarJugador);
        fprintf(fp, "GANADORES DEL JUEGO:\n");
        mapInversoListaSimpleParam(&((tRecursosMenu*)recursos)->listaDeJugadores, escribirNombre, fp);
    }
    else
    {
        printf("Nadie gano...\n");
    }

    fclose(fp);
    ///

    //mostrarListaSimpleEnOrden(&lista, mostrarCaracter);
}

int crearNombreArchivoReporte (char* nombreArchReporte)
{
    time_t ahora = time(NULL);
    struct tm *t = localtime(&ahora);
    strftime(nombreArchReporte, 100, FORMATO_NOMBRE_ARCH_REPORTE, t);
    return TODO_OK;
}

void escribirArchivoReporte(FILE* fpArch, const char* nombreJugador, unsigned ronda, const t_lista *secuenciaMostrada, const t_lista* respuestaJugador,
                            unsigned puntajeObtenidoPorPregunta, unsigned cantVidasUsadas)
{
    //Imprime el nombre del jugador
    fprintf(fpArch, "Jugador: %s\n", nombreJugador);

    //Imprime la secuencia mostrada y la respuesta del jugador
    fprintf(fpArch, "Ronda: %d\nSecuencia Mostrada: ", ronda);
    //Imprime la secuencia mostrada
    mapListaSimpleParam(secuenciaMostrada, escribirChar, fpArch);

    fprintf(fpArch, "\nRespuesta: ");
    if(!listaSimpleVacia(respuestaJugador))
    {
        //Mostrar la lista respuestaJugador
        mapInversoListaSimpleParam(respuestaJugador, escribirChar, fpArch);
    }
    fprintf(fpArch, "\nPuntaje de la ronda: %d\nVidas usadas: %d\n\n", puntajeObtenidoPorPregunta, cantVidasUsadas);
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
        if(trozarArchivoVariable(linea, &config) == NO_PUDE_LEER
                || config.cantVidas < 0 || config.cantVidas > 5
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

int trozarArchivoVariable (char* s, tConfig* d)
{
    char* aux = strchr(s, '\n');
    if(!aux)
        return NO_PUDE_LEER;
    *aux = '\0';

    /** Cant. Vidas */
    aux = strrchr(s, '|');
    if(!aux)
        return NO_PUDE_LEER;
    d->cantVidas = atoi(aux + 1);
    *aux = '\0';

    /** Tiempo contestar */
    aux = strrchr(s, '|');
    if(!aux)
        return NO_PUDE_LEER;
    d->tiempoContestar = atoi(aux + 1);
    *aux = '\0';

    /** Tiempo secuencia */
    aux = strrchr(s, '|');
    if(!aux)
        return NO_PUDE_LEER;
    d->tiempoSecuenciaEnPantalla = atoi(aux + 1);
    *aux = '\0';

    /** Nivel */
    sscanf(s, "%c", &d->nivel);
    return TODO_OK;
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
