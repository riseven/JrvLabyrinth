#ifndef __PUERTA_H__
#define __PUERTA_H__

#include <allegro.h>
#include <alleggl.h>

#include "Item.h"

class Puerta : public Item
{
private:
    Vector posicionCerrada ;
    Vector posicionAbierta ;
    int estado ;
    static int glTexPuerta;
    int orientacion;
    
public:
    enum Enum
    {
        Abierta,
        Cerrada
    };
    Puerta(Nivel *nivel, int cod, Vector centroCelda, Vector radioCelda, int orien);
    ~Puerta();
    void Actualizar();
    void Pintar(int etapa);
    void Excitar();
    void Desexcitar();
    void Intercambiar();
};

#endif
