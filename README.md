# Cmon dice
Una vez iniciado el juego, se presentara el siguiente menu donde debe ingresar la letra **A** si desea jugar o **B** si quiere salir del juego.

![](https://github.com/ezequielmp/TP-AyED/blob/main/cmonmenu.png)

Si la opcion ingresada fue **A**:
1. Se debe ingresar los nombres de los jugadores.
2. Elegir la dificultad

Luego, los jugadores seran sorteados para determinar el orden de juego.

## Reglas del juego
1. Empieza el primer jugador sorteado.
2. Se muestra la primera secuencia al jugador durante X tiempo, una letra a la vez
3. El jugador tiene Y segundos para ingresar la secuencia letra por letra. 
4. Si se le acaba el tiempo y no contesta:
    * Si tiene una vida, puede volver para atras N cantidad de jugadas y tiene un 		      segundo intento
    * Si no tiene vidas, el juego finaliza para esa persona y le toca al siguiente jugador.

5. Si ingresa correctamente la secuencia, se le mostrara al jugador la siguiente secuencia.
6. Si ingresa la secuencia, pero es incorrecta, puede deshacer los ultimos movimientos, segun la cantidad de vidas que tenga.
7. Por cada round se van sumando puntos, y el jugador que obtenga la mayor cantidad de puntos es el ganador.

### Sistema de puntos
Ganara la persona con mayor cantidad de puntos. En caso de que hayan 2 personas con la mayor cantidad de puntos, ambos seran ganadores.

* Por cada secuencia bien ingresada, sin usar vida, se recibe **+3 puntos**
* Por cada secuencia bien ingresada, pero usando vida, se recibe **+1 punto**

### Niveles de dificultad
El juego posee tres niveles de dificultad:
   * Facil
   * Medio
   * Dificil
   
Estos niveles de dificultad son configurables mediante un archivo txt.

Si desea cambiar estas configuraciones iniciales, debera modificar el archivo **config.txt** que se encuentra dentro de los archivos del juego, respetando el siguiente formato.

Nivel | Cantidad de tiempo que se muestra las secuencia completa | Cantidad de tiempo que tiene el jugador para contestar | Cantidad de vidas
