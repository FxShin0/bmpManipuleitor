#ifndef FUNCIONES_DIAZ_H_INCLUDED
#define FUNCIONES_DIAZ_H_INCLUDED
#include "funciones_grupo.h"
void **crearMatrizGen (const unsigned int fil, const unsigned int col, const size_t tam);
void destruirMat (void** mat, const unsigned int fil);
bool inicializarImageStateData (t_imageDataVector *imageStateData);
bool expandirImageStateData (t_imageDataVector *imageStateDataVector);
char cargarImagen (t_imageData *datosImagen);
void inicializarRuta (char *buff, const char *arg, char **ptr);
bool crearArchivoImagen (const t_imageData *imagen, const char *nombreArchivo);
void procesarImagenesFiltroArg1Met (t_imageDataVector *imageStateDataVector, const t_functionsData *datosFunciones, unsigned char argToNumberRet,char filtro(t_imageData *imagen, int porcentaje), const char *nombreGrupo,char *nombreImagenGuarBuff,bool mode, FILE **pfErr);
void procesarImagenesFiltroArg0 (t_imageDataVector *imageStateDataVector, const t_functionsData *datosFunciones, unsigned char argToNumberRet, void filtro(t_imageData *imagen), const char *nombreGrupo,char *nombreImagenGuarBuff,bool mode, FILE **pfErr);
void procesarImagenesFiltroArg0Met (t_imageDataVector *imageStateDataVector, const t_functionsData *datosFunciones, unsigned char argToNumberRet,bool filtro(t_imageData *imagen), const char *nombreGrupo,char *nombreImagenGuarBuff,bool mode, FILE **pfErr);
void procesarImagenesConcat (t_imageDataVector *imageStateDataVector, t_functionsData *ptrFun, bool filtro (t_imageData *imagen1, t_imageData *imagen2), const char *nombreGrupo,char *nombreImagenGuarBuff, t_imageData **concatenar1, t_imageData **concatenar2,bool mode, FILE **pfErr);
bool encontrarImagenes (t_imageDataVector *imageStateDataVector, t_imageData **imagen1, t_imageData **imagen2);
void restaurarPunteroImg(t_imageDataVector *imageStateDataVector);
void liberarMemoria (t_imageDataVector *imageStateDataVector);
bool rotarDerecha (t_imageData *imagenSt);
bool rotarIzquierda (t_imageData *imagenSt);
char recortar(t_imageData *imagenSt, int porcentaje);
char achicar (t_imageData *imagenSt, int porcentaje);
bool concatenarHorizontal (t_imageData *imagen1, t_imageData *imagen2);
//agregados entrega 2
bool concatenarVertical (t_imageData *imagen1, t_imageData *imagen2);
bool isConfName (const char *arg);
bool isErrName (const char *arg);
void inicializarConfErr (t_archConfErrData *vec);
bool obtenerNombreErr (char *arg, t_archConfErrData *dest, t_archConfErrData *verif);
bool verificarArchivoConf(char *arg, t_archConfErrData *dest, t_archConfErrData *verif);
void limpiarArgConf (char *argConf);
#endif // FUNCIONES_DIAZ_H_INCLUDED
