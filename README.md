Memoria Dinámica: todas las matrices se generaron de manera generica usando memoria dinamica.

Punteros a funciones: Se usan punteros a funciones para generalizar los distintos filtros y evitar repetir codigo, las funciones procesadoras son:
-procesarImagenesFiltroArg1Met: sirve para recortar y achicar. (modifican metadatos, reciben un porcentaje para trabajar)
-procesarImagenesFiltroArg0: sirve para escala de grises, espejar horizontal, espejar vertical, negativo, comodin (no modifican metadatos, no reciben un porcentaje)
-procesarImagenesFiltroArg0Met: sirve para rotar-derecha, rotar-izquierda (modifican metadatos, no reciben un porcentaje)
-procesarImagenesConcat: sirve para concatenar-horizontal y concatenar-vertical (modifican metadatos, no reciben un porcentaje, trabajan con 2 imagenes a la vez)
-procesarImagenesFiltroArg1: sirve para aumentar contraste, reducir contraste, tonalidad azul, tonalidad verde, tonalidad roja (no modifican metadatos, reciben un porcentaje)
Cada una de ellas recibe el filtro a aplicar correspondiente en cada caso.

Archivos de texto: es posible aplicar filtros usando un archivo .conf y un archivo .txt para log de errores/flujo.
Se pueden declarar multiples archivos tal que --filtros=conf.conf --filtros=hola.conf --errores=hola.txt --errores=punto.txt
Lo que el programa hará es seleccionar el primer archivo .conf cuyo nombre sea valido y que ademas exista, y en cuanto a archivo de errores seleccionara 
el primero que no se llame igual que el de filtros y ademas tenga la extension correcta (.txt).
Imaginemos que hola.conf existe y conf.conf no, entonces el programa ignorara el primer argumento y usara el segundo como archivo de configuracion.
De todos modos el programa muestra por pantalla cual archivo de configuracion se eligio y cual se usara como log.
Si se ingresa mas de 1 archivo de configuracion valido se usara siempre el primero.
Para el procesamiento del archivo .conf los mensajes que antes se mostraban en pantalla pasaran a escribirse en el log, ahi no solo estaran los errores, sino tambien
las imagenes que se hayan podido crear con exito, todos los eventos relevantes (sea errores de memoria dinamica, porcentajes invalidos, errores generales, argumentos incorrectos y 
imagenes generadas exitosamente) seran informadas en el archivo .txt de manera tal que se pueda seguir el rastro de que fue lo que sucedio durante el procesamiento.
Cabe destacar que el .conf debe existir previamente, pero el .txt se crea.
