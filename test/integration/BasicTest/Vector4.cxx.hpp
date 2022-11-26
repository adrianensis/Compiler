#ifndef Vector4_cxx_HPP
#define Vector4_cxx_HPP
#include "test/integration/BasicTest/BasicTypes.cxx.hpp"
class Vector4;
class Vector3;
class Vector2;
class Vector4{
    public:
    f32 x=0;
    f32 y=0;
    f32 z=0;
    f32 w=0;
    Vector4();
    Vector4(f32 x,f32 y,f32 z,f32 w);
    Vector4(const Vector4&other);
    Vector4(const Vector3&other);
    Vector4(const Vector2&other);
    Vector4(const Vector3&other,f32 w);
    Vector4(const Vector2&other,f32 z,f32 w);
    Vector4&set(f32 x,f32 y,f32 z,f32 w);
    Vector4&set(const Vector2&rhs);
    Vector4&set(const Vector3&rhs);
    Vector4&set(const Vector4&rhs);
    Vector4&add(const Vector4&rhs);
    Vector4&sub(const Vector4&rhs);
    Vector4&mul(const Vector4&rhs);
    Vector4&div(const Vector4&rhs);
    Vector4&add(f32 rhs);
    Vector4&sub(f32 rhs);
    Vector4&mul(f32 rhs);
    Vector4&div(f32 rhs);
    f32 dot(const Vector4&v)const;
    f32 sqrlen()const;
    f32 sqrdst(const Vector4&v)const;
    f32 len()const;
    f32 max()const;
    f32 min()const;
    Vector4&nor();
    f32 dst(const Vector4&v)const;
    bool eq(const Vector4&v,f32 e)const;
    bool eq(const Vector4&v)const;
    Vector4&lerp(const Vector4&target,f32 t);
    Vector4&clamp(f32 maxLength);
    Vector4&operator=(const Vector4&other);
    Vector4&operator+=(const Vector4&rhs);
    Vector4&operator-=(const Vector4&rhs);
    Vector4&operator*=(const Vector4&rhs);
    Vector4&operator/=(const Vector4&rhs);
    Vector4&operator+=(f32 rhs);
    Vector4&operator-=(f32 rhs);
    Vector4&operator*=(f32 rhs);
    Vector4&operator/=(f32 rhs);
    bool operator==(const Vector4&rhs)const;
    bool operator!=(const Vector4&rhs)const;
    Vector4&operator+(const Vector4&rhs)const;
    Vector4&operator-(const Vector4&rhs)const;
    Vector4&operator*(const Vector4&rhs)const;
    Vector4&operator/(const Vector4&rhs)const;
    Vector4&operator+(f32 rhs)const;
    Vector4&operator-(f32 rhs)const;
    Vector4&operator-()const;
    Vector4&operator*(f32 rhs)const;
    Vector4&operator/(f32 rhs)const;

};
#endif // Vector4.cxx