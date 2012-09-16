#include <alleggl.h>
#include <allegro.h>
#include "Luz.h"

Luz::Luz(int cod, Vector pos, int r, int g, int b, int atenuacion)
{
    posicion[0] = pos.GetX();
    posicion[1] = pos.GetY();
    posicion[2] = pos.GetZ();
    posicion[3] = 1.0 ;
    
    codigo = GL_LIGHT0 + cod ;
    
    color[0] = r/255.0 ;
    color[1] = g/255.0 ;
    color[2] = b/255.0 ;
    color[3] = 1.0 ;
    
    linearAtenuacion = atenuacion / 10.0 ;
}

Luz::~Luz()
{
    glDisable(codigo);
}

void
Luz::Pintar()
{
    glEnable(codigo);
    
    float cero[4] = {0,0,0,0} ;
    float dif[4] ;
    dif[0] = color[0] * 0.4 ;
    dif[2] = color[1] * 0.4 ;
    dif[2] = color[2] * 0.4 ;
    dif[4] = 1.0 ;
    
    glLightfv(codigo, GL_AMBIENT, color);
    glLightfv(codigo, GL_DIFFUSE, color);
    glLightfv(codigo, GL_SPECULAR, color);
    
    glLightfv(codigo, GL_POSITION, posicion);
    //glLightf(codigo, GL_QUADRATIC_ATTENUATION, 1.0);
    glLightf(codigo, GL_LINEAR_ATTENUATION, linearAtenuacion);
}
