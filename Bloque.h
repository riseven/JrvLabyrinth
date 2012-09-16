#ifndef __BLOQUE_H__
#define __BLOQUE_H__

#include <allegro.h>
#include <alleggl.h>

#include "Item.h"
#include "Vector.h"

class Bloque : public Item
{
private:
    
public:
    Bloque(Nivel *nivel, Vector posicion);
    
};

#endif
