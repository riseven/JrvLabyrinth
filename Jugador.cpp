#include <allegro.h>
#include <alleggl.h>
#include "Jugador.h"

#include "Excepcion.h"
#include "Nivel.h"
#include "EtapaRender.h"
#include <cmath>

#define PIOVER180 0.0174532925

Jugador::Jugador(Nivel *nivel):
    Item(nivel, -1)
{
    try
    {
        if ( !nivel )
        {
            throw Excepcion("Error");
        }
        empujable = true ;
        velocidadMuerte = false ;
    }
    catch ( Excepcion &ex )
    {
        throw Excepcion(ex, "Jugador::Jugador(Nivel *)");
    }
    catch ( exception &ex )
    {
        throw Excepcion(ex, "Jugador::Jugador(Nivel *)");
    }
}

Jugador::Jugador(Vector posicion, Nivel *nivel):
    Item(nivel, -1, posicion)
{
    try
    {
        if ( !nivel )
        {
            throw Excepcion("Error");
        }
    }
    catch ( Excepcion &ex )
    {
        throw Excepcion(ex, "Jugador::Jugador(Vector, Nivel *)");
    }
    catch ( exception &ex )
    {
        throw Excepcion(ex, "Jugador::Jugador(Vector, Nivel *)");
    }
}

Jugador::~Jugador()
{
}

void
Jugador::Actualizar()
{
    int x, y;
    get_mouse_mickeys(&x, &y);
    int dx, dy ;
    dx = orientacion.GetX() + y ;
    if ( dx < -85 )
    {
        dx = -85 ;
    }
    if ( dx > 85 )
    {
        dx = 85 ;
    }
    dy = ((int)(orientacion.GetY() + x))%360 ;

    orientacion.SetX(dx);
    orientacion.SetY(dy);
    
    Vector mov(0,0,0);
    
    if ( movimiento.GetY() > -0.1 )
    {
        if ( key[KEY_UP] )
        {
            mov.SetX(mov.GetX()+ 0.1 * cos((orientacion.GetY()-90)*PIOVER180) );
            mov.SetZ(mov.GetZ()+ 0.1 * sin((orientacion.GetY()-90)*PIOVER180) );
        }
        if ( key[KEY_DOWN] )
        {
            mov.SetX(mov.GetX()+ 0.1 * cos((orientacion.GetY()+90)*PIOVER180) );
            mov.SetZ(mov.GetZ()+ 0.1 * sin((orientacion.GetY()+90)*PIOVER180) );
        }       
        if ( key[KEY_RIGHT] )
        {
            mov.SetX(mov.GetX()+ 0.1 * cos((orientacion.GetY())*PIOVER180) );
            mov.SetZ(mov.GetZ()+ 0.1 * sin((orientacion.GetY())*PIOVER180) );
        }
        if ( key[KEY_LEFT] )
        {
            mov.SetX(mov.GetX()+ 0.1 * cos((orientacion.GetY()+180)*PIOVER180) );
            mov.SetZ(mov.GetZ()+ 0.1 * sin((orientacion.GetY()+180)*PIOVER180) );
        }
    }
    movimiento.SetX( mov.GetX() );
    movimiento.SetZ( mov.GetZ() );
    movimiento.SetY( movimiento.GetY() - 0.005 );

    if ( movimiento.GetY() < -0.3 )
    {
        velocidadMuerte = true ;
    }
    nivel->DesplazarItem(posicion, movimiento, radio);
    posicion = posicion + movimiento ;    
    
    if ( ((movimiento.GetY() > -0.1 ) && velocidadMuerte) || posicion.GetY() < 0.0 )
    {
        nivel->MuereJugador();
    }
}

void
Jugador::Pintar(int etapa)
{
    if ( etapa == EtapaRender::Normal )
    {
        glColor4f(0.0, 1.0, 1.0, 1.0);
        glBegin(GL_QUADS);
        
            glVertex3f( posicion.GetX() - 2, posicion.GetY(), posicion.GetZ() - 2);
            glVertex3f( posicion.GetX() + 2, posicion.GetY(), posicion.GetZ() - 2);
            glVertex3f( posicion.GetX() - 2, posicion.GetY(), posicion.GetZ() + 2);
            glVertex3f( posicion.GetX() + 2, posicion.GetY(), posicion.GetZ() + 2);
            
            glVertex3f( posicion.GetX() - 2, posicion.GetY() + 2, posicion.GetZ() - 2);
            glVertex3f( posicion.GetX() + 2, posicion.GetY() + 2, posicion.GetZ() - 2);
            glVertex3f( posicion.GetX() - 2, posicion.GetY() + 2, posicion.GetZ() + 2);
            glVertex3f( posicion.GetX() + 2, posicion.GetY() + 2, posicion.GetZ() + 2);
            
            glVertex3f( posicion.GetX() - 2, posicion.GetY() + 2, posicion.GetZ() - 2);
            glVertex3f( posicion.GetX() - 2, posicion.GetY() + 2, posicion.GetZ() + 2);
            glVertex3f( posicion.GetX() - 2, posicion.GetY() - 2, posicion.GetZ() - 2);
            glVertex3f( posicion.GetX() - 2, posicion.GetY() - 2, posicion.GetZ() + 2);
            
            glVertex3f( posicion.GetX() + 2, posicion.GetY() + 2, posicion.GetZ() - 2);
            glVertex3f( posicion.GetX() + 2, posicion.GetY() + 2, posicion.GetZ() + 2);
            glVertex3f( posicion.GetX() + 2, posicion.GetY() - 2, posicion.GetZ() - 2);
            glVertex3f( posicion.GetX() + 2, posicion.GetY() - 2, posicion.GetZ() + 2);
            
         glEnd(); 
    }    
}

