#ifndef __ASCENSOR_H__
#define __ASCENSOR_H__

#include <allegro.h>
#include <alleggl.h>

#include "Item.h"

class Ascensor : public Item
{
private:
    Vector posicionReposo ;
    Vector posicionExcitado ;
    int estado ;
    static int glTexAscensor;
    
public:
    enum Enum
    {
        Excitado,
        Reposo
    };
    Ascensor(Nivel *nivel, int cod, Vector centroCelda, Vector radioCelda, Vector alturaReposo, Vector alturaExcitado);
    ~Ascensor();
    bool EsAscensor(){return true;}
    void Actualizar();
    void Pintar(int etapa);
    void Excitar();
    void Desexcitar();
    void Intercambiar();
};

#endif
