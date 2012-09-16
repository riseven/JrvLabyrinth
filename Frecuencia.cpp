#include "Frecuencia.h"
#include <windows.h>

Frecuencia::Frecuencia()
{
  periodo=-1;
  ultimo=GetTickCount();
}
        
Frecuencia::Frecuencia(float f)
{
    ultimo = GetTickCount() ;
  if (f<0) periodo=-1;
  else
      periodo=1000/f;
      
}

/*
 * Devuelve true si es necesario comprobar 
*/
bool
Frecuencia::comprobar()
{
   unsigned int temp=GetTickCount();
   
   //Si ya ha expirado, actualizar
   if (periodo<=(temp - ultimo))
   {
      ultimo=ultimo+periodo;
      return true;
      
   }else
      return false;
}

void
Frecuencia::setFrec(float f)
{
    if (f<0) periodo=-1;
      else
        periodo=1000/f;
}

