#ifndef Vector2_cxx_HPP
#define Vector2_cxx_HPP
#include "test/integration/BasicTest/BasicTypes.cxx.hpp"
class Vector2;
class Vector3;
class Vector3{
    public:
    f32 x=0;
    f32 y=0;
    f32 z=0;

};
class Vector2{
    public:
    f32 x=0;
    f32 y=0;
    Vector2();
    Vector2(f32 x,f32 y);
    Vector2(const Vector2&other);
    Vector2(const Vector3&other);
    Vector2&set(f32 x,f32 y);
    Vector2&set(const Vector2&rhs);
    Vector2&set(const Vector3&rhs);
    Vector2&add(const Vector2&rhs);
    Vector2&sub(const Vector2&rhs);
    Vector2&mul(const Vector2&rhs);
    Vector2&div(const Vector2&rhs);
    Vector2&add(f32 rhs);
    Vector2&sub(f32 rhs);
    Vector2&mul(f32 rhs);
    Vector2&div(f32 rhs);
    f32 dot(const Vector2&v)const;
    f32 sqrlen()const;
    f32 sqrdst(const Vector2&v)const;
    f32 len()const;
    f32 max()const;
    f32 min()const;
    Vector2&nor();
    f32 dst(const Vector2&v)const;
    bool eq(const Vector2&v)const;
    bool eq(const Vector2&v,f32 e)const;
    Vector2&lerp(const Vector2&target,f32 t);
    f32 angle(const Vector2&v)const;
    Vector2&clamp(f32 maxLength);
    Vector2&operator=(const Vector2&other);
    Vector2&operator+=(const Vector2&rhs);
    Vector2&operator-=(const Vector2&rhs);
    Vector2&operator*=(const Vector2&rhs);
    Vector2&operator/=(const Vector2&rhs);
    Vector2&operator+=(f32 rhs);
    Vector2&operator-=(f32 rhs);
    Vector2&operator*=(f32 rhs);
    Vector2&operator/=(f32 rhs);
    bool operator==(const Vector2&rhs)const;
    bool operator!=(const Vector2&rhs)const;
    Vector2&operator+(const Vector2&rhs)const;
    Vector2&operator-(const Vector2&rhs)const;
    Vector2&operator*(const Vector2&rhs)const;
    Vector2&operator/(const Vector2&rhs)const;
    Vector2&operator+(f32 rhs)const;
    Vector2&operator-(f32 rhs)const;
    Vector2&operator-()const;
    Vector2&operator*(f32 rhs)const;
    Vector2&operator/(f32 rhs)const;

};
#endif // Vector2.cxx