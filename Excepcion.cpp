#include <allegro.h>

#include "Excepcion.h"

Excepcion::Excepcion():
    mensajeDeError("Error no especificado")
{
}

Excepcion::Excepcion(string mensajeDeError):
    mensajeDeError(mensajeDeError)
{
}

Excepcion::Excepcion(const Excepcion &ex)
{
    *this = ex ;
}

Excepcion::Excepcion(const exception &ex)
{
    mensajeDeError = ex.what() ;
}


Excepcion::Excepcion(Excepcion &ex, string mensajeDeTraza)
{
    *this = ex ;
    mensajeDeError += "\n" ;
    mensajeDeError += mensajeDeTraza ;
}

Excepcion::Excepcion(exception &ex, string mensajeDeTraza)
{
    mensajeDeError = ex.what() ;
    mensajeDeError += "\n" ;
    mensajeDeError += mensajeDeTraza ;
}

Excepcion &
Excepcion::operator =(const Excepcion &ex)
{
    if ( this == &ex )
    {
        return *this ;
    }
    
    mensajeDeError = ex.mensajeDeError ;
    return *this ;
}

Excepcion::~Excepcion()
{
}

string
Excepcion::GetMensajeDeError()
{
    return mensajeDeError ;
}
