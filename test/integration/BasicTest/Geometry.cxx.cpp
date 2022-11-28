#include "test/integration/BasicTest/Geometry.cxx.hpp"
#include "test/integration/BasicTest/BasicTypes.cxx.hpp"
#include "test/integration/BasicTest/Vector3.cxx.hpp"
#include "test/integration/BasicTest/MathUtils.cxx.hpp"
Face::Face(u32 index0,u32 index1,u32 index2)
{
    mIndex0=index0;;
    mIndex1=index1;;
    mIndex2=index2;;

}

Face&Face::operator=(const Face&rhs)
{
    mIndex0=rhs.mIndex0;;
    mIndex1=rhs.mIndex1;;
    mIndex2=rhs.mIndex2;;
    return*this;;

}

Line::Line()
{
    mVerticesCount=2;;

}

Line::Line(f32 xStart,f32 yStart,f32 xEnd,f32 yEnd)
{
    mStart.set(xStart,yStart,0);
    mEnd.set(xEnd,yEnd,0);

}

Line::Line(const Vector3&start,const Vector3&end)
{
    mStart.set(start);
    mEnd.set(end);

}

Vector3&Line::toVector()const
{
    return mEnd-mStart;;

}

bool Line::isZero()const
{
    return toVector().len()<=MathUtils::FLOAT_EPSILON;;

}

Rectangle::Rectangle()
{
    mVerticesCount=4;;

}

Rectangle::Rectangle(const Vector3&leftTopFront,const Vector3&size)
{
    set(leftTopFront,size);

}

void Rectangle::set(const Vector3&leftTopFront,const Vector3&size)
{
    mLeftTopFront.set(leftTopFront);
    mSize.set(size);

}

bool Rectangle::isZero()const
{
    return mSize.len()<=MathUtils::FLOAT_EPSILON;;

}

Cube::Cube()
{
    mVerticesCount=8;;

}

Cube::Cube(const Vector3&leftTopFront,const Vector3&size)
{
    set(leftTopFront,size);

}

Sphere::Sphere()
{
    mVerticesCount=0;;

}

Sphere::Sphere(const Vector3&center,f32 radius)
{
    mCenter.set(center);
    mRadius=radius;;

}

bool Sphere::isZero()const
{
    return mRadius<=MathUtils::FLOAT_EPSILON;;

}

bool Geometry::testCubeSphere(const Cube&cube,const Sphere&sphere,f32 eps)
{
    return(cube.mLeftTopFront.x-sphere.mRadius-eps)<=sphere.mCenter.x&&(cube.mLeftTopFront.x+cube.mSize.x+sphere.mRadius+eps)>=sphere.mCenter.x&&(cube.mLeftTopFront.y+sphere.mRadius+eps)>=sphere.mCenter.y&&(cube.mLeftTopFront.y-cube.mSize.y-sphere.mRadius-eps)<=sphere.mCenter.y&&(cube.mLeftTopFront.z+sphere.mRadius+eps)>=sphere.mCenter.z&&(cube.mLeftTopFront.z-cube.mSize.z-sphere.mRadius-eps)<=sphere.mCenter.z;;

}

bool Geometry::testCubePoint(const Cube&cube,const Vector3&point,f32 eps)
{
    return testCubeSphere(cube,Sphere(point,0.0),eps);;

}

bool Geometry::testSphereSphere(const Sphere&sphereA,const Sphere&sphereB,f32 eps)
{
    f32 distance=sphereA.mCenter.dst(sphereB.mCenter);
    ;
    return(distance<(sphereA.mRadius+sphereB.mRadius+eps));;

}

bool Geometry::testLineLine(const Line&lineA,const Line&lineB,Vector3&intersectionResult,f32 eps)
{
    Vector3 da=lineA.mEnd-lineA.mStart;
    ;
    Vector3 db=lineB.mEnd-lineB.mStart;
    ;
    Vector3 dc=lineB.mStart-lineA.mStart;
    ;
    if(dc.dot(Vector3(da).cross(db))!=0.0)
    {
        return false;;

    }
    ;
    f32 s=Vector3(dc).cross(db).dot(Vector3(da).cross(db))/Vector3(da).cross(db).sqrlen();
    ;
    if(s>=0.0&&s<=1.0)
    {
        intersectionResult=lineA.mStart+(da*s);;
        return true;;

    }
    ;
    return false;;

}

