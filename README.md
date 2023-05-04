# FDF2

Nueva version en c++ de FDF42 [Texto del enlace](https://github.com/aruiz-ba/FDF42) 
un proyecto de modelado de paisajes en 3D con implementación del algoritmo de
trazado de líneas rectas de Bresenham.

Además de reescribir el programa en C++ y utilizar SDL para la gestión de
ventanas y eventos, he optimizado el código y añadido matrices de rotación, lo
que ha dado al software una perspectiva completamente diferente.

## Instalación

``` 
git clone git@github.com:aruiz-ba/FDF42.git

cd FDF42

make

./fdf2 ./maps/FDF42
``` 

## Controles

| Controles   | Descripción    |
|--------------- | --------------- |
| w   | Desplazar mapa hacia arriba   |
| s   | Desplazar mapa hacia abajo   |
| a   | Desplazar mapa hacia la izquierda   |
| d   | Desplazar mapa hacia la derecha |
| h   | Rotar incrementando el valor x |
| j   | Rotar incrementando el valor y |
| k   | Rotar incrementando el valor z |
| [shift] + h   | Rotar disminuyendo el valor x |
| [shift] + j   | Rotar disminuyendo el valor x |
| [shift] + k   | Rotar disminuyendo el valor x |

