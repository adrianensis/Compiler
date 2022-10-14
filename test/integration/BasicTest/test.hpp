#ifndef test_HPP
#define test_HPP
#include "test/integration/BasicTest/test2.hpp"
#include "test/integration/BasicTest/test2.hpp"
class Test {
    Test2 memberA ;
    Test ( ) ;
    Test ( Test2 param , int a ) ;
    void method1 ( int param , int a ) ;
    int method2 ( int param , int a , float b ) ;

} ;
static float globalVar = 18 ;

int function ( int x ) ;
#endif // test