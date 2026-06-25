Sistema Interactivo Wack a mole (Golpea el topo) con Arduino


Integrantes

Jhon Cassiani
Yesid Hernandez
Roldan Rojas


Descripción del proyecto

Este proyecto consiste en el desarrollo de un juego interactivo basado en el clásico Whack-a-Mole (Golpea el Topo) utilizando Arduino Uno. El sistema
emplea LEDs verdes para representar los topos, botones para que el jugador pueda golpearlos y LEDs rojos para mostrar efectos visuales, trampas y
puntuaciones. El objetivo es que el jugador reaccione rápidamente presionando el botón correcto cuando aparece un topo, acumulando puntos durante una
partida de 30 segundos.


Problema identificado

Actualmente existen pocas alternativas prácticas para aplicar conocimientos de programación y electrónica de forma interactiva y entretenida. Este
proyecto busca demostrar cómo integrar sensores, actuadores y lógica de programación en un sistema recreativo que permita desarrollar habilidades de
reacción y coordinación.

La falta de proyectos de este tipo limita las oportunidades de aprendizaje práctico en electrónica y programación. Además, los juegos electrónicos
permiten comprender conceptos como eventos aleatorios, control de tiempo, máquinas de estados y sistemas de puntuación.


Objetivo general

Diseñar e implementar un sistema de juego interactivo tipo "Golpea el Topo" utilizando Arduino Uno, botones y LEDs, que permita al usuario obtener
puntos mediante la identificación y activación correcta de los topos mostrados aleatoriamente.


Objetivos específicos

Implementar tres niveles de dificultad que modifiquen la velocidad y complejidad del juego.
Programar la aparición aleatoria de los topos utilizando LEDs verdes.
Desarrollar un sistema de puntuación y penalización basado en la precisión del jugador.
Incorporar trampas identificadas mediante LEDs rojos para aumentar la dificultad.
Mostrar visualmente la puntuación final mediante secuencias de LEDs al finalizar cada partida.


Componentes utilizados

Componente	                Cantidad	                    Función

Arduino Uno	                   1	             Controlador principal del sistema
LEDs verdes	                   3                 Representan los topos
LEDs rojos	                   5	             Indicadores visuales y trampas
Botones pulsadores	           3	             Interacción del jugador
Resistencias	              11	             Protección y control de corriente
Protoboard	                   1	             Montaje del circuito
Cables de conexión	          21	             Interconexión de componentes


Arquitectura del sistema

Botones -> Arduino Uno -> Lógica del juego

Arduino Uno -> LEDs verdes (Topos)

Arduino Uno -> LEDs rojos (Trampas y puntuación)

Fuente de alimentación -> Arduino Uno y circuito


Funcionamiento

El usuario selecciona la dificultad mediante uno de los tres botones.
El sistema configura los parámetros correspondientes al nivel elegido.
El jugador inicia la partida presionando cualquier botón.
Durante 30 segundos aparecen topos representados por LEDs verdes.
El jugador debe presionar el botón correspondiente al topo iluminado.
Cada acierto suma puntos.
Si se presiona un botón incorrecto, se resta un punto.
Cuando aparece una trampa, los LEDs rojos parpadean como advertencia.
Si el jugador golpea una trampa, pierde la mitad de sus puntos acumulados.
Al finalizar el tiempo, el sistema muestra la puntuación mediante los LEDs rojos y bloquea temporalmente los botones.


Evidencias del proyecto

Fotos Y Esquema de conexiones

![Evidencia_Del_Circuito_Y_Esquema](image.png)

Ejemplos

Video de funcionamiento:

Ver video del proyecto

<video controls src="Prueba.mp4" title="Evidencia del funcionamiento y Explicacion"></video>


Código fuente

El programa principal fue desarrollado en Arduino IDE y permite controlar la lógica completa del juego, incluyendo selección de dificultad, generación
aleatoria de topos, sistema de puntuación, trampas y visualización de resultados.


Enlace al diseño:

https://www.tinkercad.com/things/lCicWXOTWYX/editel?sharecode=cDscO-N-cd544z5rHsBDAqrfZyjdYY-nIHKSwuc6sV4


Pruebas realizadas

Selección de dificultad	Verificación de los tres modos de juego	Correcto
Aparición de topos	Encendido aleatorio de LEDs verdes	Correcto
Sistema de puntuación	Suma y resta de puntos	Correcto
Sistema de trampas	Penalización al presionar un topo trampa	Correcto
Visualización final	Encendido progresivo de LEDs rojos	Correcto
Prueba integral	Ejecución completa del juego	Exitosa


Estado actual del proyecto

El proyecto se encuentra finalizado y funcional. Todas las características principales fueron implementadas, incluyendo niveles de dificultad, sistema
de puntuación, trampas, efectos visuales y visualización de resultados.


Dificultades encontradas

Sincronizar correctamente los tiempos de aparición de los topos.
Implementar eventos aleatorios sin repeticiones consecutivas.
Gestionar los diferentes estados del juego (selección, inicio, partida y finalización).
Diseñar un sistema de trampas equilibrado que aumentara la dificultad sin afectar la jugabilidad.


Mejoras futuras
Incorporar una pantalla LCD para mostrar la puntuación en tiempo real.
Agregar un buzzer para efectos sonoros.
Implementar una tabla de récords.
Añadir más niveles de dificultad.
Permitir partidas multijugador.
Incorporar comunicación Bluetooth para registrar estadísticas.


Conclusiones

El proyecto permitió aplicar conocimientos de electrónica, programación y diseño de sistemas interactivos mediante el uso de Arduino Uno. Se logró
desarrollar un juego completamente funcional que integra entradas digitales, salidas visuales, eventos aleatorios y sistemas de puntuación. Además, se
fortalecieron habilidades relacionadas con el control de hardware y la implementación de lógica de programación orientada a eventos, demostrando el
potencial de Arduino para la creación de aplicaciones educativas y recreativas.hola