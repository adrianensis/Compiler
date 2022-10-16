#ifndef Vector2_cxx_HPP
#define Vector2_cxx_HPP
class Vector2 {
    public : float x = 0 ;
    float y = 0 ;
    Vector2 ( ) ;
    Vector2 ( float x , float y ) ;
    Vector2 ( Vector2 * other ) ;
    Vector2 * add ( Vector2 * rhs ) ;

} ;
#endif // Vector2.cxx