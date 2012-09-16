#ifndef __LUZ_H__
#define __LUZ_H__

#include <alleggl.h>
#include <allegro.h>

#include "Vector.h"

class Luz
{
private:
    float color[4] ;
    float posicion[4] ;
    float linearAtenuacion ;
    int codigo ;    
    
public:
    Luz(int cod, Vector pos, int r, int g, int b, int atenuacion);
    ~Luz();
    void Pintar();    
};

#endif
