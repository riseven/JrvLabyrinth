#include <allegro.h>
#include <alleggl.h>
#include "Ascensor.h"

#include "Orientacion.h"
#include "Excepcion.h"
#include "EtapaRender.h"
#include "Nivel.h"

int Ascensor::glTexAscensor;

Ascensor::Ascensor(Nivel *nivel, int cod, Vector centroCelda, Vector radioCelda, Vector posicionReposo, Vector posicionExcitado):
    Item(nivel, cod), posicionReposo(posicionReposo), posicionExcitado(posicionExcitado)
{
    float rad = 0.3 ;
    estado = Reposo ;
    
    posicion = centroCelda ;
    posicion.SetY( posicion.GetY() - 0.5);
    
    Ascensor::posicionReposo.SetY( posicionReposo.GetY() -0.5 );
    Ascensor::posicionExcitado.SetY( posicionExcitado.GetY() -0.5);
    obstaculo = true ;
    
    radio = Vector(radioCelda.GetX(), 0.5, radioCelda.GetZ());
    estado = Reposo ;
    
    
    BITMAP * bitmap = load_bitmap("Texturas\\plataforma.bmp", NULL);
    
    if (!bitmap)
    {
        throw Excepcion("No se puedo cargar la textura plataforma.bmp");
    }
    
    // Creamos la textura
    glTexAscensor = allegro_gl_make_texture_ex(AGL_TEXTURE_MIPMAP | AGL_TEXTURE_MASKED | AGL_TEXTURE_FLIP | AGL_TEXTURE_RESCALE, bitmap, -1);

}

void
Ascensor::Actualizar()
{
    if ( estado == Reposo )
    {
        float dif = posicion.GetY() - posicionReposo.GetY();
        if ( dif < 0.0 ) dif = - dif ;
        if ( dif < 0.03 )
        {
            posicion.SetY( posicionReposo.GetY() );
            nivel->EmpujarItems(posicion, Vector(0, dif+0.02, 0), radio);
        }
        if ( posicion.GetY() > posicionReposo.GetY() )
        {
            posicion.SetY( posicion.GetY() - 0.03 );
        }
        if ( posicion.GetY() < posicionReposo.GetY() )
        {
            nivel->EmpujarItems(posicion, Vector(0, 0.05, 0), radio);            
            posicion.SetY( posicion.GetY() + 0.03 );
        }        
    }
    else if ( estado == Excitado )
    {
        float dif = posicion.GetY() - posicionExcitado.GetY();
        if ( dif < 0.0 ) dif = - dif ;  
        if ( dif < 0.03 )
        {
            nivel->EmpujarItems(posicion, Vector(0, dif+0.02, 0), radio);
            posicion.SetY( posicionExcitado.GetY() );            
        }
        if ( posicion.GetY() < posicionExcitado.GetY() )
        {
            nivel->EmpujarItems(posicion, Vector(0, 0.05, 0), radio);
            posicion.SetY( posicion.GetY() +0.03 );
        }        
        if ( posicion.GetY() > posicionExcitado.GetY() )
        {
            posicion.SetY( posicion.GetY() -0.03 );
        }
    }  
}

void 
Ascensor::Pintar(int etapa)
{
    if ( etapa == EtapaRender::Normal )
    {
        // Pintamos el cubo que representa el bounding box
        Vector b1, b2 ;
        GetBoundingBox(b1, b2);
        b1.SetX( b1.GetX() + 0.01 );
        b1.SetZ( b1.GetZ() + 0.01 );
        b2.SetX( b2.GetX() - 0.01 );
        b2.SetZ( b2.GetZ() - 0.01 );
        b2.SetY( b2.GetY() + 0.01 );
        
        
        glEnable(GL_TEXTURE_2D);
        glColor4f(1,1,1,1);
       // glColor4f(0.25,0.0,1,1);
        glBindTexture(GL_TEXTURE_2D, glTexAscensor);
        glBegin(GL_QUADS);
    
            // -YZ
            glVertex3f(b1.GetX(), b1.GetY(), b1.GetZ());
            glVertex3f(b1.GetX(), b1.GetY(), b2.GetZ());
            glVertex3f(b1.GetX(), b2.GetY(), b2.GetZ());
            glVertex3f(b1.GetX(), b2.GetY(), b1.GetZ());
            
            // +YZ
            glVertex3f(b2.GetX(), b1.GetY(), b1.GetZ());
            glVertex3f(b2.GetX(), b1.GetY(), b2.GetZ());
            glVertex3f(b2.GetX(), b2.GetY(), b2.GetZ());
            glVertex3f(b2.GetX(), b2.GetY(), b1.GetZ());
            
            // X-Z
            glTexCoord2f(0,0);
            glVertex3f(b1.GetX(), b1.GetY(), b1.GetZ());
            glTexCoord2f(0,4);
            glVertex3f(b1.GetX(), b1.GetY(), b2.GetZ());
            glTexCoord2f(4,0);
            glVertex3f(b2.GetX(), b1.GetY(), b2.GetZ());
            glTexCoord2f(4,4);
            glVertex3f(b2.GetX(), b1.GetY(), b1.GetZ());
            
            // X+Z
            glTexCoord2f(0,0);
            glVertex3f(b1.GetX(), b2.GetY(), b1.GetZ());
            glTexCoord2f(0,4);
            glVertex3f(b1.GetX(), b2.GetY(), b2.GetZ());
            glTexCoord2f(4,0);
            glVertex3f(b2.GetX(), b2.GetY(), b2.GetZ());
            glTexCoord2f(4,4);
            glVertex3f(b2.GetX(), b2.GetY(), b1.GetZ());
            
            // XY-
            glVertex3f(b1.GetX(), b1.GetY(), b1.GetZ());
            glVertex3f(b2.GetX(), b1.GetY(), b1.GetZ());
            glVertex3f(b2.GetX(), b2.GetY(), b1.GetZ());
            glVertex3f(b1.GetX(), b2.GetY(), b1.GetZ());
            
            // XY+
            glVertex3f(b1.GetX(), b1.GetY(), b2.GetZ());
            glVertex3f(b2.GetX(), b1.GetY(), b2.GetZ());
            glVertex3f(b2.GetX(), b2.GetY(), b2.GetZ());
            glVertex3f(b1.GetX(), b2.GetY(), b2.GetZ());
            
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }        
}

void
Ascensor::Excitar()
{
    estado = Excitado ;
}

void
Ascensor::Desexcitar()
{
    estado = Reposo ;
}

void
Ascensor::Intercambiar()
{
    if ( estado == Excitado )
    {
        estado = Reposo ;
        return ;
    }
    estado = Excitado ;
}



