#ifndef __NIVEL_H__
#define __NIVEL_H__

#include <allegro.h>
#include <alleggl.h>

#include <string>
#include <fstream>
#include <list>

#include "Vector.h"
#include "Frecuencia.h"

using namespace std ;

class Camara ;
class Jugador ;
class Item ;
class Luz ;
class Ascensor ;

class Nivel
{
public:
    enum EstadoNivel
    {
        EN_Jugando,
        EN_Muerto,
        EN_Superado
    };    
    
private:
    static int glTexGui ;
    static int glTexNivel ;
    static int glTexNum[10] ;
    static int glTexTiempo ;
    static int glTexSuelo;
    static int glTexPared;
    
    static float guiRojo ;
    static float guiVerde ;
    static float guiAzul ;
    
    int numNivel ;
    int **altura ;
    int w, h ;  
    int tiempo ;
    bool visionEstereo ;
    float zoom ;
    float disVista ;
    Frecuencia frecTiempo;
    
    // OpenGL CallList para pintar el escenario
    unsigned callList ; 
    
    // Camara con la que se visualiza el escenario
    Camara *camara ;
    
    // Jugador
    Jugador *jugador ;
    
    // Items
    list <Item *> item ;
    
    // Luces
    list <Luz *> luz ;
    
    void GenerarMapa();
    EstadoNivel estado ;
    
public: 

    static void Inicializar();
    
    static const float escala = 5.0f ;
       
    Nivel(int numNivel);
    ~Nivel();
    void DesplazarItem(Vector pos, Vector &mov, Vector rad);
    void EmpujarItems(Vector pos, Vector mov, Vector rad);
    void Actualizar();
    void Pintar();
    void PintarGui();
    void GetPosicionJugador(int &x, int &y);
    float GetAlturaJugador();
    Ascensor * GetAscensorEn(float px, float py);
    EstadoNivel GetEstado(){return estado;}
    void MuereJugador(){estado = EN_Muerto;}
    
    
    void ExcitarCodigo(int codigo);
    void DesexcitarCodigo(int codigo);
    void IntercambiarCodigo(int codigo);
};

#endif
