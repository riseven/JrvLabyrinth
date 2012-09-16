#include <allegro.h>
#include <alleggl.h>
#include "Bloque.h"

Bloque::Bloque(Nivel *nivel, Vector posicion):
    Item(nivel, -1, posicion)
{
}