bool Geometry::testLineSphereSimple(const Line&line,const Sphere&sphere,f32 eps)
{
    bool lineIntersectsSphere=false;
    ;
    Vector3 closestPoint=closestPointInLine(line,sphere.mCenter);
    ;
    if(testSpherePoint(line.mStart,sphere,eps)||testSpherePoint(line.mEnd,sphere,eps))
    {
        lineIntersectsSphere=true;;

    }
    else
    {
        lineIntersectsSphere=testSpherePoint(closestPoint,sphere,eps);;

    }
    ;
    return lineIntersectsSphere;;

}

u8 Geometry::testLineSphere(const Line&line,const Sphere&sphere,f32 eps,Vector3&intersectionResult1,Vector3&intersectionResult2)
{
    f32 radiusEps=sphere.mRadius+eps;
    ;
    bool lineIntersectsSphere=false;
    ;
    Vector3 dVector=line.toVector();
    ;
    Vector3 startToCenter=line.mStart-sphere.mCenter;
    ;
    f32 A=dVector.sqrlen();
    ;
    f32 B=2.0*dVector.dot(startToCenter);
    ;
    f32 C=startToCenter.sqrlen()-radiusEps*radiusEps;
    ;
    f32 det=B*B-4.0*A*C;
    ;
    if(det==MathUtils::FLOAT_EPSILON)
    {
        f32 t=-B/(2*A);
        ;
        intersectionResult1.x=line.mStart.x+t*(dVector.x);;
        intersectionResult1.y=line.mStart.y+t*(dVector.y);;
        intersectionResult1.z=line.mStart.z+t*(dVector.z);;
        return 1;;

    }
    ;
    if(det>MathUtils::FLOAT_EPSILON)
    {
        f32 t1=(-B+std::sqrt((B*B)-4.0*A*C))/(2.0*A);
        ;
        intersectionResult1.x=line.mStart.x+t1*(dVector.x);;
        intersectionResult1.y=line.mStart.y+t1*(dVector.y);;
        intersectionResult1.z=line.mStart.z+t1*(dVector.z);;
        f32 t2=(-B-std::sqrt((B*B)-4.0*A*C))/(2.0*A);
        ;
        intersectionResult2.x=line.mStart.x+t2*(dVector.x);;
        intersectionResult2.y=line.mStart.y+t2*(dVector.y);;
        intersectionResult2.z=line.mStart.z+t2*(dVector.z);;
        return 2;;

    }
    ;
    return 0;;

}

bool Geometry::testSpherePoint(const Vector3&point,const Sphere&sphere,f32 eps)
{
    return sphere.mCenter.dst(point)<=(sphere.mRadius+eps);;

}

bool Geometry::testLinePoint(const Line&line,const Vector3&point,f32 eps)
{
    f32 d1=line.mStart.dst(point);
    ;
    f32 d2=line.mEnd.dst(point);
    ;
    f32 lineLen=line.mStart.dst(line.mEnd);
    ;
    bool pointIsInLine=false;
    ;
    if(d1+d2>=lineLen-eps&&d1+d2<=lineLen+eps)
    {
        pointIsInLine=true;;

    }
    ;
    return pointIsInLine;;

}

Vector3 Geometry::closestPointInLine(const Line&line,const Vector3&point)
{
    Vector3 pointStartVector=(point-line.mStart);
    ;
    Vector3 lineVector=(line.mEnd-line.mStart);
    ;
    f32 t=pointStartVector.dot(lineVector)/lineVector.dot(lineVector);
    ;
    t=std::fmaxf(t,0.0);;
    t=std::fminf(t,1.0);;
    return Vector3(line.mStart+(lineVector.mul(t)));;

}

Vector3 Geometry::midPoint(const Line&line)
{
    return Vector3((line.mStart.x+line.mEnd.x)/2.0,(line.mStart.y+line.mEnd.y)/2.0,(line.mStart.z+line.mEnd.z)/2.0);;

}

