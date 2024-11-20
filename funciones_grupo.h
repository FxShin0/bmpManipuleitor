#ifndef FUNCIONES_GRUPO_H_INCLUDED
#define FUNCIONES_GRUPO_H_INCLUDED
/*
Aquí deben hacer los includes de sus archivos individuales, por ejemplo:
#include "funciones_machi.h"
 */
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define FUNCTIONS 16
#define ERROR_MEMORIA -1
#define ERROR_SIN_IMAGEN -2
#define TAM_INI_IMGVEC 5
#define INCR_FAC 1
#define FILTRO_NULO -3
#define FILTRO_APLICADO -4
#pragma pack(push, 1)
#include <stdbool.h>
typedef struct
{
    unsigned char pixel[3];
}t_pixel;
typedef struct
{
    unsigned short fileType;            // 2 bytes
    unsigned int fileSize;              // 4 bytes
    unsigned short reserved1;           // 2 bytes
    unsigned short reserved2;           // 2 bytes
    unsigned int imageStart;            // 4 bytes
    unsigned int bmpHeaderSize;         // 4 bytes
    unsigned int width;                 // 4 bytes
    unsigned int height;                // 4 bytes
    unsigned short planeQuantity;       // 2 bytes
    unsigned short bitsPerPixel;        // 2 bytes
    unsigned int compression;           // 4 bytes
    unsigned int imageSize;             // 4 bytes
    unsigned int hResolution;           // 4 bytes
    unsigned int vResolution;           // 4 bytes
    unsigned int colorTableSize;        // 4 bytes
    unsigned int importantColorCounter; // 4 bytes
} t_metadata;
typedef struct
{
    const char *name; //se refiere a la direccion y el nombre real (argv)
    char realName[200]; //se refiere al nombre real de la imagen
    bool isLoaded;
    t_metadata metadata;
    t_metadata metadataMod; //copia modificable de los metadatos
    unsigned char padding; //guarda el padding original
    unsigned char paddingMod; //guarda padding de imagen modificada (en caso de que se haya modificado metadatos claro)
    t_pixel **imagePixels;
    t_pixel **imagePixelsMod; //copia modificable de los pixeles
    t_pixel **imagenPixelsModPtr; //copia del puntero de la matriz modificable original de la imagen (hay filtros que cambian la direccion por eso necesito guardarlo)
}t_imageData;
typedef struct
{
    t_imageData *vec;
    int ce;
    int tam;
    t_imageData *ptrMov;
}t_imageDataVector;
typedef struct
{
    char functionName[50];
    bool functionIsUsed;
    int porcentaje;
}t_functionsData;
typedef struct
{
    char archNom[256]; //indica el nombre del archivo conf/errores
    bool isValid; //indica si existe y es valido para usar en el procesamiento de .conf
}t_archConfErrData;
#include "funciones_diaz.h"
#include "funciones_girardin.h"
#include "funciones_mussipulcini.h"
int solucion(int argc, char* argv[]);
#endif // FUNCIONES_GRUPO_H_INCLUDED
