#ifndef FUNCIONES_MUSSIPULCINI_H_INCLUDED
#define FUNCIONES_MUSSIPULCINI_H_INCLUDED
#include "funciones_grupo.h"
int miStrlen (const char *string);
char argToNumber (char *arg, const t_functionsData *ptr);
int miStrcmp (const char *s1, const char *s2);
char aMinus (char c);
void miStrcpy (char *dest, const char *ori);
void crearNombreImagenGuardado (const char *nombreImagen,const char *nombreFiltro, const char *nombreGrupo, char *nombreImagenGuarBuff);
void crearNombreImagenGuardadoConcat(char *nombreImagen1,const char *nombreImagen2,const char *nombreFiltro, const char *nombreGrupo, char *iniNombreImg);
char *obtenerNombreRealImg (const char *arg);
char *miStrrchr (const char *arg, char c);
void intercambiarPixeles (t_pixel *p1, t_pixel *p2);
void mensajeDeErrorPorcentajes (const char *arg);

void espejarVertical (t_imageData *imagenSt);
void espejarHorizontal (t_imageData *imagenSt);
void greyFilter (t_imageData *imagenSt);
void negativo (t_imageData *imagenSt);
#endif // FUNCIONES_MUSSIPULCINI_H_INCLUDED
