class Vector2
{
public:

	float x = 0;
	float y = 0;

    Vector2()
    {
        x = 0;
        y = 0;
    }

    Vector2(float x, float y)
    {
        this.x = x;
        this.y = y;
    }

    Vector2(Vector2 other)
    {
        this.x = other.x;
        this.y = other.y;
    }

    // Vector2(Vector3 other)
    // {
    //     this->x = other.x;
    //     this->y = other.y;
    // }

    // Vector2(Vector4 other)
    // {
    //     this->x = other.x;
    //     this->y = other.y;
    // }

    // Vector2& set(f32 x, f32 y)
    // {
    //     this->x = x;
    //     this->y = y;
    //     return *this;
    // }

    // Vector2& set(const Vector2& rhs)
    // {
    //     this->set(rhs.x, rhs.y);
    //     return *this;
    // }

    // Vector2& set(const Vector3& rhs)
    // {
    //     this->set(rhs.x, rhs.y);
    //     return *this;
    // }

    // Vector2& set(const Vector4& rhs)
    // {
    //     this->set(rhs.x, rhs.y);
    //     return *this;
    // }

    Vector2 add(Vector2 rhs)
    {
        x = x + rhs.x;
        y = y + rhs.y;
        return this;
    }

    // Vector2& sub(const Vector2& rhs)
    // {
    //     x = x - rhs.x;
    //     y = y - rhs.y;
    //     return *this;
    // }

    // Vector2& mul(const Vector2& rhs)
    // {
    //     x = x * rhs.x;
    //     y = y * rhs.y;
    //     return *this;
    // }

    // Vector2& div(const Vector2& rhs)
    // {
    //     ASSERT_MSG(rhs.x != 0, "Division by zero.");
    //     ASSERT_MSG(rhs.y != 0, "Division by zero.");
    //     x = x / rhs.x;
    //     y = y / rhs.y;
    //     return *this;
    // }

    // Vector2& add(f32 rhs)
    // {
    //     x = x + rhs;
    //     y = y + rhs;
    //     return *this;
    // }

    // Vector2& sub(f32 rhs)
    // {
    //     x = x - rhs;
    //     y = y - rhs;
    //     return *this;
    // }

    // Vector2& mul(f32 rhs)
    // {
    //     x = x * rhs;
    //     y = y * rhs;
    //     return *this;
    // }

    // Vector2& div(f32 rhs)
    // {
    //     ASSERT_MSG(rhs != 0, "Division by zero.");
    //     x = x / rhs;
    //     y = y / rhs;
    //     return *this;
    // }

    // f32 dot(const Vector2& v) const
    // {
    //     return this->x * v.x + this->y * v.y;
    // }

    // f32 sqrlen() const
    // {
    //     return this->dot(*this);
    // }

    // f32 sqrdst(const Vector2& v) const
    // {
    //     Vector2 sub = Vector2(v) - (*this);
    //     return sub.dot(sub);
    // }

    // f32 len() const
    // {
    //     return sqrtf(this->sqrlen());
    // }

    // f32 max() const
    // {
    //     return std::max(x, y);
    // }

    // f32 min() const
    // {
    //     return std::min(x, y);
    // }

    // Vector2& nor()
    // {
    //     f32 len = this->len();

    //     ASSERT_MSG(len > 0, "Length is zero.");
    //     this->div(len);

    //     return *this;
    // }

    // f32 dst(const Vector2& v) const
    // {
    //     return sqrtf(this->sqrdst(v));
    // }

    // bool eq(const Vector2& v) const
    // {
    //     return MathUtils::eqf(this->x, v.x) && MathUtils::eqf(this->y, v.y);
    // }

    // bool eq(const Vector2& v, f32 e) const
    // {
    //     return MathUtils::eqf(this->x, v.x, e) && MathUtils::eqf(this->y, v.y, e);
    // }

    // Vector2& lerp(const Vector2& target, f32 t)
    // {
    //     (*this) += (Vector2(target) - (*this)) * t;
    //     return *this;
    // }

    // f32 angle(const Vector2& v) const
    // {
    //     f32 angle = atan2f(v.y, v.x) - atan2f(this->y, this->x);
    //     return angle < 0 ? angle += 2 * MathUtils::PI : angle;
    // }

    // Vector2& clamp(f32 maxLength)
    // {
    //     if (this->sqrlen() > maxLength* maxLength)
    //     {
    //         this->nor();
    //         this->mul(maxLength);
    //     }

    //     return *this;
    // }
};