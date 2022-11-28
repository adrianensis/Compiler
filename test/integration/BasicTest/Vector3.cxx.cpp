#include "test/integration/BasicTest/Vector3.cxx.hpp"
#include "test/integration/BasicTest/BasicTypes.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector4.cxx.hpp"
#include "test/integration/BasicTest/MathUtils.cxx.hpp"
Vector3::Vector3()
{
    x=0;;
    y=0;;
    z=0;;

}

Vector3::Vector3(f32 x,f32 y,f32 z)
{
    x=x;;
    y=y;;
    z=z;;

}

Vector3::Vector3(const Vector3&other)
{
    x=other.x;;
    y=other.y;;
    z=other.z;;

}

Vector3::Vector3(const Vector2&other)
{
    x=other.x;;
    y=other.y;;
    z=0.0;;

}

Vector3::Vector3(const Vector2&other,f32 z)
{
    x=other.x;;
    y=other.y;;
    z=z;;

}

Vector3::Vector3(const Vector4&other)
{
    x=other.x;;
    y=other.y;;
    z=other.z;;

}

Vector3&Vector3::set(f32 x,f32 y,f32 z)
{
    this->x=x;;
    this->y=y;;
    this->z=z;;
    return*this;;

}

Vector3&Vector3::set(const Vector2&rhs)
{
    this->set(rhs.x,rhs.y,0);
    return*this;;

}

Vector3&Vector3::set(const Vector3&rhs)
{
    this->set(rhs.x,rhs.y,rhs.z);
    return*this;;

}

Vector3&Vector3::set(const Vector4&rhs)
{
    this->set(rhs.x,rhs.y,rhs.z);
    return*this;;

}

Vector3&Vector3::add(const Vector3&rhs)
{
    x=x+rhs.x;;
    y=y+rhs.y;;
    z=z+rhs.z;;
    return*this;;

}

Vector3&Vector3::sub(const Vector3&rhs)
{
    x=x-rhs.x;;
    y=y-rhs.y;;
    z=z-rhs.z;;
    return*this;;

}

Vector3&Vector3::mul(const Vector3&rhs)
{
    x=x*rhs.x;;
    y=y*rhs.y;;
    z=z*rhs.z;;
    return*this;;

}

Vector3&Vector3::div(const Vector3&rhs)
{
    x=x/rhs.x;;
    y=y/rhs.y;;
    z=z/rhs.z;;
    return*this;;

}

Vector3&Vector3::add(f32 rhs)
{
    x=x+rhs;;
    y=y+rhs;;
    z=z+rhs;;
    return*this;;

}

Vector3&Vector3::sub(f32 rhs)
{
    x=x-rhs;;
    y=y-rhs;;
    z=z-rhs;;
    return*this;;

}

Vector3&Vector3::mul(f32 rhs)
{
    x=x*rhs;;
    y=y*rhs;;
    z=z*rhs;;
    return*this;;

}

Vector3&Vector3::div(f32 rhs)
{
    x=x/rhs;;
    y=y/rhs;;
    z=z/rhs;;
    return*this;;

}

f32 Vector3::dot(const Vector3&v)const
{
    return this->x*v.x+this->y*v.y+this->z*v.z;;

}

f32 Vector3::sqrlen()const
{
    return this->dot(*this);;

}

f32 Vector3::sqrdst(const Vector3&v)const
{
    Vector3 sub=Vector3(v)-(*this);
    ;
    return sub.dot(sub);;

}

f32 Vector3::len()const
{
    return sqrtf(this->sqrlen());;

}

f32 Vector3::max()const
{
    return std::max(x,std::max(y,z));;

}

f32 Vector3::min()const
{
    return std::min(x,std::min(y,z));;

}

Vector3&Vector3::nor()
{
    f32 len=this->len();
    ;
    if(len>0)
    {
        this->div(len);

    }
    ;
    return*this;;

}

f32 Vector3::dst(const Vector3&v)const
{
    return sqrtf(this->sqrdst(v));;

}

bool Vector3::eq(const Vector3&v)const
{
    return MathUtils::eqf(this->x,v.x)&&MathUtils::eqf(this->y,v.y)&&MathUtils::eqf(this->z,v.z);;

}

