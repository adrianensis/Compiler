#ifndef Vector3_cxx_HPP
#define Vector3_cxx_HPP
#include "test/integration/BasicTest/BasicTypes.cxx.hpp"
class Vector3;
class Vector2;
class Vector4;
class Vector3{
    public:
    f32 x=0;
    f32 y=0;
    f32 z=0;
    Vector3();
    Vector3(f32 x,f32 y,f32 z);
    Vector3(const Vector3&other);
    Vector3(const Vector2&other);
    Vector3(const Vector2&other,f32 z);
    Vector3(const Vector4&other);
    Vector3&set(f32 x,f32 y,f32 z);
    Vector3&set(const Vector2&rhs);
    Vector3&set(const Vector3&rhs);
    Vector3&set(const Vector4&rhs);
    Vector3&add(const Vector3&rhs);
    Vector3&sub(const Vector3&rhs);
    Vector3&mul(const Vector3&rhs);
    Vector3&div(const Vector3&rhs);
    Vector3&add(f32 rhs);
    Vector3&sub(f32 rhs);
    Vector3&mul(f32 rhs);
    Vector3&div(f32 rhs);
    f32 dot(const Vector3&v)const;
    f32 sqrlen()const;
    f32 sqrdst(const Vector3&v)const;
    f32 len()const;
    f32 max()const;
    f32 min()const;
    Vector3&nor();
    f32 dst(const Vector3&v)const;
    bool eq(const Vector3&v)const;
    bool eq(const Vector3&v,f32 e)const;
    Vector3&cross(const Vector3&v);
    Vector3&lerp(const Vector3&target,f32 t);
    Vector3&nlerp(const Vector3&target,f32 t);
    Vector3&slerp(const Vector3&target,f32 t);
    f32 angle(const Vector3&v)const;
    f32 angle(const Vector3&v,const Vector3&n)const;
    Vector3&clamp(f32 maxLength);
    Vector3&operator=(const Vector3&other);
    Vector3&operator+=(const Vector3&rhs);
    Vector3&operator-=(const Vector3&rhs);
    Vector3&operator*=(const Vector3&rhs);
    Vector3&operator/=(const Vector3&rhs);
    Vector3&operator+=(f32 rhs);
    Vector3&operator-=(f32 rhs);
    Vector3&operator*=(f32 rhs);
    Vector3&operator/=(f32 rhs);
    bool operator==(const Vector3&rhs)const;
    bool operator!=(const Vector3&rhs)const;
    Vector3&operator+(const Vector3&rhs)const;
    Vector3&operator-(const Vector3&rhs)const;
    Vector3&operator*(const Vector3&rhs)const;
    Vector3&operator/(const Vector3&rhs)const;
    Vector3&operator+(f32 rhs)const;
    Vector3&operator-(f32 rhs)const;
    Vector3&operator-()const;
    Vector3&operator*(f32 rhs)const;
    Vector3&operator/(f32 rhs)const;

};
#endif // Vector3.cxx