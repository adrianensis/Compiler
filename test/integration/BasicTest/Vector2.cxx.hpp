#ifndef Vector2_cxx_HPP
#define Vector2_cxx_HPP
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
#include "test/integration/BasicTest/Vector2.cxx.hpp"
class Vector2{
    public:float x=0;
    float y=0;
    Vector2();
    Vector2(float x,float y);
    Vector2(Vector2*other);
    Vector2*set(float x,float y);
    Vector2*add(Vector2*rhs);
    Vector2*sub(Vector2*rhs);
    Vector2*mul(Vector2*rhs);
    Vector2*div(Vector2*rhs);
    Vector2*add(float rhs);
    Vector2*sub(float rhs);
    Vector2*mul(float rhs);
    Vector2*div(float rhs);
    float dot(Vector2*v)const;

};
#endif // Vector2.cxx