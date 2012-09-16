#include <allegro.h>
#include <alleggl.h>
#include "Losa.h"

#include "Nivel.h"
#include "Excepcion.h"
#include "EtapaRender.h"

int Losa::glTextura ;

void
Losa::Inicializar()
{
    BITMAP *bitmap = NULL;
    bitmap = load_bitmap("Texturas\\anillo2.bmp", NULL);
    
    if (!bitmap)
    {
        throw Excepcion("No se puedo cargar la textura para la losa");
    }
    
    // Creamos la textura
    glTextura = allegro_gl_make_texture_ex(AGL_TEXTURE_MIPMAP | AGL_TEXTURE_MASKED | AGL_TEXTURE_FLIP | AGL_TEXTURE_RESCALE, bitmap, -1);
}

void
Losa::Cerrar()
{
}


Losa::Losa(Nivel *nivel, int cod, Vector pos, Vector rad, int objetivo, int tipoActuador):
    Item(nivel, cod), objetivo(objetivo), tipoActuador(tipoActuador)
{
    posicion = pos ;
    posicion.SetY( posicion.GetY() + 0.001 );
    radio = rad ;
    angulo = 0.0 ;
    velAng = 0.2 ;
    ascensorBuscado = false ;
    estado = Activada ;
    
}

void 
Losa::Actualizar()
{
    if ( estado == Activada )
    {
        int jx, jz ;
        nivel->GetPosicionJugador(jx, jz);
        
        float dif = nivel->GetAlturaJugador() - posicion.GetY() ;
        if ( dif < 0.0 ) dif = - dif ;    
        if ( jx == (int)( posicion.GetX() / Nivel::escala ) &&
             jz == (int)( posicion.GetZ() / Nivel::escala ) &&
             dif <= 0.4 )
        {
            if ( !estabaDentro )
            {
                switch ( tipoActuador )
                {
                case Excitador:
                    nivel->ExcitarCodigo(objetivo);
                    break ;
                case Desexcitador:
                    nivel->DesexcitarCodigo(objetivo);
                    break ;
                case Intercambiador:
                    nivel->IntercambiarCodigo(objetivo);
                    break ;
                }
                
                estabaDentro = true ;
            }        
        }
        else
        {
            estabaDentro = false ;
        }  
        
        // Giro del anillo
        if ( estabaDentro )
        {
            if ( velAng < 2.0 )
            {
                velAng += 0.1 ;
            }
        }
        else
        {
            if ( velAng > 0.1 )
            {
                velAng -= 0.01 ;
            }
        }
        angulo += velAng ;       
        
        // Situacion vertical
        if ( !ascensorBuscado )
        {
            ascensorBuscado = true ;
            ascensorLinkado = nivel->GetAscensorEn(posicion.GetX(), posicion.GetZ());
        }
        if ( ascensorLinkado )
        {
            posicion.SetY( ascensorLinkado->GetPosicion().GetY() + ascensorLinkado->GetRadio().GetY() +0.1 );
        }
    }    
}

void
Losa::Pintar(int etapaRender)
{
    if ( estado==Activada )
    {
       
        float des = angulo ;
        for ( int i = 0 ; (i < 1 && velAng > 0.5 ) || !i ; i++ )
        {
            des -= 1.0 ;
            if ( etapaRender == EtapaRender::Transparente )
            {
                glPushMatrix();
                
                glTranslatef(posicion.GetX(), posicion.GetY()+0.01, posicion.GetZ());
                
                glRotatef(des,0,1,0);
        
                
                glScalef(0.7,0.7,0.7);
                
                Vector b1, b2 ;
                GetBoundingBox(b1, b2);
                b1 = Vector(-radio.GetX(),-radio.GetY(),-radio.GetZ());
                b2 = Vector(radio.GetX(),radio.GetY(),radio.GetZ());
                
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
                
                glEnable(GL_TEXTURE_2D);
                
                float emision[4]={0,1,0,1} ;
                float emisionNormal[4]={0,0,0,1};
                
                emision[1] = velAng/3.0 + 0.2 ;
                emision[2] = 0.2 ;
                
                glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emision);
                
                
                
                glBindTexture(GL_TEXTURE_2D, glTextura);
                glColor4f(0,0,1,1);
                glBegin(GL_QUADS);
                    glTexCoord2f(0, 1);
                    glVertex3f(b1.GetX(), 0, b1.GetZ());
                    glTexCoord2f(1, 1);
                    glVertex3f(b2.GetX(), 0, b1.GetZ());
                    glTexCoord2f(1, 0);
                    glVertex3f(b2.GetX(), 0, b2.GetZ());
                    glTexCoord2f(0, 0);
                    glVertex3f(b1.GetX(), 0, b2.GetZ());
                glEnd();
                
                glDisable(GL_BLEND);
                
                glDisable(GL_TEXTURE_2D);    
                glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emisionNormal);  
                
                glPopMatrix();
            }
        }
    }
}
