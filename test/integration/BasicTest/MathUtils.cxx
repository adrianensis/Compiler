decl_module(MathUtils);

import_module(Basic);

class MathUtils
{
    public:
        static const f32 FLOAT_EPSILON = 0.0000001;

        static const f32 PI = std::acos(-1.0);
        static const f32 PI_2 = PI / 2.0;
        static const f32 PI_4 = PI / 4.0;
        static const f32 INV_PI = 1.0 / PI;
        static const f32 INV_2_PI = 2.0 / PI;
        static const f32 INV_2_SQRTPI = 2 / std::sqrt(PI);
        static const f32 SQRT2 = std::sqrt(2.0);
        static const f32 INV_SQRT2 = 1.0 / std::sqrt(2.0);
        static const f32 E = 2.71828182845904523536;
        static const f32 LOG2E = std::log2(E);
        static const f32 LOG10E = std::log10(E);
        static const f32 LN2 = std::log1p(2.0);
        static const f32 LN10 = std::log1p(10.0);

        static const f32 PI_180 = PI / 180.0;
        static const f32 INV_PI_180 = 180.0 / PI;
};