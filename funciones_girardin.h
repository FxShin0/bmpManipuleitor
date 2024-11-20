#ifndef FUNCIONES_GIRARDIN_H_INCLUDED
#define FUNCIONES_GIRARDIN_H_INCLUDED
#include "funciones_grupo.h"
bool isImageName (const char *arg);
bool isDuplicatedName (const t_imageData *names, const int ce,const char *name);
bool obtenerPorcentaje (const char *arg, int *porcentaje);
bool esNumero (char c);
void inicializarDatosFunciones (t_functionsData *datosFunciones);
void restaurarImagenModificable (t_imageData *imagen);
unsigned char calcularPadding (const unsigned int width);
void procesarImagenesFiltroArg1 (t_imageDataVector *imageStateDataVector, const t_functionsData *datosFunciones, unsigned char argToNumberRet,void filtro(t_imageData *imagen, int porcentaje), const char *nombreGrupo,char *nombreImagenGuarBuff, bool mode, FILE **pfErr);
void recalcularMetadatos (t_imageData *imagen, int height, int width);

void aumentarContraste (t_imageData *imagenSt, int porcentaje);
void reducirContraste (t_imageData *imagenSt, int porcentaje);
void tonalidadAzul (t_imageData *imagenSt, int porcentaje);
void tonalidadVerde (t_imageData *imagenSt, int porcentaje);
void tonalidadRoja (t_imageData *imagenSt, int porcentaje);
void comodin (t_imageData *imagenSt);
#endif // FUNCIONES_GIRARDIN_H_INCLUDED
