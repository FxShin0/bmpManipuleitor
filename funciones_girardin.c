#include "funciones_girardin.h"
bool isImageName (const char *arg)
{
    const char *aux;
    int len;
    char ext[]=".bmp";
    char *extPtr=ext+3;
    len=miStrlen(arg);
    aux=arg+len-1;
    if(len<4)
        return false;
    while(aux>arg&&extPtr>ext)
    {
        if(*aux!=*extPtr)
            return false;
        aux--;
        extPtr--;
    }
    //ultima comparacion manual para evitar que algun puntero se vaya a memoria invalida
    if(*aux!=*extPtr)
        return false;
    return true;
}
bool isDuplicatedName (const t_imageData *names, const int ce,const char *name)
{
    for(int x=0; x<ce; x++)
    {
        if(miStrcmp(names->name,name)==0)
            return true;
        names++;
    }
    return false;
}
bool obtenerPorcentaje (const char *arg, int *porcentaje)
{
    const char *lectura,*numero;
    int aux;
    lectura=miStrrchr(arg,'=');
    lectura++;
    numero=lectura;
    if(*lectura=='\0')
        return false; //significa que el argumento era del tipo --funcion=
    while(*lectura)
    {
        if(!esNumero(*lectura))
            return false;
        lectura++;
    }
    aux=atoi(numero);
    if(aux<0||aux>100)
        return false;
    *porcentaje=aux;
    return true;
}
bool esNumero (char c)
{
    if(c>='0'&&c<='9')
        return true;
    return false;
}
void inicializarDatosFunciones (t_functionsData *datosFunciones)
{
    //Aca se deben agregar los nombres de funciones (y se debe aumentar el valor de FUNCIONES en el define de estructuras.h
    //1.escala-de-grises
    miStrcpy(datosFunciones->functionName,"escala-de-grises");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //2. espejar-horizontal
    miStrcpy(datosFunciones->functionName,"espejar-horizontal");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //3. espejar-vertical
    miStrcpy(datosFunciones->functionName,"espejar-vertical");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //4. rotar-derecha
    miStrcpy(datosFunciones->functionName,"rotar-derecha");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //5. rotar-izquierda
    miStrcpy(datosFunciones->functionName,"rotar-izquierda");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //6. aumentar-contraste
    miStrcpy(datosFunciones->functionName,"aumentar-contraste");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //7. reducir-contraste
    miStrcpy(datosFunciones->functionName,"reducir-contraste");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //8. tonalidad-azul
    miStrcpy(datosFunciones->functionName,"tonalidad-azul");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //9. tonalidad-verde
    miStrcpy(datosFunciones->functionName,"tonalidad-verde");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //10. tonalidad-roja
    miStrcpy(datosFunciones->functionName,"tonalidad-roja");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //11. recortar
    miStrcpy(datosFunciones->functionName,"recortar");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //12. achicar
    miStrcpy(datosFunciones->functionName,"achicar");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //13. concatenar-horizontal
    miStrcpy(datosFunciones->functionName,"concatenar-horizontal");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //14. negativo
    miStrcpy(datosFunciones->functionName,"negativo");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //14. comodin
    miStrcpy(datosFunciones->functionName,"comodin");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
    datosFunciones++;
    //15. concatenar-vertical
    miStrcpy(datosFunciones->functionName,"concatenar-vertical");
    datosFunciones->functionIsUsed=false;
    datosFunciones->porcentaje=0;
}
void restaurarImagenModificable (t_imageData *imagen)
{
    t_pixel **ptrImgMod=imagen->imagePixelsMod;
    t_pixel **ptrImg=imagen->imagePixels;
    for(int x=0; x<imagen->metadata.height; x++)
    {
        memcpy(*ptrImgMod,*ptrImg,sizeof(t_pixel)*imagen->metadata.width);
        ptrImgMod++;
        ptrImg++;
    }
    imagen->metadataMod=imagen->metadata;
    imagen->paddingMod=imagen->padding;
}
unsigned char calcularPadding (const unsigned int width)
{
    return (4-(width*sizeof(t_pixel)%4))%4;
}
void procesarImagenesFiltroArg1 (t_imageDataVector *imageStateDataVector, const t_functionsData *datosFunciones, unsigned char argToNumberRet,void filtro(t_imageData *imagen, int porcentaje), const char *nombreGrupo,char *nombreImagenGuarBuff,bool mode, FILE **pfErr)
{
    //funcion procesadora para aquellos filtros que trabajan con una sola imagen, no modifican metadatos y tienen 1 argumento de entrada
    //sirve para aumentar contraste, reducir contraste, tonalidad azul, tonalidad verde, tonalidad roja
    const t_functionsData *funPtr=datosFunciones+argToNumberRet;
    t_imageData *ptrVec=imageStateDataVector->vec;
    for(int y=0; y<imageStateDataVector->ce; y++)
    {
        if(ptrVec->isLoaded)
        {
            filtro(ptrVec,funPtr->porcentaje);
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
void recalcularMetadatos (t_imageData *imagen, int newHeight, int newWidth)
{
    imagen->metadataMod.height=newHeight;
    imagen->metadataMod.width=newWidth;
    imagen->paddingMod=calcularPadding(newWidth);
    imagen->metadataMod.imageSize=newHeight*(newWidth*sizeof(t_pixel)+imagen->paddingMod);
    imagen->metadataMod.fileSize=imagen->metadataMod.imageSize+imagen->metadataMod.bmpHeaderSize;
}
void aumentarContraste (t_imageData *imagenSt, int porcentaje)
{
    if(porcentaje==0) //nos ahorramos el procesamiento si el contraste es 0
        return;
    float p=1+(float)porcentaje/100;
    t_pixel **imagen=imagenSt->imagePixelsMod;
    unsigned char *pixelPtr=NULL; //igual esto en realidad sirve para accesar un color dentro del pixel
    int height=imagenSt->metadataMod.height, width=imagenSt->metadataMod.width;
    for(int x=0; x<height; x++)
    {
        for(int y=0; y<width; y++)
        {
            pixelPtr=imagen[x][y].pixel;
            for(int z=0; z<3; z++)
            {
                if(*pixelPtr>127)
                {
                    if((*pixelPtr)*p>255)
                        *pixelPtr=255;
                    else
                        (*pixelPtr)*=p;
                }
                else
                    (*pixelPtr)/=p;
                pixelPtr++;
            }
        }
    }
}

void reducirContraste (t_imageData *imagenSt, int porcentaje)
{
    if(porcentaje==0)
        return;
    float p=1+(float)porcentaje/100;
    t_pixel **imagen=imagenSt->imagePixelsMod;
    unsigned char *pixelPtr=NULL;
    int height=imagenSt->metadataMod.height, width=imagenSt->metadataMod.width;
    for(int x=0; x<height; x++)
    {
        for(int y=0; y<width; y++)
        {
            pixelPtr=imagen[x][y].pixel;
            for(int z=0; z<3; z++)
            {
                if(*pixelPtr>127)
                {
                    if((*pixelPtr)/p<127)
                        *pixelPtr=127;
                    else
                        (*pixelPtr)/=p;
                }
                else if((*pixelPtr)*p>127)
                    *pixelPtr=127;
                else
                    (*pixelPtr)*=p;
                pixelPtr++;
            }
        }
    }
}
void tonalidadAzul (t_imageData *imagenSt, int porcentaje)
{
    float p=1+(float)porcentaje/100;
    t_pixel **imagen=imagenSt->imagePixelsMod;
    unsigned char *pixelPtr=NULL;
    int height=imagenSt->metadataMod.height, width=imagenSt->metadataMod.width;
    for(int x=0; x<height; x++)
    {
        for(int y=0; y<width; y++)
        {
            pixelPtr=imagen[x][y].pixel;
            if((*pixelPtr)*p>255)
                *pixelPtr=255;
            else
                (*pixelPtr)*=p;
        }
    }
}
void tonalidadVerde (t_imageData *imagenSt, int porcentaje)
{
    float p=1+(float)porcentaje/100;
    t_pixel **imagen=imagenSt->imagePixelsMod;
    unsigned char *pixelPtr=NULL;
    int height=imagenSt->metadataMod.height, width=imagenSt->metadataMod.width;
    for(int x=0; x<height; x++)
    {
        for(int y=0; y<width; y++)
        {
            pixelPtr=imagen[x][y].pixel+1;
            if((*pixelPtr)*p>255)
                *pixelPtr=255;
            else
                (*pixelPtr)*=p;
        }
    }
}
void tonalidadRoja (t_imageData *imagenSt, int porcentaje)
{
    float p=1+(float)porcentaje/100;
    t_pixel **imagen=imagenSt->imagePixelsMod;
    unsigned char *pixelPtr=NULL;
    int height=imagenSt->metadataMod.height, width=imagenSt->metadataMod.width;
    for(int x=0; x<height; x++)
    {
        for(int y=0; y<width; y++)
        {
            pixelPtr=imagen[x][y].pixel+2;
            if((*pixelPtr)*p>255)
                *pixelPtr=255;
            else
                (*pixelPtr)*=p;
        }
    }
}
void comodin (t_imageData *imagenSt)
{
    t_pixel **imagen=imagenSt->imagePixelsMod;
    unsigned char *pixelPtr=NULL;
    int height=imagenSt->metadataMod.height, width=imagenSt->metadataMod.width;
    for(int x=0; x<height; x++)
    {
        for(int y=0; y<width; y++)
        {
            pixelPtr=imagen[x][y].pixel;
            for(int z=0; z<3; z++)
            {
                if(*pixelPtr==255)
                    pixelPtr++;
                else
                {
                    *pixelPtr=(*pixelPtr)/64;
                    (*pixelPtr)*=64;
                    pixelPtr++;
                }

            }

        }
    }
}
