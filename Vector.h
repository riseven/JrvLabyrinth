#ifndef __VECTOR_H__
#define __VECTOR_H__

class Vector
{
private:
    float x, y, z ;
    
public:
    Vector();
    Vector(float x, float y, float z);
    Vector(const Vector &v);
    Vector & operator = (const Vector &v);
    ~Vector();
    float GetX();
    float GetY();
    float GetZ();
    void SetX(float x);
    void SetY(float y);
    void SetZ(float z);
    Vector operator + (const Vector &v);
    Vector operator - (const Vector &v);
};

#endif 
    
