#include "funciones_diaz.h"
void **crearMatrizGen (const unsigned int fil, const unsigned int col, const size_t tam)
{
    void **x,**y,**mat,**ult;
    mat=malloc(sizeof(void*)*fil);
    if(mat==NULL)
        return NULL;
    ult=mat+fil-1;
    for(x=mat; x<=ult; x++)
    {
        *x=malloc(tam*col);
        if(*x==NULL)
        {
            for(y=x; y<=ult; y++)
                free(*y);
            free(mat);
            return NULL;
        }
    }
    return (void**)mat;
}
void destruirMat (void** mat, const unsigned int fil)
{
    void **x,**ult;
    ult=mat+fil-1;
    for(x=mat; x<=ult; x++)
    {
        free(*x);
    }
    free(mat);
}
bool expandirImageStateData (t_imageDataVector *imageStateDataVector)
{
    t_imageData *temp,*ptrAux;
    int incremento=imageStateDataVector->tam*INCR_FAC;
    temp=(t_imageData*)realloc(imageStateDataVector->vec,sizeof(t_imageData)*(imageStateDataVector->tam+incremento));
    if(temp==NULL)
        return false;
    imageStateDataVector->vec=temp;
    ptrAux=temp+imageStateDataVector->tam-1;
    imageStateDataVector->tam+=incremento;
    for(int x=0; x<incremento; x++)
    {
        ptrAux->isLoaded=false;
        ptrAux++;
    }
    return true;
}
bool inicializarImageStateData (t_imageDataVector *imageStateData)
{
    t_imageData *ptrAux;
    imageStateData->ce=0;
    imageStateData->vec=(t_imageData*)malloc(sizeof(t_imageData)*TAM_INI_IMGVEC);
    if(imageStateData->vec==NULL)
        return false;
    imageStateData->tam=TAM_INI_IMGVEC;
    ptrAux=imageStateData->vec;
    imageStateData->ptrMov=imageStateData->vec;
    for(int x=0; x<TAM_INI_IMGVEC; x++)
    {
        ptrAux->isLoaded=false;
        ptrAux++;
    }
    return true;
}
char cargarImagen (t_imageData *datosImagen)
{
    FILE *arch;
    unsigned char auxPad[3]; //para guardar el padding al usar fread
    t_pixel **ptrImg, **ptrImgMod;
    arch=fopen(datosImagen->name,"rb");
    if(arch==NULL)
        return 1;
    fread(&datosImagen->metadata,sizeof(t_metadata),1,arch);
    memcpy(&datosImagen->metadataMod,&datosImagen->metadata,sizeof(t_metadata)); //realiza la copia modificable de los metadatos
    datosImagen->padding=(4-(datosImagen->metadata.width*sizeof(t_pixel)%4))%4;
    datosImagen->paddingMod=datosImagen->padding;
    fseek(arch,datosImagen->metadata.imageStart,SEEK_SET);
    datosImagen->imagePixels=(t_pixel **)crearMatrizGen(datosImagen->metadata.height,datosImagen->metadata.width,sizeof(t_pixel));
    if(datosImagen->imagePixels==NULL)
        return 2;
    datosImagen->imagePixelsMod=(t_pixel **)crearMatrizGen(datosImagen->metadata.height,datosImagen->metadata.width,sizeof(t_pixel));
    datosImagen->imagenPixelsModPtr=datosImagen->imagePixelsMod;
    if(datosImagen->imagePixelsMod==NULL)
        return 2;
    ptrImg=datosImagen->imagePixels;
    ptrImgMod=datosImagen->imagePixelsMod;
    for(int x=0; x<datosImagen->metadata.height; x++)
    {
        fread(*ptrImg,sizeof(t_pixel),datosImagen->metadata.width,arch);
        if(datosImagen->padding>0)
            fread(auxPad,datosImagen->padding,1,arch); //salta el padding
        ptrImg++;
    }
    fclose(arch);
    ptrImg=datosImagen->imagePixels;
    for(int x=0; x<datosImagen->metadata.height; x++)
    {
        memcpy(*ptrImgMod,*ptrImg,sizeof(t_pixel)*(datosImagen->metadata.width));
        ptrImgMod++;
        ptrImg++;
    }
    return 0;
}
bool crearArchivoImagen (const t_imageData *imagen, const char *nombreArchivo)
{
    FILE *arch;
    char padding;
    unsigned char auxpad[3]= {0,0,0};
    t_pixel **imgPtrMod=imagen->imagePixelsMod;
    arch=fopen(nombreArchivo,"wb");
    if(arch==NULL)
        return false;
    fwrite(&imagen->metadataMod,sizeof(t_metadata),1,arch);
    fseek(arch,imagen->metadataMod.imageStart,SEEK_SET);
    padding=imagen->paddingMod;
    for(int x=0; x<imagen->metadataMod.height; x++)
    {
        fwrite(*imgPtrMod,sizeof(t_pixel),imagen->metadataMod.width,arch);
        if(padding>0)
            fwrite(auxpad,padding,1,arch);
        imgPtrMod++;
    }
    fclose(arch);
    return true;
}
void procesarImagenesFiltroArg1Met (t_imageDataVector *imageStateDataVector, const t_functionsData *datosFunciones, unsigned char argToNumberRet,char filtro(t_imageData *imagen, int porcentaje), const char *nombreGrupo,char *nombreImagenGuarBuff,bool mode, FILE **pfErr)
{
    //Funcion procesadora para aquellos filtros que reciben 1 argumento, trabajan con 1 imagen y que modifican metadatos
    //Sirve para: --recortar, --achicar
    t_imageData *vecPtr=imageStateDataVector->vec; //puntero de imagenes
    const t_functionsData *funPtr=datosFunciones+argToNumberRet; //puntero de datos de funciones
    for(int y=0; y<imageStateDataVector->ce; y++)
    {
        if(vecPtr->isLoaded)
        {
            crearNombreImagenGuardado(vecPtr->realName,funPtr->functionName,nombreGrupo,nombreImagenGuarBuff);
            switch(filtro(vecPtr,funPtr->porcentaje))
            {
            case FILTRO_APLICADO:
            {
                if(!crearArchivoImagen(vecPtr,nombreImagenGuarBuff))
                    mode==false?printf("ERROR: No se pudo crear el archivo %s\n",nombreImagenGuarBuff):
                        fprintf(*pfErr,"ERROR: No se pudo crear el archivo %s\n",nombreImagenGuarBuff);
                else
                    mode==false?printf("La imagen %s fue creada exitosamente!\n",nombreImagenGuarBuff):
                        fprintf(*pfErr,"La imagen %s fue creada exitosamente!\n",nombreImagenGuarBuff);
                destruirMat((void**)vecPtr->imagePixelsMod,vecPtr->metadataMod.height);
                vecPtr->imagePixelsMod=vecPtr->imagenPixelsModPtr;
                restaurarImagenModificable(vecPtr);
                break;
            }
            case ERROR_MEMORIA:
            {
                mode==false?printf("ERROR: No se pudo asignar memoria para generar la imagen %s\n",nombreImagenGuarBuff):
                    fprintf(*pfErr,"ERROR: No se pudo asignar memoria para generar la imagen %s\n",nombreImagenGuarBuff);
                break;
            }
            case FILTRO_NULO:
            {
                if(!crearArchivoImagen(vecPtr,nombreImagenGuarBuff))
                    mode==false?printf("ERROR: No se pudo crear el archivo %s\n",nombreImagenGuarBuff):
                        fprintf(*pfErr,"ERROR: No se pudo crear el archivo %s\n",nombreImagenGuarBuff);
                else
                    mode==false?printf("La imagen %s fue creada exitosamente!\n",nombreImagenGuarBuff):
                        fprintf(*pfErr,"La imagen %s fue creada exitosamente!\n",nombreImagenGuarBuff);
                break;
            }
            default:
                break;
            }
        }
        vecPtr++;
    }
}
void procesarImagenesFiltroArg0Met (t_imageDataVector *imageStateDataVector, const t_functionsData *datosFunciones, unsigned char argToNumberRet,bool filtro(t_imageData *imagen), const char *nombreGrupo,char *nombreImagenGuarBuff,bool mode, FILE **pfErr)
{
    //funcion procesadora para aquellos filtros que no tienen argumentos de entrada , trabajan con una imagen y modifican metadatos
    //sirve para rotar-derecha, rotar-izquierda
    const t_functionsData *funPtr=datosFunciones+argToNumberRet;
    t_imageData *vecPtr=imageStateDataVector->vec;
    for(int y=0; y<imageStateDataVector->ce; y++)
    {
        if(vecPtr->isLoaded)
        {
            crearNombreImagenGuardado(vecPtr->realName,funPtr->functionName,nombreGrupo,nombreImagenGuarBuff);
            if(!filtro(vecPtr))
                mode==false?printf("ERROR: No se pudo asignar memoria para generar la imagen %s\n",nombreImagenGuarBuff):
                    fprintf(*pfErr,"ERROR: No se pudo asignar memoria para generar la imagen %s\n",nombreImagenGuarBuff);
            else
            {
                if(!crearArchivoImagen(vecPtr,nombreImagenGuarBuff))
                    mode==false?printf("ERROR: No se pudo crear el archivo %s\n",nombreImagenGuarBuff):
                        fprintf(*pfErr,"ERROR: No se pudo crear el archivo %s\n",nombreImagenGuarBuff);
                else
                    mode==false?printf("La imagen %s fue creada exitosamente!\n",nombreImagenGuarBuff):
                        fprintf(*pfErr,"La imagen %s fue creada exitosamente!\n",nombreImagenGuarBuff);
                destruirMat((void**)vecPtr->imagePixelsMod,vecPtr->metadataMod.height);
                vecPtr->imagePixelsMod=vecPtr->imagenPixelsModPtr;
                restaurarImagenModificable(vecPtr);
            }
        }
        vecPtr++;
    }
}
void procesarImagenesFiltroArg0 (t_imageDataVector *imageStateDataVector, const t_functionsData *datosFunciones, unsigned char argToNumberRet,void filtro(t_imageData *imagen), const char *nombreGrupo,char *nombreImagenGuarBuff,bool mode, FILE **pfErr)
{
    //funcion procesadora para aquellos filtros que trabajan con una sola imagen, no modifican metadatos ni tienen argumentos de entrada
    //sirve para escala de grises, espejar horizontal, espejar vertical, negativo, comodin
    const t_functionsData *funPtr=datosFunciones+argToNumberRet;
    t_imageData *ptrVec=imageStateDataVector->vec;
    for(int y=0; y<imageStateDataVector->ce; y++)
    {
        if(ptrVec->isLoaded)
        {
            filtro(ptrVec);
            crearNombreImagenGuardado(ptrVec->realName,funPtr->functionName,nombreGrupo,nombreImagenGuarBuff);
            if(!crearArchivoImagen(ptrVec,nombreImagenGuarBuff))
                mode==false?printf("ERROR: No se pudo crear el archivo %s\n",nombreImagenGuarBuff):
                    fprintf(*pfErr,"ERROR: No se pudo crear el archivo %s\n",nombreImagenGuarBuff);
            else
                mode==false?printf("La imagen %s fue creada exitosamente!\n",nombreImagenGuarBuff):
                    fprintf(*pfErr,"La imagen %s fue creada exitosamente!\n",nombreImagenGuarBuff);
            restaurarImagenModificable(ptrVec);
        }
        ptrVec++;
    }
}
bool encontrarImagenes (t_imageDataVector *imageStateDataVector, t_imageData **imagen1, t_imageData **imagen2)
{
    int ce=imageStateDataVector->ce,x=0,flag1=0,flag2=0;
    t_imageData *vec=imageStateDataVector->vec;
    //quizas previamente ya se habia llamado a encontrar imagenes, en ese caso evitamos procesar ya que ya se encontraron las imagenes.
    if(*imagen1!=NULL&&*imagen2!=NULL)
        return true;
    while((flag1==0||flag2==0)&&x<ce)
    {
        if(vec->isLoaded)
        {
            if(flag1==0)
            {
                *imagen1=vec;
                flag1=1;
            }
            else if (flag2==0)
            {
                *imagen2=vec;
                flag2=1;
            }
        }
        vec++;
        x++;
    }
    if(flag1==0||flag2==0)
        return false;
    return true;
}
void restaurarPunteroImg(t_imageDataVector *imageStateDataVector)
{
    imageStateDataVector->ptrMov=imageStateDataVector->vec;
}
void liberarMemoria (t_imageDataVector *imageStateDataVector)
{
    t_imageData *vec=imageStateDataVector->vec;
    for(int x=0; x<imageStateDataVector->ce; x++)
    {
        if(vec->isLoaded)
        {
            destruirMat((void**)vec->imagePixels,vec->metadata.height);
            destruirMat((void**)vec->imagePixelsMod,vec->metadata.height);
        }
        vec++;
    }
    free(imageStateDataVector->vec);
}
bool rotarIzquierda (t_imageData *imagenSt)
{
    int newHeight=imagenSt->metadataMod.width,newWidth=imagenSt->metadataMod.height;
    int oriHeight=imagenSt->metadataMod.height,oriWidth=imagenSt->metadataMod.width;
    t_pixel **imagenNueva;
    t_pixel **imagenVieja=imagenSt->imagePixelsMod;
    imagenNueva=(t_pixel**)crearMatrizGen(newHeight,newWidth,sizeof(t_pixel));
    if(imagenNueva==NULL)
        return false;
    for (int x = 0; x < oriHeight; x++)
    {
        for (int y = 0; y <oriWidth; y++)
            imagenNueva[y][oriHeight-1-x] = imagenVieja[x][y];
        //Nota mental: las imagenes bmp se leen de abajo hacia arriba y no de arriba a abajo (por alguna razon)
    }
    imagenSt->imagePixelsMod=imagenNueva;
    recalcularMetadatos(imagenSt,newHeight,newWidth);
    return true;
}
bool rotarDerecha (t_imageData *imagenSt)
{
    int newHeight=imagenSt->metadataMod.width,newWidth=imagenSt->metadataMod.height;
    int oriHeight=imagenSt->metadataMod.height,oriWidth=imagenSt->metadataMod.width;
    t_pixel **imagenNueva;
    t_pixel **imagenVieja=imagenSt->imagePixelsMod;
    imagenNueva=(t_pixel**)crearMatrizGen(newHeight,newWidth,sizeof(t_pixel));
    if(imagenNueva==NULL)
        return false;
    for (int x = 0; x < oriWidth; x++)
    {
        for (int y = 0; y < oriHeight; y++)
            imagenNueva[oriWidth - x - 1][y] = imagenVieja[y][x];
        //Nota mental: las imagenes bmp se leen de abajo hacia arriba y no de arriba a abajo (por alguna razon)
    }
    imagenSt->imagePixelsMod=imagenNueva;
    recalcularMetadatos(imagenSt,newHeight,newWidth);
    return true;
}
char achicar (t_imageData *imagenSt, int porcentaje)
{
    float multiplicador=(float)porcentaje/100;
    int newHeight,newWidth,oldHeight,oldWidth;
    t_pixel **newImage,**oldImage=imagenSt->imagePixelsMod;
    float escalaX, escalaY;
    if(porcentaje==100)
        return FILTRO_NULO; //achicar al 100% es no hacer nada asi que evitamos procesar la imagen en ese caso
    oldHeight=imagenSt->metadataMod.height;
    oldWidth=imagenSt->metadataMod.width;
    newHeight=oldHeight*multiplicador;
    newWidth=oldWidth*multiplicador;
    escalaX=(float)oldWidth/newWidth;
    escalaY=(float)oldHeight/newHeight;
    newImage=(t_pixel**)crearMatrizGen(newHeight,newWidth,sizeof(t_pixel));
    if(newImage==NULL)
        return ERROR_MEMORIA;
    for(int x=0; x<newHeight; x++)
    {
        for(int y=0; y<newWidth; y++)
            newImage[x][y]=oldImage[(int)(x*escalaX)][(int)(y*escalaY)];
    }
    imagenSt->imagePixelsMod=newImage;
    recalcularMetadatos(imagenSt,newHeight,newWidth);
    return FILTRO_APLICADO;
}
char recortar(t_imageData *imagenSt, int porcentaje)
{
    if(porcentaje==100)
        return FILTRO_NULO; //Evitamos el procesamiento inecesario, una imagen recortada al 100% es la imagen original.
    float multiplicador=(float)porcentaje/100;
    int newHeight,newWidth;
    t_pixel **newImage,**oldImage=imagenSt->imagePixelsMod;
    newHeight=imagenSt->metadataMod.height*multiplicador;
    newWidth=imagenSt->metadataMod.width*multiplicador;
    newImage=(t_pixel **)crearMatrizGen(newHeight,newWidth,sizeof(t_pixel));
    if(newImage==NULL)
        return ERROR_MEMORIA;
    for(int x=0; x<newHeight; x++)
    {
        for(int y=0; y<newWidth; y++)
            newImage[x][y]=oldImage[x][y];
    }
    imagenSt->imagePixelsMod=newImage;
    recalcularMetadatos(imagenSt,newHeight,newWidth);
    return FILTRO_APLICADO;
}
bool concatenarHorizontal (t_imageData *imagen1, t_imageData *imagen2)
{
    int height1=imagen1->metadataMod.height,width1=imagen1->metadataMod.width;
    int height2=imagen2->metadataMod.height,width2=imagen2->metadataMod.width;
    t_pixel **newImage, **img1=imagen1->imagePixelsMod,**img2=imagen2->imagePixelsMod;
    t_pixel dorado= {{59,131,189}};
    int flagDif=1,xGapStart,yGapStart,a,yGapFinish;
    int newHeight,newWidth;
    if(height1>height2)
    {
        newHeight=height1;
        xGapStart=height2;
        yGapStart=width1;
        yGapFinish=width1+width2;
    }
    else if(height2>height1)
    {
        newHeight=height2;
        xGapStart=height1;
        yGapStart=0;
        yGapFinish=width1;
    }
    else
    {
        newHeight=height1;
        flagDif=0;
    }
    newWidth=width1+width2;
    newImage=(t_pixel**)crearMatrizGen(newHeight,newWidth,sizeof(t_pixel));
    if(newImage==NULL)
        return false;
    for(int x=height1-1; x>=0; x--)
    {
        for(int y=0; y<width1; y++)
            newImage[x][y]=img1[x][y];
    }
    for(int x=height2-1; x>=0; x--)
    {
        a=width1;
        for(int y=0; y<width2; y++)
        {
            newImage[x][a]=img2[x][y];
            a++;
        }
    }
    if(flagDif)
    {
        for(int x=newHeight-1; x>=xGapStart; x--)
        {
            for(int y=yGapStart; y<yGapFinish; y++)
                newImage[x][y]=dorado;
        }
    }
    imagen1->imagePixelsMod=newImage;
    recalcularMetadatos(imagen1,newHeight,newWidth);
    return true;
}
bool concatenarVertical (t_imageData *imagen1, t_imageData *imagen2)
{
    t_pixel **canvas, **pincel1=imagen1->imagePixelsMod,**pincel2=imagen2->imagePixelsMod;
    int height1=imagen1->metadataMod.height,height2=imagen2->metadataMod.height;
    int width1=imagen1->metadataMod.width,width2=imagen2->metadataMod.width;
    int newHeight=height1+height2,newWidth;
    int xGapStart,xGapFinish,yGapStart,yGapFinish;
    bool flagDif=true;
    t_pixel dorado= {{59,131,189}};
    if(width1>width2)
    {
        newWidth=width1;
        yGapStart=height1;
        yGapFinish=newHeight;
        xGapStart=width2;
        xGapFinish=newWidth;
    }
    else if(width1<width2)
    {
        newWidth=width2;
        yGapStart=0;
        yGapFinish=height1;
        xGapStart=width1;
        xGapFinish=newWidth;
    }
    else
    {
        flagDif=false;
        newWidth=width1;
    }
    canvas=(t_pixel**)crearMatrizGen(newHeight,newWidth,sizeof(t_pixel));
    if(canvas==NULL)
        return false;
    for(int y=0;y<height1;y++)
    {
        for(int x=0;x<width1;x++)
            canvas[y][x]=pincel1[y][x];
    }
    for(int y=0;y<height2;y++)
    {
        for(int x=0;x<width2;x++)
            canvas[y+height1][x]=pincel2[y][x];
    }
    if(flagDif)
    {
        for(int y=yGapStart;y<yGapFinish;y++)
        {
            for(int x=xGapStart;x<xGapFinish;x++)
                canvas[y][x]=dorado;
        }
    }
    imagen1->imagePixelsMod=canvas;
    recalcularMetadatos(imagen1,newHeight,newWidth);
    return true;
}
//agregados reentrega
void procesarImagenesConcat (t_imageDataVector *imageStateDataVector, t_functionsData *ptrFun, bool filtro (t_imageData *imagen1, t_imageData *imagen2), const char *nombreGrupo,char *nombreImagenGuarBuff, t_imageData **concatenar1, t_imageData **concatenar2,bool mode, FILE **pfErr)
{
    //funcion procesadora para los filtros de concatenacion: --concatenar-vertical, --concatenar-horizontal
    if(encontrarImagenes(imageStateDataVector,concatenar1,concatenar2))
    {
        if(filtro(*concatenar1,*concatenar2))
        {
            crearNombreImagenGuardadoConcat((*concatenar1)->realName,(*concatenar2)->realName,ptrFun->functionName,nombreGrupo,nombreImagenGuarBuff);
            if(!crearArchivoImagen(*concatenar1,nombreImagenGuarBuff))
                mode==false?printf("ERROR: No se pudo crear el archivo %s\n",nombreImagenGuarBuff):
                    fprintf(*pfErr,"ERROR: No se pudo crear el archivo %s\n",nombreImagenGuarBuff);
            else
                mode==false?printf("La imagen %s fue creada exitosamente!\n",nombreImagenGuarBuff):
                    fprintf(*pfErr,"La imagen %s fue creada exitosamente!\n",nombreImagenGuarBuff);
            destruirMat((void**)(*concatenar1)->imagePixelsMod,(*concatenar1)->metadataMod.height);
            (*concatenar1)->imagePixelsMod=(*concatenar1)->imagenPixelsModPtr;
            restaurarImagenModificable(*concatenar1);
        }
        else
            mode==false?printf("Error: no se pudo asignar memoria dinamica para la imagen concatenada\n"):
                fprintf(*pfErr,"Error: no se pudo asignar memoria dinamica para la imagen concatenada\n");
    }
    else
        mode==false?printf("Nota: '%s' no se pudo ejecutar porque se necesitan al menos 2 imagenes cargadas\n",ptrFun->functionName):
            fprintf(*pfErr,"Nota: '%s' no se pudo ejecutar porque se necesitan al menos 2 imagenes cargadas\n",ptrFun->functionName);
    ptrFun->functionIsUsed=true;
}
bool isConfName (const char *arg)
{
    if(miStrcmp(arg,"filtros")==0)
        return true;
    else
        return false;
}
bool isErrName (const char *arg)
{
    if(miStrcmp(arg,"errores")==0)
        return true;
    else
        return false;
}
bool verificarArchivoConf(char *arg, t_archConfErrData *dest, t_archConfErrData *verif)
{
    FILE *pf;
    char *iniNom=NULL;
    char *iniExt=NULL;
    iniExt=miStrrchr(arg,'.');
    if(iniExt==NULL) //el archivo conf no tiene una extension asociada
        return false;
    iniExt++;
    if(miStrcmp(iniExt,"conf")!=0) //el archivo no tiene extension conf
        return false;
    iniNom=miStrrchr(arg,'=');
    iniNom++;
    pf=fopen(iniNom,"rt");
    if(pf==NULL)   //el archivo conf no existe
        return false;
    fclose(pf);
    if(verif->isValid&&miStrcmp(iniNom,verif->archNom)==0)
        return false; //el archivo existe pero es el de errores
    miStrcpy(dest->archNom,iniNom); //si llega aca es xq existe por tanto copiamos el nombre
    return true;
}
void inicializarConfErr (t_archConfErrData *vec)
{
    miStrcpy(vec->archNom,"");
    vec->isValid=false;
    vec++;
    miStrcpy(vec->archNom,"");
    vec->isValid=false;
}
bool obtenerNombreErr (char *arg, t_archConfErrData *dest, t_archConfErrData *verif)
{
    char *iniNom=NULL;
    char *iniExt=NULL;
    iniExt=miStrrchr(arg,'.');
    if(iniExt==NULL) //el archivo errores no tiene una extension asociada
        return false;
    iniExt++;
    if(miStrcmp(iniExt,"txt")!=0) //el archivo no tiene extension .txt
        return false;
    iniNom=miStrrchr(arg,'=');
    iniNom++;
    if(verif->isValid&&miStrcmp(iniNom,verif->archNom)==0)
        return false; //el archivo de errores se llama igual que el de conf.
    miStrcpy(dest->archNom,iniNom); //si llega aca es xq es un nombre valido, copiamos el nombre
    return true;
}
void limpiarArgConf (char *argConf)
{
    char *fin=argConf+miStrlen(argConf)-1;
    if(*fin=='\n') //el ultimo registro del archivo conf no va a tener salto de linea por eso esta este if.
        *fin='\0';
}
