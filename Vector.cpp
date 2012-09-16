#include <allegro.h>
#include <alleggl.h>
#include "Vector.h"

Vector::Vector():
    x(0.0f), y(0.0f), z(0.0f)
{
}

Vector::Vector(float x, float y, float z):
    x(x), y(y), z(z)
{
}

Vector::Vector(const Vector &v)
{
    *this = v ;
}

Vector &
Vector::operator =(const Vector &v)
{
    if ( this == &v )
    {
        return *this ;
    }
    
    x = v.x ;
    y = v.y ;
    z = v.z ;
    return *this ;
}

Vector::~Vector()
{
    x = 0.0f ;
    y = 0.0f ;
    z = 0.0f ;
}

float
Vector::GetX()
{
    return x ;
}

float
Vector::GetY()
{
    return y ;
}

float
Vector::GetZ()
{
    return z ;
}

void
Vector::SetX(float x)
{
    Vector::x = x ;
}

void
Vector::SetY(float y)
{
    Vector::y = y ;
}

void
Vector::SetZ(float z)
{
    Vector::z = z ;
}

Vector
Vector::operator +(const Vector &v)
{
    return Vector(x+v.x, y+v.y, z+v.z);
}

Vector
Vector::operator -(const Vector &v)
{
    return Vector(x-v.x, y-v.y, z-v.z);
}


