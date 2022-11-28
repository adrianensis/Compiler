#ifndef Geometry_cxx_HPP
#define Geometry_cxx_HPP
#include "test/integration/BasicTest/BasicTypes.cxx.hpp"
#include "test/integration/BasicTest/Vector3.cxx.hpp"
class Face;
class Vector3;
class Cube;
class Sphere;
class Line;
class Face{
    public:
    Face(u32 index0,u32 index1,u32 index2);
    Face&operator=(const Face&rhs);
    public:
    u32 mIndex0=0;
    u32 mIndex1=0;
    u32 mIndex2=0;

};
class Shape{
    protected:
    u32 mVerticesCount=0;
    public:
    virtual bool isZero()const=0;
    u32 getVerticesCount()const;

};
class Line:public Shape{
    public:
    private:
    Vector3 mStart=Vector3();
    Vector3 mEnd=Vector3();
    public:
    Line();
    Line(f32 xStart,f32 yStart,f32 xEnd,f32 yEnd);
    Line(const Vector3&start,const Vector3&end);
    Vector3&toVector()const;
    bool isZero()const override;
    const Vector3&getStart()const;
    const Vector3&getEnd()const;

};
class Rectangle:public Shape{
    public:
    protected:
    Vector3 mLeftTopFront=Vector3();
    Vector3 mSize=Vector3();
    public:
    Rectangle();
    Rectangle(const Vector3&leftTopFront,const Vector3&size);
    void set(const Vector3&leftTopFront,const Vector3&size);
    bool isZero()const override;
    const Vector3&getLeftTopFront()const;
    const Vector3&getSize()const;

};
class Cube:public Rectangle{
    public:
    public:
    Cube();
    Cube(const Vector3&leftTopFront,const Vector3&size);

};
class Sphere:public Shape{
    public:
    protected:
    Vector3 mCenter=Vector3();
    f32 mRadius=0;
    public:
    Sphere();
    Sphere(const Vector3&center,f32 radius);
    bool isZero()const override;
    const Vector3&getCenter()const;
    f32 getRadius()const;

};
class Geometry{
    public:
    static bool testCubeSphere(const Cube&cube,const Sphere&sphere,f32 eps);
    static bool testCubePoint(const Cube&cube,const Vector3&point,f32 eps);
    static bool testSphereSphere(const Sphere&sphereA,const Sphere&sphereB,f32 eps);
    static bool testLineLine(const Line&lineA,const Line&lineB,Vector3&intersectionResult,f32 eps);
    static bool testLineSphereSimple(const Line&line,const Sphere&sphere,f32 eps);
    static u8 testLineSphere(const Line&line,const Sphere&sphere,f32 eps,Vector3&intersectionResult1,Vector3&intersectionResult2);
    static bool testSpherePoint(const Vector3&point,const Sphere&sphere,f32 eps);
    static bool testLinePoint(const Line&line,const Vector3&point,f32 eps);
    static Vector3 closestPointInLine(const Line&line,const Vector3&point);
    static Vector3 midPoint(const Line&line);

};
#endif // Geometry.cxx