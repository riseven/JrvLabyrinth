#include <allegro.h>
#include <alleggl.h>
#include "Item.h"

#include "Excepcion.h"

Item::Item(Nivel *nivel, int codigo):
    nivel(nivel), codigo(codigo)
{
    try
    {
        posicion = Vector(0,0,0);
        orientacion = Vector(0,0,0);
        radio = Vector(0,0,0);
        obstaculo = false ;
        empujable = false ;
    }
    catch ( Excepcion &ex )
    {
        throw Excepcion(ex, "Item::~Item()");
    }
    catch ( exception &ex )
    {
        throw Excepcion(ex, "Item::~Item()");
    } 
}

Item::Item(Nivel *nivel, int codigo, Vector posicion):
    nivel(nivel), posicion(posicion), codigo(codigo)
{    
    try
    {
        orientacion = Vector(0,0,0);
        radio = Vector(0,0,0);
        obstaculo = false ;
    }
    catch ( Excepcion &ex )
    {
        throw Excepcion(ex, "Item::~Item(Vector)");
    }
    catch ( exception &ex )
    {
        throw Excepcion(ex, "Item::~Item(Vector)");
    } 
}

Item::~Item()
{
    try
    {
        posicion = Vector(0,0,0);
        orientacion = Vector(0,0,0);
    }
    catch ( Excepcion &ex )
    {
        throw Excepcion(ex, "Item::~Item()");
    }
    catch ( exception &ex )
    {
        throw Excepcion(ex, "Item::~Item()");
    } 
}

void
Item::SetPosicion(Vector posicion)
{
    Item::posicion = posicion ;
}

Vector
Item::GetPosicion()
{
    return posicion ;
}

void
Item::SetOrientacion(Vector orientacion)
{
    Item::orientacion = orientacion ;
}

Vector
Item::GetOrientacion()
{
    return orientacion ;
}

void
Item::SetRadio(Vector radio)
{
    Item::radio = radio ;
}

Vector
Item::GetRadio()
{
    return radio ;
}

void
Item::SetEsObstaculo(bool obstaculo)
{
    Item::obstaculo = obstaculo ;
}

bool
Item::GetEsObstaculo()
{
    return obstaculo ;
}

void
Item::GetBoundingBox(Vector &v1, Vector &v2)
{
    v1 = posicion - radio ;
    v2 = posicion + radio ;
}


