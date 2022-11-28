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
    return getSize().len()<=MathUtils::FLOAT_EPSILON;;

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
    return(cube.getLeftTopFront().x-sphere.getRadius()-eps)<=sphere.getCenter().x&&(cube.getLeftTopFront().x+cube.getSize().x+sphere.getRadius()+eps)>=sphere.getCenter().x&&(cube.getLeftTopFront().y+sphere.getRadius()+eps)>=sphere.getCenter().y&&(cube.getLeftTopFront().y-cube.getSize().y-sphere.getRadius()-eps)<=sphere.getCenter().y&&(cube.getLeftTopFront().z+sphere.getRadius()+eps)>=sphere.getCenter().z&&(cube.getLeftTopFront().z-cube.getSize().z-sphere.getRadius()-eps)<=sphere.getCenter().z;;

}

bool Geometry::testCubePoint(const Cube&cube,const Vector3&point,f32 eps)
{
    return testCubeSphere(cube,Sphere(point,0.0),eps);;

}

bool Geometry::testSphereSphere(const Sphere&sphereA,const Sphere&sphereB,f32 eps)
{
    f32 distance=sphereA.getCenter().dst(sphereB.getCenter());
    ;
    return(distance<(sphereA.getRadius()+sphereB.getRadius()+eps));;

}

bool Geometry::testLineLine(const Line&lineA,const Line&lineB,Vector3&intersectionResult,f32 eps)
{
    Vector3 da=lineA.getEnd()-lineA.getStart();
    ;
    Vector3 db=lineB.getEnd()-lineB.getStart();
    ;
    Vector3 dc=lineB.getStart()-lineA.getStart();
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
        intersectionResult=lineA.getStart()+(da*s);;
        return true;;

    }
    ;
    return false;;

}

bool Geometry::testLineSphereSimple(const Line&line,const Sphere&sphere,f32 eps)
{
    bool lineIntersectsSphere=false;
    ;
    Vector3 closestPoint=closestPointInLine(line,sphere.getCenter());
    ;
    if(testSpherePoint(line.getStart(),sphere,eps)||testSpherePoint(line.getEnd(),sphere,eps))
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
    f32 radiusEps=sphere.getRadius()+eps;
    ;
    bool lineIntersectsSphere=false;
    ;
    Vector3 dVector=line.toVector();
    ;
    Vector3 startToCenter=line.getStart()-sphere.getCenter();
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
        intersectionResult1.x=line.getStart().x+t*(dVector.x);;
        intersectionResult1.y=line.getStart().y+t*(dVector.y);;
        intersectionResult1.z=line.getStart().z+t*(dVector.z);;
        return 1;;

    }
    ;
    if(det>MathUtils::FLOAT_EPSILON)
    {
        f32 t1=(-B+std::sqrt((B*B)-4.0*A*C))/(2.0*A);
        ;
        intersectionResult1.x=line.getStart().x+t1*(dVector.x);;
        intersectionResult1.y=line.getStart().y+t1*(dVector.y);;
        intersectionResult1.z=line.getStart().z+t1*(dVector.z);;
        f32 t2=(-B-std::sqrt((B*B)-4.0*A*C))/(2.0*A);
        ;
        intersectionResult2.x=line.getStart().x+t2*(dVector.x);;
        intersectionResult2.y=line.getStart().y+t2*(dVector.y);;
        intersectionResult2.z=line.getStart().z+t2*(dVector.z);;
        return 2;;

    }
    ;
    return 0;;

}

bool Geometry::testSpherePoint(const Vector3&point,const Sphere&sphere,f32 eps)
{
    return sphere.getCenter().dst(point)<=(sphere.getRadius()+eps);;

}

bool Geometry::testLinePoint(const Line&line,const Vector3&point,f32 eps)
{
    f32 d1=line.getStart().dst(point);
    ;
    f32 d2=line.getEnd().dst(point);
    ;
    f32 lineLen=line.getStart().dst(line.getEnd());
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
    Vector3 pointStartVector=(point-line.getStart());
    ;
    Vector3 lineVector=(line.getEnd()-line.getStart());
    ;
    f32 t=pointStartVector.dot(lineVector)/lineVector.dot(lineVector);
    ;
    t=std::fmaxf(t,0.0);;
    t=std::fminf(t,1.0);;
    return Vector3(line.getStart()+(lineVector.mul(t)));;

}

Vector3 Geometry::midPoint(const Line&line)
{
    return Vector3((line.getStart().x+line.getEnd().x)/2.0,(line.getStart().y+line.getEnd().y)/2.0,(line.getStart().z+line.getEnd().z)/2.0);;

}

