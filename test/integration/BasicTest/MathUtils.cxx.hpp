#ifndef MathUtils_cxx_HPP
#define MathUtils_cxx_HPP
#include "test/integration/BasicTest/BasicTypes.cxx.hpp"
class MathUtils{
    public:
    inline static const f32 FLOAT_EPSILON=0.0000001;
    inline static const f32 PI=std::acos(-1.0);
    inline static const f32 PI_2=PI/2.0;
    inline static const f32 PI_4=PI/4.0;
    inline static const f32 INV_PI=1.0/PI;
    inline static const f32 INV_2_PI=2.0/PI;
    inline static const f32 INV_2_SQRTPI=2/std::sqrt(PI);
    inline static const f32 SQRT2=std::sqrt(2.0);
    inline static const f32 INV_SQRT2=1.0/std::sqrt(2.0);
    inline static const f32 E=2.71828182845904523536;
    inline static const f32 LOG2E=std::log2(E);
    inline static const f32 LOG10E=std::log10(E);
    inline static const f32 LN2=std::log1p(2.0);
    inline static const f32 LN10=std::log1p(10.0);
    inline static const f32 PI_180=PI/180.0;
    inline static const f32 INV_PI_180=180.0/PI;
    static f32 rad(f32 deg);
    static f32 deg(f32 rad);
    static bool eqf(f32 a,f32 b,f32 epsilon);
    static bool eqf(f32 a,f32 b);
    static f32 clamp(f32 n,f32 lower,f32 upper);
    static f32 det(f32 a,f32 b,f32 c,f32 d);

};
#endif // MathUtils.cxx