class MathUtils
{
    public:
        f32 FLOAT_EPSILON = 0.0000001;

        const f32 PI = std::acos(-1.0);
        f32 PI_2 = PI / 2.0;
        f32 PI_4 = PI / 4.0;
        f32 INV_PI = 1.0 / PI;
        f32 INV_2_PI = 2.0 / PI;
        f32 INV_2_SQRTPI = 2 / std::sqrt(PI);
        f32 SQRT2 = std::sqrt(2.0);
        f32 INV_SQRT2 = 1.0 / std::sqrt(2.0);
        f32 E = 2.71828182845904523536;
        f32 LOG2E = std::log2(E);
        f32 LOG10E = std::log10(E);
        f32 LN2 = std::log1p(2.0);
        f32 LN10 = std::log1p(10.0);

        f32 PI_180 = PI / 180.0;
        f32 INV_PI_180 = 180.0 / PI;
};