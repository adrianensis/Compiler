stack class Vector3
{
public:

	f32 x = 0;
	f32 y = 0;
	f32 z = 0;
};

stack class Vector2
{
public:

	f32 x = 0;
	f32 y = 0;

    Vector2()
    {
        x = 0;
        y = 0;
    }

    Vector2(f32 x, f32 y)
    {
        this.x = x;
        this.y = y;
    }

    Vector2(const Vector2 other)
    {
        this.x = other.x;
        this.y = other.y;
    }

    Vector2(const Vector3 other)
    {
        this.x = other.x;
        this.y = other.y;
    }

    // Vector2(Vector4 other)
    // {
    //     this->x = other.x;
    //     this->y = other.y;
    // }

    Vector2 set(f32 x, f32 y)
    {
        this.x = x;
        this.y = y;
        return this;
    }

    Vector2 set(const Vector2 rhs)
    {
        this.set(rhs.x, rhs.y);
        return this;
    }

    Vector2 set(const Vector3 rhs)
    {
        this.set(rhs.x, rhs.y);
        return this;
    }

    // Vector2& set(const Vector4& rhs)
    // {
    //     this->set(rhs.x, rhs.y);
    //     return *this;
    // }

    Vector2 add(const Vector2 rhs)
    {
        x = x + rhs.x;
        y = y + rhs.y;
        return this;
    }

    Vector2 sub(const Vector2 rhs)
    {
        x = x - rhs.x;
        y = y - rhs.y;
        return this;
    }

    Vector2 mul(const Vector2 rhs)
    {
        x = x * rhs.x;
        y = y * rhs.y;
        return this;
    }

    Vector2 div(const Vector2 rhs)
    {
        // ASSERT_MSG(rhs.x != 0, "Division by zero.");
        // ASSERT_MSG(rhs.y != 0, "Division by zero.");
        x = x / rhs.x;
        y = y / rhs.y;
        return this;
    }

    Vector2 add(f32 rhs)
    {
        x = x + rhs;
        y = y + rhs;
        return this;
    }

    Vector2 sub(f32 rhs)
    {
        x = x - rhs;
        y = y - rhs;
        return this;
    }

    Vector2 mul(f32 rhs)
    {
        x = x * rhs;
        y = y * rhs;
        return this;
    }

    Vector2 div(f32 rhs)
    {
        //ASSERT_MSG(rhs != 0, "Division by zero.");
        x = x / rhs;
        y = y / rhs;
        return this;
    }

    f32 dot(const Vector2 v) const
    {
        return this.x * v.x + (this.y * v.y);
    }

    f32 sqrlen() const
    {
        return this.dot(this);
    }

    f32 sqrdst(Vector2 v) const
    {
        Vector2 sub = Vector2(v).sub(this);
        return sub.dot(sub);
    }

    f32 len() const
    {
        return std::sqrt(this.sqrlen());
    }

    f32 max() const
    {
        return std::max(x, y);
    }

    f32 min() const
    {
        return std::min(x, y);
    }

    Vector2 nor()
    {
        f32 len = this.len();

        //ASSERT_MSG(len > 0, "Length is zero.");
        this.div(len);

        return this;
    }

    // f32 dst(Vector2 v) const
    // {
    //     return sqrtf(this.sqrdst(v));
    // }

    // bool eq(Vector2 v) const
    // {
    //     return MathUtils::eqf(this.x, v.x) && MathUtils::eqf(this.y, v.y);
    // }

    // bool eq(Vector2 v, f32 e) const
    // {
    //     return MathUtils::eqf(this.x, v.x, e) && MathUtils::eqf(this.y, v.y, e);
    // }

    // Vector2 lerp(Vector2 target, f32 t)
    // {
    //     (this) += Vector2(target).sub(this).mul(t);
    //     return this;
    // }

    f32 angle(const Vector2 v) const
    {
        f32 angle = std::atan2(v.y, v.x) - std::atan2(this.y, this.x);
        if(angle < 0)
        {
            angle += 2 * MathUtils::PI;
        }

        return angle;
        //return angle < 0 ? angle += 2 * MathUtils::PI : angle;
    }

    Vector2 clamp(f32 maxLength)
    {
        if (this.sqrlen() > maxLength* maxLength)
        {
            this.nor();
            this.mul(maxLength);
        }

        return this;
    }
};