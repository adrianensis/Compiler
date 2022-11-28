decl_module(Geometry);

import_module(MathUtils);
import_module(Vector2);
import_module(Vector3);
import_module(Vector4);

stack class Face
{
	// GENERATE_METADATA_STRUCT(Face);
	
public:
    Face(u32 index0, u32 index1, u32 index2)
    {
        mIndex0 = index0;
        mIndex1 = index1;
        mIndex2 = index2;
    }

	Face operator=(const Face rhs)
	{
        mIndex0 = rhs.mIndex0;
        mIndex1 = rhs.mIndex1;
        mIndex2 = rhs.mIndex2;
        return this;
	}

public:
	u32 mIndex0 = 0;
    u32 mIndex1 = 0;
    u32 mIndex2 = 0;
};

stack class Shape //: public ObjectBase
{
    // GENERATE_METADATA(Shape)
    
protected: 
    u32 mVerticesCount = 0;

public:

    virtual bool isZero() const = 0;

    // TODO: autogenerate
    u32 getVerticesCount() const { return mVerticesCount; }

    // void serialize(JSON json) const override { }
	// void deserialize(const JSON json) override { }

    // COPY(Shape)
    // {
    //     DO_COPY(mVerticesCount)
    // }

    // GET(VerticesCount)
};

stack class Line: public Shape
{
    // GENERATE_METADATA(Line)

private: 
    Vector3 mStart = Vector3();
    Vector3 mEnd = Vector3();

public:
    Line() { mVerticesCount = 2; }

    Line(f32 xStart, f32 yStart, f32 xEnd, f32 yEnd)//: Line()
    {
        mStart.set(xStart,yStart, 0);
        mEnd.set(xEnd, yEnd, 0);
    }

    Line(const Vector3 start, const Vector3 end)//: Line()
    {
        mStart.set(start);
        mEnd.set(end);
    }

    Vector3 toVector() const
    {
        return mEnd - mStart;
    }

    bool isZero() const override
    {
        return toVector().len() <= MathUtils::FLOAT_EPSILON;
    }

    // TODO: autogenerate
    const Vector3 getStart() const { return mStart; }
    const Vector3 getEnd() const { return mEnd; }

    // COPY(Line)
    // {
    //     Shape::copy(other);
    //     DO_COPY(mStart)
    //     DO_COPY(mEnd)
    // }

    // void serialize(JSON json) const override
    // {
    //     Shape::serialize(json);

    //     SERIALIZE("start", mStart)
    //     SERIALIZE("end", mEnd)
    // }

    // void deserialize(const JSON json) override
    // {
    //     Shape::deserialize(json);

    //     DESERIALIZE("start", mStart)
    //     DESERIALIZE("end", mEnd)
    // }

    // CRGET_SET(Start)
    // CRGET_SET(End)
};

// // class Cube;

stack class Rectangle: public Shape
{
    // GENERATE_METADATA(Rectangle)

protected: 
    Vector3 mLeftTopFront = Vector3();
    Vector3 mSize = Vector3();

public:
    Rectangle() { mVerticesCount = 4; }

    Rectangle(const Vector3 leftTopFront, const Vector3 size)//: Rectangle()
    {
        set(leftTopFront, size);
    }

    // Rectangle(const Cube cube)
    // {
    //     set(cube.getLeftTopFront(), cube.getSize());
    // }

    void set(const Vector3 leftTopFront, const Vector3 size)
    {
        mLeftTopFront.set(leftTopFront);
        mSize.set(size);
    }

    bool isZero() const override
    {
        return getSize().len() <= MathUtils::FLOAT_EPSILON;
    }

    const Vector3 getLeftTopFront() const { return mLeftTopFront; }
    const Vector3 getSize() const { return mSize; }

    // COPY(Rectangle)
    // {
    //     Shape::copy(other);
    //     DO_COPY(mLeftTopFront)
    //     DO_COPY(mSize)
    // }

    // void serialize(JSON json) const override
    // {
    //     Shape::serialize(json);

    //     SERIALIZE("left_top", mLeftTopFront)
    //     SERIALIZE("size", mSize)
    // }

    // void deserialize(const JSON json) override
    // {
    //     Shape::deserialize(json);

    //     DESERIALIZE("left_top", mLeftTopFront);
    //     DESERIALIZE("size", mSize);
    // }

    // CRGET_SET(LeftTopFront)
    // CRGET_SET(Size)
};

stack class Cube: public Rectangle
{
    // GENERATE_METADATA(Cube)

public:
    Cube() { mVerticesCount = 8; }

    Cube(const Vector3 leftTopFront, const Vector3 size)//: Cube()
    {
        set(leftTopFront, size);
    }

    // Cube(const Rectangle rectangle)
    // {
    //     set(rectangle.getLeftTopFront(), rectangle.getSize());
    // }
};

stack class Sphere: public Shape
{
    // GENERATE_METADATA(Sphere)

protected: 
    Vector3 mCenter = Vector3();
    f32 mRadius = 0;

public:
    Sphere() { mVerticesCount = 0; }

    Sphere(const Vector3 center, f32 radius)//: Sphere()
    {
        mCenter.set(center);
        mRadius = radius;
    }

    bool isZero() const override
    {
        return mRadius <= MathUtils::FLOAT_EPSILON;
    }

    const Vector3 getCenter() const { return mCenter; }
    f32 getRadius() const { return mRadius; }

    // COPY(Sphere)
    // {
    //     Shape::copy(other);
    //     DO_COPY(mCenter)
    //     DO_COPY(mRadius)
    // }

    // CRGET_SET(Center)
    // CRGET_SET(Radius)
};

class Geometry
{
public:

