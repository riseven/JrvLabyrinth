#ifndef __LOSA_H__
#define __LOSA_H__

#include <allegro.h>
#include <alleggl.h>

#include "Item.h"
#include "Ascensor.h"

class Losa : public Item
{
private:
    static int glTextura ;
    
    int objetivo ;
    int tipoActuador ;
    
    bool estabaDentro ;
    
    float angulo ;
    float velAng ;
    
    bool ascensorBuscado ;
    Ascensor *ascensorLinkado ;
    int estado ;
public:
    static void Inicializar();
    static void Cerrar();
    
    enum Estado
    {
        Activada,
        Desactivada
    };
    
    enum EnumTipoActuador
    {
        Excitador,
        Desexcitador,
        Intercambiador
    };
    
    Losa(Nivel *nivel, int cod, Vector posicion, Vector radio, int objetivo, int tipoActuador);
    
        
    
    void Actualizar();
    void Pintar( int etapaRender );
    
    void Excitar(){estado = Activada;}
    void Desexcitar(){estado = Desactivada;}
    void Intercambiar(){estado=(estado==Activada?Desactivada:Activada);}
};

#endif
