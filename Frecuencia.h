/*
 * Esta clase guarda frecuencias de ejecuci�n
 * medidas con funciones de tiempo, para independizar
 * las animaciones de la velocidad de la CPU
 *
 * Se llamara a comprobar() y s�lo devolver�
 * true con la frecuencia definida en la clase
 * 
 *
*/
#ifndef __L_FRECREAL_H__
#define __L_FRECREAL_H__
     
class Frecuencia
{
      public:
             Frecuencia();
             Frecuencia(float f);
             bool comprobar();
             void setFrec(float f);
      
      private:
              float periodo;//Guarda milisegundos
              unsigned int ultimo;
};


#endif
