#include "funciones_mussipulcini.h"
void negativo (t_imageData *imagenSt)
{
    t_pixel **img=imagenSt->imagePixelsMod;
    unsigned char *ptrAux;
    int height=imagenSt->metadataMod.height, width=imagenSt->metadataMod.width;
    for(int x=0; x<height; x++)
    {
        for(int y=0; y<width; y++)
        {
            ptrAux=img[x][y].pixel;
            for(int z=0; z<3; z++)
            {
                *ptrAux=255-*ptrAux;
                ptrAux++;
            }
        }
    }
}

void mensajeDeErrorPorcentajes (const char *arg)
{
    const char *errorValue;
    errorValue=miStrrchr(arg,'=');
    errorValue++;
    *errorValue=='\0'? printf("'%s' No contiene un porcentaje de aumento\n",arg):printf("'%s' contiene un porcentaje no valido para la funcion llamada.\n",arg);
}


void greyFilter (t_imageData *imagenSt)
{
    int acum=0;
    t_pixel **imagen=imagenSt->imagePixelsMod;
    unsigned char *pixelPtr=NULL;
    int height=imagenSt->metadata.height, width=imagenSt->metadata.width;
    for(int x=0; x<height; x++)
    {
        for(int y=0; y<width; y++)
        {
            pixelPtr=imagen[x][y].pixel;
            for(int z=0; z<3; z++)
            {
                acum+=*pixelPtr;
                pixelPtr++;
            }
            pixelPtr=imagen[x][y].pixel;
            for(int z=0; z<3; z++)
            {
                *pixelPtr=acum/3;
                pixelPtr++;
            }
            acum=0;
        }
    }
}
void espejarVertical (t_imageData *imagenSt)
{
    int height=imagenSt->metadata.height, width=imagenSt->metadata.width;
    t_pixel **imagen=imagenSt->imagePixelsMod;
    int filTop,filBot;
    filTop=0;
    filBot=height-1;
    for(int x=0; x<width; x++)
    {
        while(filBot>filTop)
        {
            intercambiarPixeles(&imagen[filTop][x],&imagen[filBot][x]);
            filTop++;
            filBot--;
        }
        filTop=0;
        filBot=height-1;
    }
}
void espejarHorizontal (t_imageData *imagenSt)
{
    t_pixel *fin,*ini;
    int height=imagenSt->metadata.height, width=imagenSt->metadata.width;
    t_pixel **imagen=imagenSt->imagePixelsMod;
    for(int x=0; x<height; x++)
    {
        ini=*imagen;
        fin=*imagen+width-1;
        while(fin>ini)
        {
            intercambiarPixeles(ini,fin);
            ini++;
            fin--;
        }
        imagen++;
    }
}
void intercambiarPixeles (t_pixel *p1, t_pixel *p2)
{
    t_pixel aux;
    unsigned char *pixelPtrAux=aux.pixel;
    unsigned char *pixelPtr1=p1->pixel;
    unsigned char *pixelPtr2=p2->pixel;
    for(int x=0; x<3; x++)
    {
        *pixelPtrAux=*pixelPtr1;
        *pixelPtr1=*pixelPtr2;
        *pixelPtr2=*pixelPtrAux;
        pixelPtrAux++;
        pixelPtr1++;
        pixelPtr2++;
    }
}


char *miStrrchr (const char *arg, char c)
{
    int len=miStrlen(arg);
    if(len==0)
        return NULL;
    const char *fin=arg+len-1;
    while(fin!=arg&&*fin!=c)
        fin--;
    if(*fin!=c)
        return NULL;
    return (char*)fin;
}
char *obtenerNombreRealImg (const char *arg)
{
    const char *nom=miStrrchr(arg,'\\');
    if(nom==NULL)
        return (char*)arg;
    else
        return (char*)nom+1;
}
void crearNombreImagenGuardado (const char *nombreImagen,const char *nombreFiltro, const char *nombreGrupo, char *nombreImagenGuarBuff)
{
    sprintf(nombreImagenGuarBuff,"%s_%s_%s",nombreGrupo,nombreFiltro,nombreImagen);
}
void crearNombreImagenGuardadoConcat(char *nombreImagen1,const char *nombreImagen2,const char *nombreFiltro, const char *nombreGrupo, char *iniNombreImg)
{
    char *finFalso=miStrrchr(nombreImagen1,'.'),aux;
    aux=*finFalso;
    *finFalso='\0';
    sprintf(iniNombreImg,"%s_%s_%s_%s",nombreGrupo,nombreFiltro,nombreImagen1,nombreImagen2);
    *finFalso=aux;
}

void miStrcpy (char *dest, const char *ori)
{
    while(*ori)
    {
        *dest=*ori;
        dest++;
        ori++;
    }
    *dest='\0';
}
char argToNumber (char *arg, const t_functionsData *ptr)
{
    char *fin=arg+miStrlen(arg)-1;
    char aux;
    while(*arg=='-'&&*arg)
        arg++;
    fin=miStrrchr(arg,'=');
    if(fin!=NULL)
    {
        aux=*fin;
        *fin='\0';
    }
    for(int x=0; x<FUNCTIONS; x++)
    {
        if(miStrcmp(ptr->functionName,arg)==0)
        {
            if(fin!=NULL)
                *fin=aux;
            return x;
        }
        ptr++;
    }
    if(isImageName(arg))
        return -1;
    else if(isConfName(arg))
    {
        *fin=aux;
        return -3;
    }
    else if(isErrName(arg))
    {
        *fin=aux;
        return -4;
    }
    else
        return -2;
}
int miStrcmp (const char *s1, const char *s2)
{
    //si s1 es igual que s2 devuelve 0
    //si s1 es mayor que s2 devuelve >0
    //si s1 es menor que s2 devuelve <0
    while(*s1&&*s2)
    {
        if(aMinus(*s1)!=aMinus(*s2))
            return aMinus(*s1)-aMinus(*s2);
        s1++;
        s2++;
    }
    if(*s1||*s2)
        return aMinus(*s1)-aMinus(*s2);
    return 0;
}
char aMinus (char c)
{
    if(c>='A'&&c<='Z')
        return c+=32;
    return c;
}


int miStrlen (const char *string)
{
    int len=0;
    while(*string)
    {
        len++;
        string++;
    }
    return len;
}