bool Vector3::eq(const Vector3&v,f32 e)const
{
    return MathUtils::eqf(this->x,v.x,e)&&MathUtils::eqf(this->y,v.y,e)&&MathUtils::eqf(this->z,v.z,e);;

}

Vector3&Vector3::cross(const Vector3&v)
{
    f32 x1=this->x;
    ;
    f32 y1=this->y;
    ;
    f32 z1=this->z;
    ;
    f32 x2=v.x;
    ;
    f32 y2=v.y;
    ;
    f32 z2=v.z;
    ;
    this->x=y1*z2-z1*y2;;
    this->y=z1*x2-x1*z2;;
    this->z=x1*y2-y1*x2;;
    return*this;;

}

Vector3&Vector3::lerp(const Vector3&target,f32 t)
{
    (*this)+=(Vector3(target)-(*this))*t;
    return*this;;

}

Vector3&Vector3::nlerp(const Vector3&target,f32 t)
{
    this->lerp(target,t).nor();
    return*this;;

}

Vector3&Vector3::slerp(const Vector3&target,f32 t)
{
    if(t==0)
    {
        return*this;;

    }
    ;
    f32 theta=angle(target);
    ;
    f32 sinTheta=sinf(theta);
    ;
    return this->mul(sinf((1-t)*theta)/sinTheta).add(target*(sinf(t*theta)/sinTheta));;

}

f32 Vector3::angle(const Vector3&v)const
{
    return acosf(this->dot(v)/(this->len()*v.len()));;

}

f32 Vector3::angle(const Vector3&v,const Vector3&n)const
{
    f32 dot=this->dot(v);
    ;
    Vector3 cross=Vector3(*this).cross(v);
    ;
    f32 radians=atan2f(cross.len(),dot);
    ;
    if(n.dot(cross)<0.0)
    {
        radians=(2.0*MathUtils::PI)-radians;;

    }
    else
    {
        radians=radians;;

    }
    ;
    return radians;;

}

Vector3&Vector3::clamp(f32 maxLength)
{
    if(this->sqrlen()>maxLength*maxLength)
    {
        this->nor();
        this->mul(maxLength);

    }
    ;
    return*this;;

}

Vector3&Vector3::operator=(const Vector3&other)
{
    return set(other);;

}

Vector3&Vector3::operator+=(const Vector3&rhs)
{
    return this->add(rhs);;

}

Vector3&Vector3::operator-=(const Vector3&rhs)
{
    return this->sub(rhs);;

}

Vector3&Vector3::operator*=(const Vector3&rhs)
{
    return this->mul(rhs);;

}

Vector3&Vector3::operator/=(const Vector3&rhs)
{
    return this->div(rhs);;

}

Vector3&Vector3::operator+=(f32 rhs)
{
    return this->add(rhs);;

}

Vector3&Vector3::operator-=(f32 rhs)
{
    return this->sub(rhs);;

}

Vector3&Vector3::operator*=(f32 rhs)
{
    return this->mul(rhs);;

}

Vector3&Vector3::operator/=(f32 rhs)
{
    return this->div(rhs);;

}

bool Vector3::operator==(const Vector3&rhs)const
{
    return this->eq(rhs);;

}

bool Vector3::operator!=(const Vector3&rhs)const
{
    return!((*this)==rhs);;

}

Vector3&Vector3::operator+(const Vector3&rhs)const
{
    return Vector3(*this)+=rhs;;

}

Vector3&Vector3::operator-(const Vector3&rhs)const
{
    return Vector3(*this)-=rhs;;

}

Vector3&Vector3::operator*(const Vector3&rhs)const
{
    return Vector3(*this)*=rhs;;

}

Vector3&Vector3::operator/(const Vector3&rhs)const
{
    return Vector3(*this)/=rhs;;

}

Vector3&Vector3::operator+(f32 rhs)const
{
    return Vector3(*this)+=rhs;;

}

Vector3&Vector3::operator-(f32 rhs)const
{
    return Vector3(*this)-=rhs;;

}

Vector3&Vector3::operator-()const
{
    return Vector3(*this)*=-1;;

}

Vector3&Vector3::operator*(f32 rhs)const
{
    return Vector3(*this)*=rhs;;

}

Vector3&Vector3::operator/(f32 rhs)const
{
    return Vector3(*this)/=rhs;;

}

