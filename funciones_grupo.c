/*
    Integrantes del grupo. En caso de ser un grupo de dos integrantes, no completar el último campo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido: Diaz, Nicolas
    DNI: 41714473
    Entrega: Si
    -----------------
    Apellido: Mussi Pulcini, Agustin Nicolas
    DNI: 44261623
    Entrega: Si
    -----------------
    Apellido: Girardin, Gaston Adrian
    DNI: 43325353
    Entrega: Si
    -----------------
    Apellido: Almada, Marcos Rodrigo
    DNI: 38662895
    Entrega: No
    -----------------
*/
#include "funciones_grupo.h"
int solucion(int argc, char* argv[])
{
    t_functionsData datosFunciones[FUNCTIONS];
    t_functionsData *ptrFun=datosFunciones;
    const char nombreGrupo[]="DINAMITA";
    char nombreImagenGuarBuff[261];
    char **argvPtr=argv+1;
    bool canLoad=true;
    int argToNumberRet=0;
    char couldLoad=0;
    t_imageDataVector imageStateDataVector;
    t_imageData *concatenar1=NULL,*concatenar2=NULL;
    if(!inicializarImageStateData(&imageStateDataVector))
    {
        puts("Error al asignar memoria para guardar los nombres de los archivos");
        return ERROR_MEMORIA;
    }
    inicializarDatosFunciones(datosFunciones);
    //modifica ptrMov y argvPtr
    for(int x=1; x<argc; x++)
    {
        if(isImageName(*argvPtr)&&!isDuplicatedName(imageStateDataVector.vec,imageStateDataVector.ce,*argvPtr))
        {
            canLoad=true;
            if(imageStateDataVector.ce==imageStateDataVector.tam)
            {
                if(!expandirImageStateData(&imageStateDataVector))
                {
                    printf("Advertencia: No se pudo asignar memoria para guardar datos de la imagen '%s'\n",*argvPtr);
                    canLoad=false;
                }
                else
                {
                    imageStateDataVector.ptrMov=imageStateDataVector.vec; //actualizamos al puntero nuevo que haya dejado realloc
                    imageStateDataVector.ptrMov+=imageStateDataVector.ce; //ademas reajustamos el offset que veniamos teniendo
                }
            }
            if(canLoad)
            {
                imageStateDataVector.ptrMov->name=*argvPtr;
                miStrcpy(imageStateDataVector.ptrMov->realName,obtenerNombreRealImg(*argvPtr));
                imageStateDataVector.ce++;
                imageStateDataVector.ptrMov++;
            }
        }
        argvPtr++;
    }
    restaurarPunteroImg(&imageStateDataVector); //volvemos a poner los punteros en su sitio
    argvPtr=argv+1;
    if(imageStateDataVector.ce==0)
    {
        puts("Error: No se ingreso ninguna imagen.");
        return ERROR_SIN_IMAGEN;
    }
    //modifica ptrMov
    for(int x=0; x<imageStateDataVector.ce; x++)
    {
        couldLoad=cargarImagen(imageStateDataVector.ptrMov);
        switch(couldLoad)
        {
        case 0:
            imageStateDataVector.ptrMov->isLoaded=true;
            break;
        case 1:
            printf("Advertencia: El archivo '%s' no se pudo abrir o no existe\n",imageStateDataVector.ptrMov->realName);
            break;
        case 2:
            printf("Advertencia: No se pudo asignar memoria para cargar los datos de la imagen '%s'\n",imageStateDataVector.ptrMov->realName);
            break;
        }
        imageStateDataVector.ptrMov++;
    }
    restaurarPunteroImg(&imageStateDataVector);
    //modifica argvPtr, y funPtr
    for(int x=1; x<argc; x++)
    {
        argToNumberRet=argToNumber(*argvPtr,datosFunciones);
        if(argToNumberRet>=0) //solo aplicamos el offset si se trata de una llamada a funcion valida
            ptrFun+=argToNumberRet;
        switch(argToNumberRet)
        {
        case 0: //escala-de-grises
        {
            if(!ptrFun->functionIsUsed)
            {
                procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,greyFilter,nombreGrupo,nombreImagenGuarBuff);
                ptrFun->functionIsUsed=true;
            }
            else
                printf("Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            break;
        }
        case 1: //espejar-horizontal
        {
            if(!ptrFun->functionIsUsed)
            {
                procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,espejarHorizontal,nombreGrupo,nombreImagenGuarBuff);
                ptrFun->functionIsUsed=true;
            }
            else
                printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            break;
        }
        case 2: //espejar-vertical
        {
            if(!ptrFun->functionIsUsed)
            {
                procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,espejarVertical,nombreGrupo,nombreImagenGuarBuff);
                ptrFun->functionIsUsed=true;
            }
            else
                printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            break;
        }
        case 3: //rotar-derecha
        {
            if(!ptrFun->functionIsUsed)
            {
                procesarImagenesFiltroArg0Met(&imageStateDataVector,datosFunciones,argToNumberRet,rotarDerecha,nombreGrupo,nombreImagenGuarBuff);
                ptrFun->functionIsUsed=true;
            }
            else
                printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            break;
        }
        case 4: //rotar-izquierda
        {
            if(!ptrFun->functionIsUsed)
            {
                procesarImagenesFiltroArg0Met(&imageStateDataVector,datosFunciones,argToNumberRet,rotarIzquierda,nombreGrupo,nombreImagenGuarBuff);
                ptrFun->functionIsUsed=true;
            }
            else
                printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            break;
        }
        case 5: //aumentar-contraste
        {
            if(obtenerPorcentaje(*argvPtr,&ptrFun->porcentaje))
            {
                if(!ptrFun->functionIsUsed)
                {
                    if(obtenerPorcentaje(*argvPtr,&ptrFun->porcentaje))
                    {
                        procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,aumentarContraste,nombreGrupo,nombreImagenGuarBuff);
                        ptrFun->functionIsUsed=true;
                    }
                }
                else
                    printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            }
            else
                mensajeDeErrorPorcentajes(*argvPtr);
            break;
        }
        case 6: //reducir-contraste
        {
            if(obtenerPorcentaje(*argvPtr,&ptrFun->porcentaje))
            {
                if(!ptrFun->functionIsUsed)
                {
                    if(obtenerPorcentaje(*argvPtr,&ptrFun->porcentaje))
                    {
                        procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,reducirContraste,nombreGrupo,nombreImagenGuarBuff);
                        ptrFun->functionIsUsed=true;
                    }
                }
                else
                    printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            }
            else
                mensajeDeErrorPorcentajes(*argvPtr);
            break;
        }
        case 7: //tonalidadAzul
        {
            if(obtenerPorcentaje(*argvPtr,&ptrFun->porcentaje))
            {
                if(!ptrFun->functionIsUsed)
                {
                    if(obtenerPorcentaje(*argvPtr,&ptrFun->porcentaje))
                    {
                        procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,tonalidadAzul,nombreGrupo,nombreImagenGuarBuff);
                        ptrFun->functionIsUsed=true;
                    }
                }
                else
                    printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            }
            else
                mensajeDeErrorPorcentajes(*argvPtr);
            break;
        }
        case 8: //tonalidad Verde
        {
            if(obtenerPorcentaje(*argvPtr,&ptrFun->porcentaje))
            {
                if(!ptrFun->functionIsUsed)
                {
                    if(obtenerPorcentaje(*argvPtr,&ptrFun->porcentaje))
                    {
                        procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,tonalidadVerde,nombreGrupo,nombreImagenGuarBuff);
                        ptrFun->functionIsUsed=true;
                    }
                }
                else
                    printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            }
            else
                mensajeDeErrorPorcentajes(*argvPtr);
            break;
        }
        case 9: //tonalidad Roja
        {
            if(obtenerPorcentaje(*argvPtr,&ptrFun->porcentaje))
            {
                if(!ptrFun->functionIsUsed)
                {
                    if(obtenerPorcentaje(*argvPtr,&ptrFun->porcentaje))
                    {
                        procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,tonalidadRoja,nombreGrupo,nombreImagenGuarBuff);
                        ptrFun->functionIsUsed=true;
                    }
                }
                else
                    printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            }
            else
                mensajeDeErrorPorcentajes(*argvPtr);
            break;
        }
        case 10://recortar
        {
            if(obtenerPorcentaje(*argvPtr,&ptrFun->porcentaje)&&ptrFun->porcentaje>0)
            {
                if(!ptrFun->functionIsUsed)
                {
                    if(obtenerPorcentaje(*argvPtr,&ptrFun->porcentaje))
                    {
                        procesarImagenesFiltroArg1Met(&imageStateDataVector,datosFunciones,argToNumberRet,recortar,nombreGrupo,nombreImagenGuarBuff);
                        ptrFun->functionIsUsed=true;
                    }
                }
                else
                    printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            }
            else
                mensajeDeErrorPorcentajes(*argvPtr);
            break;
        }
        case 11: //achicar
        {
            if(obtenerPorcentaje(*argvPtr,&ptrFun->porcentaje)&&ptrFun->porcentaje>0)
            {
                if(!ptrFun->functionIsUsed)
                {
                    if(obtenerPorcentaje(*argvPtr,&ptrFun->porcentaje))
                    {
                        procesarImagenesFiltroArg1Met(&imageStateDataVector,datosFunciones,argToNumberRet,achicar,nombreGrupo,nombreImagenGuarBuff);
                        ptrFun->functionIsUsed=true;
                    }
                }
                else
                    printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            }
            else
                mensajeDeErrorPorcentajes(*argvPtr);
            break;
        }
        case 12: //concatenar-horizontal
        {
            if(!ptrFun->functionIsUsed)
            {
                if(encontrarImagenes(&imageStateDataVector,&concatenar1,&concatenar2))
                {
                    if(concatenarHorizontal(concatenar1,concatenar2))
                    {
                        crearNombreImagenGuardadoConcat(concatenar1->realName,concatenar2->realName,ptrFun->functionName,nombreGrupo,nombreImagenGuarBuff);
                        if(!crearArchivoImagen(concatenar1,nombreImagenGuarBuff))
                            printf("ERROR: No se pudo crear el archivo %s\n",nombreImagenGuarBuff);
                        else
                            printf("La imagen %s fue creada exitosamente!\n",nombreImagenGuarBuff);
                        destruirMat((void**)concatenar1->imagePixelsMod,concatenar1->metadataMod.height);
                        concatenar1->imagePixelsMod=concatenar1->imagenPixelsModPtr;
                        restaurarImagenModificable(concatenar1);
                    }
                    else
                        printf("Error: no se pudo asignar memoria dinamica para la imagen concatenada\n");
                }
                else
                    printf("Nota: '%s' no se pudo ejecutar porque se necesitan al menos 2 imagenes cargadas\n",ptrFun->functionName);
                ptrFun->functionIsUsed=true;
            }
            else
                printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            break;
        }
        case 13: //negativo
        {
            if(!ptrFun->functionIsUsed)
            {
                procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,negativo,nombreGrupo,nombreImagenGuarBuff);
                ptrFun->functionIsUsed=true;
            }
            else
                printf("Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            break;
        }
        case 14: //comodin
        {
            if(!ptrFun->functionIsUsed)
            {
                procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,comodin,nombreGrupo,nombreImagenGuarBuff);
                ptrFun->functionIsUsed=true;
            }
            else
                printf("Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            break;
        }
        case 15: //concatenar-vertical
        {
            if(!ptrFun->functionIsUsed)
            {
                if(encontrarImagenes(&imageStateDataVector,&concatenar1,&concatenar2))
                {
                    if(concatenarVertical(concatenar1,concatenar2))
                    {
                        crearNombreImagenGuardadoConcat(concatenar1->realName,concatenar2->realName,ptrFun->functionName,nombreGrupo,nombreImagenGuarBuff);
                        if(!crearArchivoImagen(concatenar1,nombreImagenGuarBuff))
                            printf("ERROR: No se pudo crear el archivo %s\n",nombreImagenGuarBuff);
                        else
                            printf("La imagen %s fue creada exitosamente!\n",nombreImagenGuarBuff);
                        destruirMat((void**)concatenar1->imagePixelsMod,concatenar1->metadataMod.height);
                        concatenar1->imagePixelsMod=concatenar1->imagenPixelsModPtr;
                        restaurarImagenModificable(concatenar1);
                    }
                    else
                        printf("Error: no se pudo asignar memoria dinamica para la imagen concatenada\n");
                }
                else
                    printf("Nota: '%s' no se pudo ejecutar porque se necesitan al menos 2 imagenes cargadas\n",ptrFun->functionName);
                ptrFun->functionIsUsed=true;
            }
            else
                printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            break;
        }
        case -2:
        {
            printf("'%s' No es un comando valido o reconocido\n",*argvPtr);
            break;
        }
        }
        argvPtr++;
        ptrFun=datosFunciones;
    }
    liberarMemoria(&imageStateDataVector);
    return 0;
}