//! Este método contiene toda la lógica de colisiones entre bounding boxs, no
//! es preciso que el objeto que colisiona sea un item, pero sí que el objeto
//! que limita el movimiento sea un item.
//! Por esto el jugador no sería necesario que fuera un item, pero si se quiere
//! poder limitar también el movimiento de los enemigos para que no se superpon-
//! gan al jugador, se puede considerar al jugador como un item más y de este
//! modo aprovechar el mismo motor de colisiones de forma generalizada.
bool
Item::Colision(Vector pos, Vector &mov, Vector rad)
{
    // Si el objeto no es un obstaculo, no se puede producir colision
    if ( !obstaculo )
    {
        return false ;
    }
    
    Vector b1, b2 ;
    Vector nb1, nb2 ;
    Vector box1, box2 ;
    GetBoundingBox(box1, box2);
    Vector npos = pos + mov ;
    b1 = pos - rad ;
    b2 = pos + rad ;    
    nb1 = npos - rad ;
    nb2 = npos + rad ;
    
    if ( ( nb1.GetX() >= box2.GetX() || nb2.GetX() <= box1.GetX() ) ||
         ( nb1.GetY() >= box2.GetY() || nb2.GetY() <= box1.GetY() ) ||
         ( nb1.GetZ() >= box2.GetZ() || nb2.GetZ() <= box1.GetZ() ) )
    {
        return false ;
    }

    /*
    // Se produce colision, nos aseguramos de que no exista colisión previa
    if (!(b1.GetX() > box2.GetX() || b2.GetX() < box1.GetX() ||
         b1.GetY() > box2.GetY() || b2.GetY() < box1.GetY() ||
         b1.GetZ() > box2.GetZ() || b2.GetZ() < box1.GetZ() ) )
    {
        // Si existia colision previa es un bug, expulsamos el objeto del area
        // de interseccion
        // Esto es menos frecuente (es un bug de hecho) que el caso anterior, 
        // por eso se comprueba después
        
        // TO DO (Si es necesario)
        
        // No se considera colisión esto, ya que podría llevar a más situaciones
        // anómalas, por ese motivo se devuelve false, y en todo caso en otra
        // iteración ya se producira una colisión correcta vertical

        return false ;
    }
    */

    
    // Se ha producido una nueva colisión, se deben truncar los desplazamientos
    // en cada eje para que no se interseccionen los bounding boxs
    
    bool colision = false ;
    
    // Eje X
    b1 = pos - rad ;
    b2 = pos + rad ;
    nb1 = npos - rad ;
    nb2 = npos + rad ;
    if ( pos.GetX() < posicion.GetX() )
    {
        // Movimiento positivo, se colisiona el punto nb2 con box1
        // Hay que truncar si cambia el signo de la diferencia de coordenadas
        if ( nb2.GetX() > box1.GetX() && b2.GetX() <= box1.GetX() )
        {
            npos.SetX( box1.GetX()-rad.GetX() );
            colision = true ;
        }
    }
    else
    {
        // Movimiento negativo, se colisiona el punto nb1 con box2
        // Hay que truncar si cambia el signo de la diferencia de coordenadas
        if ( nb1.GetX() < box2.GetX() && b1.GetX() >= box2.GetX() )
        {
            npos.SetX( box2.GetX()+rad.GetX() );
            colision = true ;
        }
    }        
    
    // Eje Y
    b1 = pos - rad ;
    b2 = pos + rad ;
    nb1 = npos - rad ;
    nb2 = npos + rad ;
    if ( pos.GetY() < posicion.GetY() )
    {
        // Movimiento positivo, se colisiona el punto nb2 con box1
        // Hay que truncar si cambia el signo de la diferencia de coordenadas
        if ( nb2.GetY() > box1.GetY() && b2.GetY() <= box1.GetY() )
        {
            npos.SetY( box1.GetY()-rad.GetY() );
            colision = true ;
        }
    }
    else
    {
        // Movimiento negativo, se colisiona el punto nb1 con box2
        // Hay que truncar si cambia el signo de la diferencia de coordenadas
        if ( nb1.GetY() < box2.GetY() && b1.GetY() >= box2.GetY() )
        {
            npos.SetY( box2.GetY()+rad.GetY() );
            colision = true ;
        }
    }       
    
    // Eje Z
    b1 = pos - rad ;
    b2 = pos + rad ;
    nb1 = npos - rad ;
    nb2 = npos + rad ;
    if ( pos.GetZ() < posicion.GetZ() )
    {
        // Movimiento positivo, se colisiona el punto nb2 con box1
        // Hay que truncar si cambia el signo de la diferencia de coordenadas
        if ( nb2.GetZ() > box1.GetZ() && b2.GetZ() <= box1.GetZ() )
        {
            npos.SetZ( box1.GetZ()-rad.GetZ() );
            colision = true ;
        }
    }
    else
    {
        // Movimiento negativo, se colisiona el punto nb1 con box2
        // Hay que truncar si cambia el signo de la diferencia de coordenadas
        if ( nb1.GetZ() < box2.GetZ() && b1.GetZ() >= box2.GetZ() )
        {
            npos.SetZ( box2.GetZ()+rad.GetZ() );
            colision = true ;
        }
    }              
    
    // Calculamos como queda el vector de movimiento resultante
    mov = npos - pos ;
    return colision ;    
}

