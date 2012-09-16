#include <allegro.h>
#include <alleggl.h>
#include "Juego.h"

#include <string>
#include "Excepcion.h"
#include "Losa.h"
#include "Frecuencia.h"

using namespace std;

void
Juego::Inicializar()
{
    try
    {
        // Opciones para la ventana de OpenGL
    	allegro_gl_clear_settings();
    	allegro_gl_set (AGL_COLOR_DEPTH, 32);
    	allegro_gl_set (AGL_Z_DEPTH, 8);
    	allegro_gl_set (AGL_FULLSCREEN, TRUE);
    	allegro_gl_set (AGL_DOUBLEBUFFER, 1);
    	allegro_gl_set (AGL_SUGGEST, AGL_COLOR_DEPTH | AGL_Z_DEPTH | AGL_DOUBLEBUFFER | AGL_FULLSCREEN);
    
        // Creamos la ventana a mostrar
        if ( set_gfx_mode(GFX_OPENGL_FULLSCREEN, 640, 480, 0, 0) )
        {
            throw Excepcion("Juego::Inicializar(): No se pudo establecer el modo grafico");
        }
    
    	// Color de fondo de la ventana
    	glClearColor(0.0, 0.0, 0.0, 0.0);
    
        // Para trabajar con texturas
        //glEnable(GL_TEXTURE_2D);
    	glEnable(GL_BLEND);
    	
    	glEnable(GL_LIGHTING);
    	glEnable(GL_COLOR_MATERIAL);
    	
    	glHint(GL_FOG_HINT, GL_DONT_CARE);
    	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    
    	glShadeModel(GL_SMOOTH);
    	glPolygonMode(GL_FRONT, GL_FILL);
    	//glEnable(GL_CULL_FACE);
    	
        // Se especifica que las matrices y transformaciones siguientes trabajaran
        // sobre la matriz de proyeccion
        glMatrixMode (GL_PROJECTION);
    
        // Cargamos la matriz identidad
        glLoadIdentity ();
    
        // Se multiplica la matriz identidad por la matriz de perspectiva a utilizar
        // Los primeros cuatro parametros son los limites de la pantalla (L,R,B,U)
        // Los dos ultimos definen la distancia cercana y la lejana respectivamente
        glFrustum (-1.0, 1.0, -1.0, 1.0, 1.0, 40.0);
    
        // Se especifica que las transformaciones siguientes se aplicaran al modelo
        // (tb se podria trabajar con las texturas, por ejemplo)
        glMatrixMode (GL_MODELVIEW);
        
        Nivel::Inicializar();
        Losa::Inicializar();
    }
    catch ( exception &ex )
    {
        throw Excepcion(ex, "Juego::Inicializar()");
    }
}

void
Juego::Cerrar()
{
    
}

Juego::Juego()
{
    try
    {
        numNivel = 1 ;
        nivelActual = new Nivel(numNivel);
    }
    catch ( Excepcion &ex )
    {
        throw Excepcion(ex, "Juego::Juego()");
    }
    catch ( exception &ex )
    {
        throw Excepcion(ex, "Juego::Juego()");
    }
}

Juego::~Juego()
{
    try
    {
        if ( !nivelActual )
        {
            throw Excepcion("nivelActual vale null");
        }
        delete nivelActual ;
    }
    catch ( Excepcion &ex )
    {
        throw Excepcion(ex, "Juego::~Juego()");
    }    
    catch ( exception &ex )
    {
        throw Excepcion(ex, "Juego::~Juego()");
    }    
}

void
Juego::BuclePrincipal()
{
    try 
    {
        Frecuencia frecActualizar(50);
        bool salir = false ;
        while (!salir)
        {
            allegro_gl_set_allegro_mode();
            allegro_gl_unset_allegro_mode();
            
            //Limpiamos la ventana con el color de fondo
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            while (frecActualizar.comprobar())
            {
                nivelActual->Actualizar();
            }
            nivelActual->Pintar();
         
            //Compila los comandos
            glFlush();
        
            //Vuelca el buffer a la pantalla
            allegro_gl_flip();
            
            // Comprobamos si se ha pulsado Escape, en cuyo caso salimos del bucle 
            if ( key[KEY_ESC] )
            {
                salir = true ;
            }
            else if ( key[KEY_R] || nivelActual->GetEstado()==Nivel::EN_Muerto )
            {
                delete nivelActual ;
                nivelActual = new Nivel(numNivel);
            }
            else if ( nivelActual->GetEstado()==Nivel::EN_Superado )
            {
                delete nivelActual ;
                nivelActual = new Nivel(++numNivel);
            }
        }
    }
    catch ( Excepcion &ex )
    {
        throw Excepcion(ex, "Juego::BuclePrincipal()") ;
    }
    catch ( exception &ex )
    {
        throw Excepcion(ex, "Juego::BuclePrincipal()") ;
    }
}
