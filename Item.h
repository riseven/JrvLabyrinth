#ifndef __ITEM_H__
#define __ITEM_H__

#include <allegro.h>
#include <alleggl.h>

#include "Vector.h"

class Nivel ;

class Item
{
protected:
    Nivel *nivel ;
    int codigo ;
    Vector posicion ;
    Vector orientacion ;
    Vector radio ;
    bool obstaculo;
    bool empujable;
public:
    Item(Nivel *nivel, int codigo);
    Item(Nivel *nivel, int codigo, Vector v);
    ~Item();
    void SetCodigo(int codigo) { Item::codigo = codigo ; } 
    int GetCodigo() { return codigo ; }
    void SetPosicion(Vector posicion);
    Vector GetPosicion();
    void SetOrientacion(Vector orientacion);
    Vector GetOrientacion();
    void SetRadio(Vector radio);
    Vector GetRadio();
    void SetEsObstaculo(bool obstaculo);
    bool GetEsObstaculo();
    void GetBoundingBox(Vector &v1, Vector &v2);
    bool Colision(Vector pos, Vector &mov, Vector rad);
    bool Empujar(Vector pos, Vector mov, Vector rad);
    virtual bool EsAscensor(){return false;}
    virtual void Actualizar();
    virtual void Pintar(int etapa);
    
    virtual void Excitar(){}
    virtual void Desexcitar(){}
    virtual void Intercambiar(){}
};

#endif
