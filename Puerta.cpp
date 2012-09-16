#include <allegro.h>
#include <alleggl.h>
#include "Puerta.h"

#include "Orientacion.h"
#include "Excepcion.h"
#include "EtapaRender.h"
#include "Nivel.h"

int Puerta::glTexPuerta;

Puerta::Puerta(Nivel *nivel, int cod, Vector centroCelda, Vector radioCelda, int orien):
    Item(nivel, cod)
{
    float rad = 0.3 ;
    estado = Cerrada ;
    
    posicion = centroCelda ;
    switch (orien)
    {
        case Orientacion::Norte:
            posicion.SetZ( posicion.GetZ() - radioCelda.GetZ() + rad ) ;
            posicion.SetY( posicion.GetY() + 0.8*radioCelda.GetY() );
            radio = Vector( radioCelda.GetX(), 0.8*radioCelda.GetY(), rad);
            break ;
        case Orientacion::Este:
            posicion.SetX( posicion.GetX() + radioCelda.GetX() - rad ) ;
            posicion.SetY( posicion.GetY() + 0.8*radioCelda.GetY() );
            radio = Vector( rad, 0.8*radioCelda.GetY(), radioCelda.GetZ());
            break ;
        case Orientacion::Sur:
            posicion.SetZ( posicion.GetZ() + radioCelda.GetZ() - rad ) ;
            posicion.SetY( posicion.GetY() + 0.8*radioCelda.GetY() );
            radio = Vector( radioCelda.GetX(), 0.8*radioCelda.GetY(), rad);
            break ;
        case Orientacion::Oeste:
            posicion.SetX( posicion.GetX() - radioCelda.GetX() + rad ) ;
            posicion.SetY( posicion.GetY() + 0.8*radioCelda.GetY() );
            radio = Vector( rad, 0.8*radioCelda.GetY(), radioCelda.GetZ());
            break ;
            
    }
    posicionCerrada = posicion ;
    posicionAbierta = posicionCerrada - Vector(0, 1.6*radioCelda.GetY(), 0) ;
    obstaculo = true ;
    orientacion = orien;
    
    
    BITMAP * bitmap = load_bitmap("Texturas\\puerta.bmp", NULL);
    
    if (!bitmap)
    {
        throw Excepcion("No se puedo cargar la textura puerta.bmp");
    }
    
    // Creamos la textura
    glTexPuerta = allegro_gl_make_texture_ex(AGL_TEXTURE_MIPMAP | AGL_TEXTURE_MASKED | AGL_TEXTURE_FLIP | AGL_TEXTURE_RESCALE, bitmap, -1);

}

void
Puerta::Actualizar()
{
    if ( estado == Abierta )
    {
        if ( posicion.GetY() > posicionAbierta.GetY() )
        {
            posicion.SetY( posicion.GetY() -0.03 );
        }
        if ( posicion.GetY() < posicionAbierta.GetY() )
        {
            posicion.SetY( posicionAbierta.GetY() );
        }
    }
    else if ( estado == Cerrada )
    {
        if ( posicion.GetY() < posicionCerrada.GetY() )
        {
            nivel->EmpujarItems(posicion, Vector(0, 0.03, 0), radio);
            posicion.SetY( posicion.GetY() +0.03 );
        }
        if ( posicion.GetY() > posicionCerrada.GetY() )
        {
            posicion.SetY( posicionCerrada.GetY() );
        }
    }  
}

void 
Puerta::Pintar(int etapa)
{
    if ( etapa == EtapaRender::Normal )
    {
        // Pintamos el cubo que representa el bounding box
        Vector b1, b2 ;
        GetBoundingBox(b1, b2);
        b1.SetX(b1.GetX()+0.01);
        b1.SetZ(b1.GetZ()+0.01);
        b2.SetX(b2.GetX()-0.01);
        b2.SetZ(b2.GetZ()-0.01);
        b2.SetY(b2.GetY()+0.01);
        
        glEnable(GL_TEXTURE_2D);
        glColor4f(1,1,1,1);
        glBindTexture(GL_TEXTURE_2D, glTexPuerta);
        glBegin(GL_QUADS);
    
            // -YZ
            glTexCoord2d(0,0);
            glVertex3f(b1.GetX(), b1.GetY(), b1.GetZ());
            glTexCoord2d(1,0);
            glVertex3f(b1.GetX(), b1.GetY(), b2.GetZ());
            glTexCoord2d(1,1);
            glVertex3f(b1.GetX(), b2.GetY(), b2.GetZ());
            glTexCoord2d(0,1);
            glVertex3f(b1.GetX(), b2.GetY(), b1.GetZ());
            
            // +YZ
            glTexCoord2d(0,0);
            glVertex3f(b2.GetX(), b1.GetY(), b1.GetZ());
            glTexCoord2d(1,0);
            glVertex3f(b2.GetX(), b1.GetY(), b2.GetZ());
            glTexCoord2d(1,1);
            glVertex3f(b2.GetX(), b2.GetY(), b2.GetZ());
            glTexCoord2d(0,1);
            glVertex3f(b2.GetX(), b2.GetY(), b1.GetZ());
            
            // X-Z
            glTexCoord2d(0,0);
            glVertex3f(b1.GetX(), b1.GetY(), b1.GetZ());
            glTexCoord2d(1,0);
            glVertex3f(b1.GetX(), b1.GetY(), b2.GetZ());
            glTexCoord2d(1,1);
            glVertex3f(b2.GetX(), b1.GetY(), b2.GetZ());
            glTexCoord2d(0,1);
            glVertex3f(b2.GetX(), b1.GetY(), b1.GetZ());
            
            // X+Z
            glTexCoord2d(0,0);
            glVertex3f(b1.GetX(), b2.GetY(), b1.GetZ());
            glTexCoord2d(1,0);
            glVertex3f(b1.GetX(), b2.GetY(), b2.GetZ());
            glTexCoord2d(1,1);
            glVertex3f(b2.GetX(), b2.GetY(), b2.GetZ());
            glTexCoord2d(0,1);
            glVertex3f(b2.GetX(), b2.GetY(), b1.GetZ());
            
            // XY-
            glTexCoord2d(0,0);            
            glVertex3f(b1.GetX(), b1.GetY(), b1.GetZ());
            glTexCoord2d(1,0);
            glVertex3f(b2.GetX(), b1.GetY(), b1.GetZ());
            glTexCoord2d(1,1);
            glVertex3f(b2.GetX(), b2.GetY(), b1.GetZ());
            glTexCoord2d(0,1);
            glVertex3f(b1.GetX(), b2.GetY(), b1.GetZ());
            
            // XY+
            glTexCoord2d(0,0);
            glVertex3f(b1.GetX(), b1.GetY(), b2.GetZ());
            glTexCoord2d(1,0);
            glVertex3f(b2.GetX(), b1.GetY(), b2.GetZ());
            glTexCoord2d(1,1);
            glVertex3f(b2.GetX(), b2.GetY(), b2.GetZ());
            glTexCoord2d(0,1);
            glVertex3f(b1.GetX(), b2.GetY(), b2.GetZ());
            
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }        
}

void
Puerta::Excitar()
{
    estado = Abierta ;
}

void
Puerta::Desexcitar()
{
    estado = Cerrada ;
}

void
Puerta::Intercambiar()
{
    if ( estado == Abierta )
    {
        estado = Cerrada ;
        return ;
    }
    estado = Abierta ;
}


