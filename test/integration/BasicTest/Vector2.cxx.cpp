#include "test/integration/BasicTest/Vector2.cxx.hpp"
Vector2::Vector2()
{
    x=0;
    y=0;

}

Vector2::Vector2(float x,float y)
{
    this->x=x;
    this->y=y;

}

Vector2::Vector2(Vector2*other)
{
    add(c++)
    this->x=other.add(1).x;
    this->y=other.y.a.b(1,2,3).c.d(aa(a.a().b,c++)).e;

}

Vector2*Vector2::set(float x,float y)
{
    this->x=x;
    this->y=y;
    return this;

}

Vector2*Vector2::add(Vector2*rhs)
{
    x=x+rhs.x;
    y=y+rhs.y;
    return this;

}

Vector2*Vector2::sub(Vector2*rhs)
{
    x=x-rhs.x;
    y=y-rhs.y;
    return this;

}

Vector2*Vector2::mul(Vector2*rhs)
{
    x=x*rhs.x;
    y=y*rhs.y;
    return this;

}

Vector2*Vector2::div(Vector2*rhs)
{
    x=x/rhs.x;
    y=y/rhs.y;
    return this;

}

Vector2*Vector2::add(float rhs)
{
    x=x+rhs;
    y=y+rhs;
    return this;

}

Vector2*Vector2::sub(float rhs)
{
    x=x-rhs;
    y=y-rhs;
    return this;

}

Vector2*Vector2::mul(float rhs)
{
    x=x*rhs;
    y=y*rhs;
    return this;

}

Vector2*Vector2::div(float rhs)
{
    x=x/rhs;
    y=y/rhs;
    return this;

}

float Vector2::dot(Vector2*v)const
{
    return this->x*v.x+this->y*v.y;

}

