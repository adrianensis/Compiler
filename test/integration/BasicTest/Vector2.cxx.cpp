#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/BasicTypes.cxx.hpp"
#include "test/integration/BasicTest/Vector3.cxx.hpp"
#include "test/integration/BasicTest/Vector4.cxx.hpp"
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

Vector2::Vector2(const Vector4&other)
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

Vector2&Vector2::set(const Vector4&rhs)
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

f32 Vector2::sqrdst(const Vector2&v)const
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

f32 Vector2::dst(const Vector2&v)const
{
    return sqrtf(this->sqrdst(v));;

}

bool Vector2::eq(const Vector2&v)const
{
    return MathUtils::eqf(this->x,v.x)&&MathUtils::eqf(this->y,v.y);;

}

bool Vector2::eq(const Vector2&v,f32 e)const
{
    return MathUtils::eqf(this->x,v.x,e)&&MathUtils::eqf(this->y,v.y,e);;

}

Vector2&Vector2::lerp(const Vector2&target,f32 t)
{
    (*this)+=Vector2(target).sub(*this).mul(t);
    return*this;;

}

f32 Vector2::angle(const Vector2&v)const
{
    f32 angle=std::atan2(v.y,v.x)-std::atan2(this->y,this->x);
    ;
    if(angle<0)
    {
        angle+=2*MathUtils::PI;

    }
    ;
    return angle;;

}

Vector2&Vector2::clamp(f32 maxLength)
{
    if(this->sqrlen()>maxLength*maxLength)
    {
        this->nor();
        this->mul(maxLength);

    }
    ;
    return*this;;

}

Vector2&Vector2::operator=(const Vector2&other)
{
    return set(other);;

}

Vector2&Vector2::operator+=(const Vector2&rhs)
{
    return this->add(rhs);;

}

Vector2&Vector2::operator-=(const Vector2&rhs)
{
    return this->sub(rhs);;

}

Vector2&Vector2::operator*=(const Vector2&rhs)
{
    return this->mul(rhs);;

}

Vector2&Vector2::operator/=(const Vector2&rhs)
{
    return this->div(rhs);;

}

Vector2&Vector2::operator+=(f32 rhs)
{
    return this->add(rhs);;

}

Vector2&Vector2::operator-=(f32 rhs)
{
    return this->sub(rhs);;

}

Vector2&Vector2::operator*=(f32 rhs)
{
    return this->mul(rhs);;

}

Vector2&Vector2::operator/=(f32 rhs)
{
    return this->div(rhs);;

}

bool Vector2::operator==(const Vector2&rhs)const
{
    return this->eq(rhs);;

}

bool Vector2::operator!=(const Vector2&rhs)const
{
    return!((*this)==rhs);;

}

Vector2&Vector2::operator+(const Vector2&rhs)const
{
    return Vector2(*this)+=rhs;;

}

Vector2&Vector2::operator-(const Vector2&rhs)const
{
    return Vector2(*this)-=rhs;;

}

Vector2&Vector2::operator*(const Vector2&rhs)const
{
    return Vector2(*this)*=rhs;;

}

Vector2&Vector2::operator/(const Vector2&rhs)const
{
    return Vector2(*this)/=rhs;;

}

Vector2&Vector2::operator+(f32 rhs)const
{
    return Vector2(*this)+=rhs;;

}

Vector2&Vector2::operator-(f32 rhs)const
{
    return Vector2(*this)-=rhs;;

}

Vector2&Vector2::operator-()const
{
    return Vector2(*this)*=-1;;

}

Vector2&Vector2::operator*(f32 rhs)const
{
    return Vector2(*this)*=rhs;;

}

Vector2&Vector2::operator/(f32 rhs)const
{
    return Vector2(*this)/=rhs;;

}

