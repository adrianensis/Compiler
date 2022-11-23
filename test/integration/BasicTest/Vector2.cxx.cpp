#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/BasicTypes.cxx.hpp"
#include "test/integration/BasicTest/MathUtils.cxx.hpp"
Vector2::Vector2()
{
    x=0;;
    y=0;;

}

Vector2::Vector2(f32 x,f32 y)
{
    this->x=x;;
    this->y=y;;

}

Vector2::Vector2(const Vector2&other)
{
    this->x=other.x;;
    this->y=other.y;;

}

Vector2::Vector2(const Vector3&other)
{
    this->x=other.x;;
    this->y=other.y;;

}

Vector2&Vector2::set(f32 x,f32 y)
{
    this->x=x;;
    this->y=y;;
    return*this;;

}

Vector2&Vector2::set(const Vector2&rhs)
{
    this->set(rhs.x,rhs.y);
    return*this;;

}

Vector2&Vector2::set(const Vector3&rhs)
{
    this->set(rhs.x,rhs.y);
    return*this;;

}

Vector2&Vector2::add(const Vector2&rhs)
{
    x=x+rhs.x;;
    y=y+rhs.y;;
    return*this;;

}

Vector2&Vector2::sub(const Vector2&rhs)
{
    x=x-rhs.x;;
    y=y-rhs.y;;
    return*this;;

}

Vector2&Vector2::mul(const Vector2&rhs)
{
    x=x*rhs.x;;
    y=y*rhs.y;;
    return*this;;

}

Vector2&Vector2::div(const Vector2&rhs)
{
    x=x/rhs.x;;
    y=y/rhs.y;;
    return*this;;

}

Vector2&Vector2::add(f32 rhs)
{
    x=x+rhs;;
    y=y+rhs;;
    return*this;;

}

Vector2&Vector2::sub(f32 rhs)
{
    x=x-rhs;;
    y=y-rhs;;
    return*this;;

}

Vector2&Vector2::mul(f32 rhs)
{
    x=x*rhs;;
    y=y*rhs;;
    return*this;;

}

Vector2&Vector2::div(f32 rhs)
{
    x=x/rhs;;
    y=y/rhs;;
    return*this;;

}

f32 Vector2::dot(const Vector2&v)const
{
    return this->x*v.x+(this->y*v.y);;

}

f32 Vector2::sqrlen()const
{
    return this->dot(*this);;

}

f32 Vector2::sqrdst(Vector2&v)const
{
    Vector2&sub=Vector2(v).sub(*this);
    ;
    return sub.dot(sub);;

}

f32 Vector2::len()const
{
    return std::sqrt(this->sqrlen());;

}

f32 Vector2::max()const
{
    return std::max(x,y);;

}

f32 Vector2::min()const
{
    return std::min(x,y);;

}

Vector2&Vector2::nor()
{
    f32 len=this->len();
    ;
    this->div(len);
    return*this;;

}

f32 Vector2::angle(const Vector2&v)const
{
    f32 angle=std::atan2(v.y,v.x)-std::atan2(this->y,this->x);
    ;
    if(angle<0){
        angle+=2*MathUtils::PI;

    }
    ;
    return angle;;

}

Vector2&Vector2::clamp(f32 maxLength)
{
    if(this->sqrlen()>maxLength*maxLength){
        this->nor();
        this->mul(maxLength);

    }
    ;
    return*this;;

}

