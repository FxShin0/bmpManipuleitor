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
    t_archConfErrData vecConfErr[2];
    t_functionsData *ptrFun=datosFunciones;
    FILE *pfConf=NULL,*pfErr=NULL;
    int confCounter=0;
    const char nombreGrupo[]="DINAMITA";
    char nombreImagenGuarBuff[261];
    char argConfBuff[256];
    char **argvPtr=argv+1;
    bool canLoad=true, flagConf=false,flagErr=false,flagConfAttempt=false,flagOneImg=false;
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
    inicializarConfErr(vecConfErr);
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
        liberarMemoria(&imageStateDataVector); //liberamos la memoria asignada en inicializacion
        return ERROR_SIN_IMAGEN;
    }
    //modifica ptrMov
    for(int x=0; x<imageStateDataVector.ce; x++)
    {
        couldLoad=cargarImagen(imageStateDataVector.ptrMov);
        switch(couldLoad)
        {
        case 0:
        {
            imageStateDataVector.ptrMov->isLoaded=true;
            flagOneImg=true;
            break;
        }
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
    if(!flagOneImg) //verificamos que de las imagenes ingresadas se haya cargado al menos una, si no se cargo ninguna entonces cortamos aca.
    {
        printf("Error: Ninguna de las imagenes ingresadas se pudieron cargar, sea porque no existen o porque no habia memoria para guardarlas.\n");
        liberarMemoria(&imageStateDataVector); //liberamos la memoria
        return ERROR_SIN_IMAGEN;
    }
    //modifica argvPtr, y funPtr. Procesamiento de argumentos del main
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
                procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,greyFilter,nombreGrupo,nombreImagenGuarBuff,false,&pfErr);
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
                procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,espejarHorizontal,nombreGrupo,nombreImagenGuarBuff,false,&pfErr);
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
                procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,espejarVertical,nombreGrupo,nombreImagenGuarBuff,false,&pfErr);
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
                procesarImagenesFiltroArg0Met(&imageStateDataVector,datosFunciones,argToNumberRet,rotarDerecha,nombreGrupo,nombreImagenGuarBuff,false,&pfErr);
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
                procesarImagenesFiltroArg0Met(&imageStateDataVector,datosFunciones,argToNumberRet,rotarIzquierda,nombreGrupo,nombreImagenGuarBuff,false,&pfErr);
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
                    procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,aumentarContraste,nombreGrupo,nombreImagenGuarBuff,false,&pfErr);
                    ptrFun->functionIsUsed=true;
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
                    procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,reducirContraste,nombreGrupo,nombreImagenGuarBuff,false,&pfErr);
                    ptrFun->functionIsUsed=true;
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
                    procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,tonalidadAzul,nombreGrupo,nombreImagenGuarBuff,false,&pfErr);
                    ptrFun->functionIsUsed=true;
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
                    procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,tonalidadVerde,nombreGrupo,nombreImagenGuarBuff,false,&pfErr);
                    ptrFun->functionIsUsed=true;
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
                    procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,tonalidadRoja,nombreGrupo,nombreImagenGuarBuff,false,&pfErr);
                    ptrFun->functionIsUsed=true;
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
                    procesarImagenesFiltroArg1Met(&imageStateDataVector,datosFunciones,argToNumberRet,recortar,nombreGrupo,nombreImagenGuarBuff,false,&pfErr);
                    ptrFun->functionIsUsed=true;
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
                    procesarImagenesFiltroArg1Met(&imageStateDataVector,datosFunciones,argToNumberRet,achicar,nombreGrupo,nombreImagenGuarBuff,false,&pfErr);
                    ptrFun->functionIsUsed=true;
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
                procesarImagenesConcat(&imageStateDataVector,ptrFun,concatenarHorizontal,nombreGrupo,nombreImagenGuarBuff,&concatenar1,&concatenar2,false,&pfErr);
            else
                printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            break;
        }
        case 13: //negativo
        {
            if(!ptrFun->functionIsUsed)
            {
                procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,negativo,nombreGrupo,nombreImagenGuarBuff,false,&pfErr);
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
                procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,comodin,nombreGrupo,nombreImagenGuarBuff,false,&pfErr);
                ptrFun->functionIsUsed=true;
            }
            else
                printf("Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            break;
        }
        case 15: //concatenar-vertical
        {
            if(!ptrFun->functionIsUsed)
                procesarImagenesConcat(&imageStateDataVector,ptrFun,concatenarVertical,nombreGrupo,nombreImagenGuarBuff,&concatenar1,&concatenar2,false,&pfErr);
            else
                printf("Nota: Se detecto una llamada demas a la funcion %s\n",ptrFun->functionName);
            break;
        }
        case -2: //argumento no valido
        {
            printf("'%s' No es un comando valido o reconocido\n",*argvPtr);
            break;
        }
        case -3: //posible arch de conf
        {
            flagConfAttempt=true;
            if(vecConfErr->isValid==false)
            {
                if(verificarArchivoConf(*argvPtr, vecConfErr,vecConfErr+1))
                {
                    flagConf=true;
                    vecConfErr->isValid=true;
                }
            }
            break;
        }
        case -4: //posible arch de errores
        {
            if((vecConfErr+1)->isValid==false)
            {
                if(obtenerNombreErr(*argvPtr,vecConfErr+1,vecConfErr))
                {
                    flagErr=true;
                    (vecConfErr+1)->isValid=true;
                }
            }
            break;
        }
        }
        argvPtr++;
        ptrFun=datosFunciones;
    }
    //procesamiento de los filtros recibidos por el archivo conf
    if(flagConf) //confirmamos que hay un archivo conf con el cual trabajar
    {
        if(!flagErr) //nos fijamos que haya uno de errores
        {
            printf("Error: si bien se especifico un archivo .conf, no se especifico un nombre para el archivo de log, o no existe,\no no tiene la extension adecuada (.txt), o se llama igual que el de configuracion.");
            liberarMemoria(&imageStateDataVector);
            return ERROR_LOG;
        }
        else //procesamos en este caso.
        {
            printf("Nota: se utilizara el archivo '%s' como archivo de configuracion y '%s' como log de errores.\n",vecConfErr->archNom,(vecConfErr+1)->archNom);
            pfConf=fopen(vecConfErr->archNom,"rt");
            if(pfConf==NULL)
            {
                printf("Hubo algun error al abrir '%s'.\n",vecConfErr->archNom);
                liberarMemoria(&imageStateDataVector);
                return ERROR_ARCH;
            }
            pfErr=fopen((vecConfErr+1)->archNom,"wt");
            if(pfErr==NULL)
            {
                printf("Hubo algun error al crear '%s'.\n",(vecConfErr+1)->archNom);
                liberarMemoria(&imageStateDataVector);
                return ERROR_ARCH;
            }
            while(fgets(argConfBuff,256,pfConf))
            {
                limpiarArgConf(argConfBuff);
                fprintf(pfErr,"%d. Procesando argumento: %s\n",confCounter+1,argConfBuff);
                argToNumberRet=argToNumber(argConfBuff,datosFunciones);
                if(argToNumberRet>=0) //solo aplicamos el offset si se trata de una llamada a funcion valida
                    ptrFun+=argToNumberRet;
                switch(argToNumberRet)
                {
                case 0: //escala-de-grises
                {
                    if(!ptrFun->functionIsUsed)
                    {
                        procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,greyFilter,nombreGrupo,nombreImagenGuarBuff,true,&pfErr);
                        ptrFun->functionIsUsed=true;
                    }
                    else
                        fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    break;
                }
                case 1: //espejar-horizontal
                {
                    if(!ptrFun->functionIsUsed)
                    {
                        procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,espejarHorizontal,nombreGrupo,nombreImagenGuarBuff,true,&pfErr);
                        ptrFun->functionIsUsed=true;
                    }
                    else
                        fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    break;
                }
                case 2: //espejar-vertical
                {
                    if(!ptrFun->functionIsUsed)
                    {
                        procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,espejarVertical,nombreGrupo,nombreImagenGuarBuff,true,&pfErr);
                        ptrFun->functionIsUsed=true;
                    }
                    else
                        fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    break;
                }
                case 3: //rotar-derecha
                {
                    if(!ptrFun->functionIsUsed)
                    {
                        procesarImagenesFiltroArg0Met(&imageStateDataVector,datosFunciones,argToNumberRet,rotarDerecha,nombreGrupo,nombreImagenGuarBuff,true,&pfErr);
                        ptrFun->functionIsUsed=true;
                    }
                    else
                        fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    break;
                }
                case 4: //rotar-izquierda
                {
                    if(!ptrFun->functionIsUsed)
                    {
                        procesarImagenesFiltroArg0Met(&imageStateDataVector,datosFunciones,argToNumberRet,rotarIzquierda,nombreGrupo,nombreImagenGuarBuff,true,&pfErr);
                        ptrFun->functionIsUsed=true;
                    }
                    else
                        fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    break;
                }
                case 5: //aumentar-contraste
                {
                    if(obtenerPorcentaje(argConfBuff,&ptrFun->porcentaje))
                    {
                        if(!ptrFun->functionIsUsed)
                        {
                            procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,aumentarContraste,nombreGrupo,nombreImagenGuarBuff,true,&pfErr);
                            ptrFun->functionIsUsed=true;
                        }
                        else
                            fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    }
                    else
                        fprintf(pfErr,"Argumento: '%s'. Los porcentajes ingresados son invalidos o no tienen un formato correcto.\n",argConfBuff);
                    break;
                }
                case 6: //reducir-contraste
                {
                    if(obtenerPorcentaje(argConfBuff,&ptrFun->porcentaje))
                    {
                        if(!ptrFun->functionIsUsed)
                        {
                            procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,reducirContraste,nombreGrupo,nombreImagenGuarBuff,true,&pfErr);
                            ptrFun->functionIsUsed=true;
                        }
                        else
                            fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    }
                    else
                        fprintf(pfErr,"Argumento: '%s'. Los porcentajes ingresados son invalidos o no tienen un formato correcto.\n",argConfBuff);
                    break;
                }
                case 7: //tonalidadAzul
                {
                    if(obtenerPorcentaje(argConfBuff,&ptrFun->porcentaje))
                    {
                        if(!ptrFun->functionIsUsed)
                        {
                            procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,tonalidadAzul,nombreGrupo,nombreImagenGuarBuff,true,&pfErr);
                            ptrFun->functionIsUsed=true;
                        }
                        else
                            fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    }
                    else
                        fprintf(pfErr,"Argumento: '%s'. Los porcentajes ingresados son invalidos o no tienen un formato correcto.\n",argConfBuff);
                    break;
                }
                case 8: //tonalidad Verde
                {
                    if(obtenerPorcentaje(argConfBuff,&ptrFun->porcentaje))
                    {
                        if(!ptrFun->functionIsUsed)
                        {
                            procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,tonalidadVerde,nombreGrupo,nombreImagenGuarBuff,true,&pfErr);
                            ptrFun->functionIsUsed=true;
                        }
                        else
                            fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    }
                    else
                        fprintf(pfErr,"Argumento: '%s'. Los porcentajes ingresados son invalidos o no tienen un formato correcto.\n",argConfBuff);
                    break;
                }
                case 9: //tonalidad Roja
                {
                    if(obtenerPorcentaje(argConfBuff,&ptrFun->porcentaje))
                    {
                        if(!ptrFun->functionIsUsed)
                        {
                            procesarImagenesFiltroArg1(&imageStateDataVector,datosFunciones,argToNumberRet,tonalidadRoja,nombreGrupo,nombreImagenGuarBuff,true,&pfErr);
                            ptrFun->functionIsUsed=true;
                        }
                        else
                            fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    }
                    else
                        fprintf(pfErr,"Argumento: '%s'. Los porcentajes ingresados son invalidos o no tienen un formato correcto.\n",argConfBuff);
                    break;
                }
                case 10://recortar
                {
                    if(obtenerPorcentaje(argConfBuff,&ptrFun->porcentaje)&&ptrFun->porcentaje>0)
                    {
                        if(!ptrFun->functionIsUsed)
                        {
                            procesarImagenesFiltroArg1Met(&imageStateDataVector,datosFunciones,argToNumberRet,recortar,nombreGrupo,nombreImagenGuarBuff,true,&pfErr);
                            ptrFun->functionIsUsed=true;
                        }
                        else
                            fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    }
                    else
                        fprintf(pfErr,"Argumento: '%s'. Los porcentajes ingresados son invalidos o no tienen un formato correcto.\n",argConfBuff);
                    break;
                }
                case 11: //achicar
                {
                    if(obtenerPorcentaje(argConfBuff,&ptrFun->porcentaje)&&ptrFun->porcentaje>0)
                    {
                        if(!ptrFun->functionIsUsed)
                        {
                            procesarImagenesFiltroArg1Met(&imageStateDataVector,datosFunciones,argToNumberRet,achicar,nombreGrupo,nombreImagenGuarBuff,true,&pfErr);
                            ptrFun->functionIsUsed=true;
                        }
                        else
                            fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    }
                    else
                    {
                        fprintf(pfErr,"Argumento: '%s'. Los porcentajes ingresados son invalidos o no tienen un formato correcto.\n",argConfBuff);
                    }
                    break;
                }
                case 12: //concatenar-horizontal
                {
                    if(!ptrFun->functionIsUsed)
                        procesarImagenesConcat(&imageStateDataVector,ptrFun,concatenarHorizontal,nombreGrupo,nombreImagenGuarBuff,&concatenar1,&concatenar2,true,&pfErr);
                    else
                        fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    break;
                }
                case 13: //negativo
                {
                    if(!ptrFun->functionIsUsed)
                    {
                        procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,negativo,nombreGrupo,nombreImagenGuarBuff,true,&pfErr);
                        ptrFun->functionIsUsed=true;
                    }
                    else
                        fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    break;
                }
                case 14: //comodin
                {
                    if(!ptrFun->functionIsUsed)
                    {
                        procesarImagenesFiltroArg0(&imageStateDataVector,datosFunciones,argToNumberRet,comodin,nombreGrupo,nombreImagenGuarBuff,true,&pfErr);
                        ptrFun->functionIsUsed=true;
                    }
                    else
                        fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    break;
                }
                case 15: //concatenar-vertical
                {
                    if(!ptrFun->functionIsUsed)
                        procesarImagenesConcat(&imageStateDataVector,ptrFun,concatenarVertical,nombreGrupo,nombreImagenGuarBuff,&concatenar1,&concatenar2,true,&pfErr);
                    else
                        fprintf(pfErr,"Argumento: '%s'. La funcion '%s' ya fue llamado previamente en otro argumento anterior.\n",argConfBuff,ptrFun->functionName);
                    break;
                }
                case -1: //imagen
                {
                    fprintf(pfErr,"Argumento: '%s'. No se puede especificar una imagen en el archivo de configuracion.\n",argConfBuff);
                    break;
                }
                case -2: //argumento no valido
                {
                    fprintf(pfErr,"Argumento: '%s'. No es un comando valido o reconocido\n",argConfBuff);
                    break;
                }
                case -3: //posible arch de conf
                {
                    fprintf(pfErr,"Argumento: '%s'. No se puede indicar un archivo de configuracion dentro del archivo de configuracion.\n",argConfBuff);
                    break;
                }
                case -4: //posible arch de errores
                {
                    fprintf(pfErr,"Argumento: '%s'. No se puede indicar un archivo de log de errores dentro del archivo de configuracion.\n",argConfBuff);
                    break;
                }
                }
                ptrFun=datosFunciones;
                confCounter++;
                fprintf(pfErr,"\n");
            }
            if(confCounter==0)
                fprintf(pfErr,"El archivo de configuracion se encontraba vacio.\n");
            fclose(pfConf);
            fclose(pfErr);
        }
    }
    else if(flagConfAttempt)
        printf("Error: el archivo de configuracion especificado no existe o se llama igual que el de errores o no tiene la extension adecuada (.conf).\n");
    liberarMemoria(&imageStateDataVector);
    return 0;
}



