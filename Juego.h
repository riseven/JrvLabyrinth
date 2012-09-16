#ifndef __JUEGO_H__
#define __JUEGO_H__

#include <allegro.h>
#include <alleggl.h>

#include "Nivel.h"

class Juego
{
private:
    Nivel *nivelActual ;
    int numNivel ;
    
public:
    static void Inicializar() ;
    static void Cerrar();
    
    Juego();
    ~Juego();
    void BuclePrincipal();
    
    
    
    
};

#endif
