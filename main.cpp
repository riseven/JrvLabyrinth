#include <allegro.h>
#include <alleggl.h>

#include "Juego.h"
#include "Excepcion.h"

int main(int argc, char *argv[])
{
	allegro_init();
	install_allegro_gl();
	install_keyboard();
	install_mouse();
    
    try
    {
        Juego::Inicializar() ;
        
        Juego *juego = new Juego();
        juego->BuclePrincipal();
        delete juego ;    
            
        Juego::Cerrar();        
    }
    catch ( Excepcion &ex )
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("%s\n", ex.GetMensajeDeError().c_str());
    }
    catch ( exception &ex )
    {
        set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
        allegro_message("%s\n", ex.what() );
    }    
}
END_OF_MAIN();

