#include <allegro.h>
#include <alleggl.h>
#include "Camara.h"

#include "Excepcion.h"
#include "Item.h"

Camara::Camara()
{
    try
    {
        objetivo = NULL ;    
    }
    catch ( Excepcion &ex )
    {
        throw Excepcion(ex, "Camara::Camara()");
    }
    catch ( exception &ex )
    {
        throw Excepcion(ex, "Camara::Camara()");
    }    
}

Camara::~Camara()
{
    try
    {
        objetivo = NULL ;    
    }
    catch ( Excepcion &ex )
    {
        throw Excepcion(ex, "Camara::~Camara()");
    }
    catch ( exception &ex )
    {
        throw Excepcion(ex, "Camara::~Camara()");
    }        
}

void
Camara::Linkar(Item *item)
{
    try
    {
        objetivo = item ;
    }
    catch ( Excepcion &ex )
    {
        throw Excepcion(ex, "Camara::Linkar(Item *)");
    }
    catch ( exception &ex )
    {
        throw Excepcion(ex, "Camara::Linkar(Item *)");
    }    
}

void
Camara::Pintar()
{
    try
    {
        glLoadIdentity();
        
        if ( objetivo )
        {
            Vector orientacion = objetivo->GetOrientacion();
            
            glRotatef( orientacion.GetX(), 1, 0, 0);
            glRotatef( orientacion.GetY(), 0, 1, 0);
            
            Vector posicion = objetivo->GetPosicion();
            glTranslatef( -posicion.GetX(), -posicion.GetY()-1, -posicion.GetZ() );
        }
        

    } 
    catch ( Excepcion &ex )
    {
        throw Excepcion(ex, "Camara::Pintar()");
    }
    catch ( exception &ex )
    {
        throw Excepcion(ex, "Camara::Pintar()");
    }
}

void
Camara::Pintar(int ojo)
{
    try
    {
        glLoadIdentity();
        
        if ( objetivo )
        {
            Vector orientacion = objetivo->GetOrientacion();
            
            glRotatef( orientacion.GetX(), 1, 0, 0);
            glRotatef( orientacion.GetY()+(ojo?5:-5), 0, 1, 0);
            
            Vector posicion = objetivo->GetPosicion();

            glTranslatef( -posicion.GetX(), -posicion.GetY()-1, -posicion.GetZ() );
        }
        

    } 
    catch ( Excepcion &ex )
    {
        throw Excepcion(ex, "Camara::Pintar()");
    }
    catch ( exception &ex )
    {
        throw Excepcion(ex, "Camara::Pintar()");
    }
}