    static bool testCubeSphere(const Cube cube, const Sphere sphere, f32 eps)
    {
        return
            (cube.getLeftTopFront().x - sphere.getRadius() - eps) <= sphere.getCenter().x &&
            (cube.getLeftTopFront().x + cube.getSize().x + sphere.getRadius() + eps) >= sphere.getCenter().x &&
            (cube.getLeftTopFront().y + sphere.getRadius() + eps) >= sphere.getCenter().y &&
            (cube.getLeftTopFront().y - cube.getSize().y - sphere.getRadius() - eps) <= sphere.getCenter().y &&
            (cube.getLeftTopFront().z + sphere.getRadius() + eps) >= sphere.getCenter().z &&
            (cube.getLeftTopFront().z - cube.getSize().z - sphere.getRadius() - eps) <= sphere.getCenter().z;
    }

    static bool testCubePoint(const Cube cube, const Vector3 point, f32 eps)
    {
        return testCubeSphere(cube, Sphere(point, 0.0), eps);
    }

    static bool testSphereSphere(const Sphere sphereA, const Sphere sphereB, f32 eps)
    {
        f32 distance = sphereA.getCenter().dst(sphereB.getCenter());
        return (distance < (sphereA.getRadius() + sphereB.getRadius() + eps));
    }

    static bool testLineLine(const Line lineA, const Line lineB, Vector3 intersectionResult, f32 eps)
    {

        Vector3 da = lineA.getEnd() - lineA.getStart(); 
        Vector3 db = lineB.getEnd() - lineB.getStart();
        Vector3 dc = lineB.getStart() - lineA.getStart();

        if (dc.dot(Vector3(da).cross(db)) != 0.0) // lines are not coplanar
        {
            return false;
        }

        f32 s = Vector3(dc).cross(db).dot(Vector3(da).cross(db)) / Vector3(da).cross(db).sqrlen();

        if (s >= 0.0 && s <= 1.0)
        {
            intersectionResult = lineA.getStart() + (da * s);
            return true;
        }

        return false;
    }

    static bool testLineSphereSimple(const Line line, const Sphere sphere, f32 eps)
    {
        bool lineIntersectsSphere = false;

        Vector3 closestPoint = closestPointInLine(line, sphere.getCenter());

        if (testSpherePoint(line.getStart(), sphere, eps) || testSpherePoint(line.getEnd(), sphere, eps))
        {
            lineIntersectsSphere = true;
        }
        else
        {
            lineIntersectsSphere = testSpherePoint(closestPoint, sphere, eps);
        }

        return lineIntersectsSphere;
    }

    static u8 testLineSphere(const Line line, const Sphere sphere, f32 eps, Vector3 intersectionResult1, Vector3 intersectionResult2)
    {


        f32 radiusEps = sphere.getRadius() + eps;

        bool lineIntersectsSphere = false;

        Vector3 dVector = line.toVector();

        Vector3 startToCenter = line.getStart() - sphere.getCenter();

        f32 A = dVector.sqrlen();
        f32 B = 2.0 * dVector.dot(startToCenter);
        f32 C = startToCenter.sqrlen() - radiusEps * radiusEps;

        f32 det = B * B - 4.0 * A * C;

        if ( det == MathUtils::FLOAT_EPSILON )
        {

            f32 t = -B/(2*A);

            intersectionResult1.x = line.getStart().x + t*(dVector.x);
            intersectionResult1.y = line.getStart().y + t*(dVector.y);
            intersectionResult1.z = line.getStart().z + t*(dVector.z);

            return 1;
        }

        if ( det > MathUtils::FLOAT_EPSILON )
        {

            f32 t1 = (-B + std::sqrt( (B*B) - 4.0*A*C )) / (2.0*A);

            intersectionResult1.x = line.getStart().x + t1*(dVector.x);
            intersectionResult1.y = line.getStart().y + t1*(dVector.y);
            intersectionResult1.z = line.getStart().z + t1*(dVector.z);
            
            f32 t2 = (-B - std::sqrt((B*B) - 4.0*A*C )) / (2.0*A);

            intersectionResult2.x = line.getStart().x + t2*(dVector.x);
            intersectionResult2.y = line.getStart().y + t2*(dVector.y);
            intersectionResult2.z = line.getStart().z + t2*(dVector.z);

            return 2;
        }

        return 0;
    }

    static bool testSpherePoint(const Vector3 point, const Sphere sphere, f32 eps)
    {
        return sphere.getCenter().dst(point) <= (sphere.getRadius() + eps);
    }

    static bool testLinePoint(const Line line, const Vector3 point, f32 eps)
    {
        f32 d1 = line.getStart().dst(point);
        f32 d2 = line.getEnd().dst(point);

        f32 lineLen = line.getStart().dst(line.getEnd());

        bool pointIsInLine = false;

        if (d1 + d2 >= lineLen - eps && d1 + d2 <= lineLen + eps)
        {
            pointIsInLine = true;
        }

        return pointIsInLine;
    }

    static Vector3 closestPointInLine(const Line line, const Vector3 point)
    {
        Vector3 pointStartVector = (point - line.getStart()); // .nor();
        Vector3 lineVector = (line.getEnd() - line.getStart()); // .nor();

        f32 t = pointStartVector.dot(lineVector) / lineVector.dot(lineVector);

        t = std::fmaxf(t, 0.0); // clamp to 0
        t = std::fminf(t, 1.0); // clampt to 1

        return Vector3(line.getStart() + (lineVector.mul(t)));
    }

    static Vector3 midPoint(const Line line)
    {
        return Vector3((line.getStart().x + line.getEnd().x) / 2.0, (line.getStart().y + line.getEnd().y) / 2.0, (line.getStart().z + line.getEnd().z) / 2.0);
    }

};