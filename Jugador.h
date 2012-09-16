#ifndef __JUGADOR_H__
#define __JUGADOR_H__

#include <allegro.h>
#include <alleggl.h>

#include "Vector.h"
#include "Item.h"

class Nivel ;

class Jugador : public Item
{
private:
    Vector movimiento ;
    
    bool velocidadMuerte ;
    
public:
    Jugador(Nivel *nivel);
    Jugador(Vector posicion, Nivel *nivel);
    ~Jugador();
    void Actualizar();
    void Pintar(int etapa);    
    
    
};

#endif