bool
Item::Empujar(Vector pos, Vector mov, Vector rad)
{
    // Si el objeto no es empujable, no lo empuja :D
    if ( !empujable )
    {
        return false ;
    }
    

    
    Vector b1, b2 ;
    Vector nb1, nb2 ;
    Vector box1, box2 ;
    GetBoundingBox(box1, box2);
    Vector npos = pos + mov ;
    b1 = pos - rad ;
    b2 = pos + rad ;    
    nb1 = npos - rad ;
    nb2 = npos + rad ;
    
    if ( ( nb1.GetX() >= box2.GetX() || nb2.GetX() <= box1.GetX() ) ||
         ( nb1.GetY() >= box2.GetY() || nb2.GetY() <= box1.GetY() ) ||
         ( nb1.GetZ() >= box2.GetZ() || nb2.GetZ() <= box1.GetZ() ) )
    {
        return false ;
    }

    // Se ha producido una nueva colisión, se deben empujar al item para que 
    // deje de haber colisión
    
    bool colision = false ;
    
    // Eje X
    b1 = pos - rad ;
    b2 = pos + rad ;
    nb1 = npos - rad ;
    nb2 = npos + rad ;
    if ( pos.GetX() < posicion.GetX() )
    {
        // Movimiento positivo, se colisiona el punto nb2 con box1
        // Hay que empujar si cambia el signo de la diferencia de coordenadas
        if ( nb2.GetX() > box1.GetX() && b2.GetX() <= box1.GetX() )
        {
            npos.SetX( box1.GetX()-rad.GetX() );
            colision = true ;
        }
    }
    else
    {
        // Movimiento negativo, se colisiona el punto nb1 con box2
        // Hay que truncar si cambia el signo de la diferencia de coordenadas
        if ( nb1.GetX() < box2.GetX() && b1.GetX() >= box2.GetX() )
        {
            npos.SetX( box2.GetX()+rad.GetX() );
            colision = true ;
        }
    }        
    
    // Eje Y
    b1 = pos - rad ;
    b2 = pos + rad ;
    nb1 = npos - rad ;
    nb2 = npos + rad ;
    if ( pos.GetY() < posicion.GetY() )
    {
        // Movimiento positivo, se colisiona el punto nb2 con box1
        // Hay que truncar si cambia el signo de la diferencia de coordenadas
        if ( nb2.GetY() > box1.GetY() && b2.GetY() <= box1.GetY()+0.05 )
        {
            posicion.SetY( nb2.GetY() + radio.GetY() );
//            npos.SetY( box1.GetY()-rad.GetY() );
            colision = true ;
        }
    }
    else
    {
        // Movimiento negativo, se colisiona el punto nb1 con box2
        // Hay que truncar si cambia el signo de la diferencia de coordenadas
        if ( nb1.GetY() < box2.GetY() && b1.GetY() >= box2.GetY() )
        {
            npos.SetY( box2.GetY()+rad.GetY() );
            colision = true ;
        }
    }       
    
    // Eje Z
    b1 = pos - rad ;
    b2 = pos + rad ;
    nb1 = npos - rad ;
    nb2 = npos + rad ;
    if ( pos.GetZ() < posicion.GetZ() )
    {
        // Movimiento positivo, se colisiona el punto nb2 con box1
        // Hay que truncar si cambia el signo de la diferencia de coordenadas
        if ( nb2.GetZ() > box1.GetZ() && b2.GetZ() <= box1.GetZ() )
        {
            npos.SetZ( box1.GetZ()-rad.GetZ() );
            colision = true ;
        }
    }
    else
    {
        // Movimiento negativo, se colisiona el punto nb1 con box2
        // Hay que truncar si cambia el signo de la diferencia de coordenadas
        if ( nb1.GetZ() < box2.GetZ() && b1.GetZ() >= box2.GetZ() )
        {
            npos.SetZ( box2.GetZ()+rad.GetZ() );
            colision = true ;
        }
    }              
    
    // Calculamos como queda el vector de movimiento resultante
    mov = npos - pos ;
    return colision ;    
}


void
Item::Actualizar()
{
}

void
Item::Pintar(int etapa)
{
}

