Vector2 :: Vector2 ( ) {
    x = 0 ;
    y = 0 ;

}

Vector2 :: Vector2 ( float x , float y ) {
    this -> x = x ;
    this -> y = y ;

}

Vector2 :: Vector2 ( Vector2 * other ) {
    this -> x = other -> x ;
    this -> y = other -> y ;

}

Vector2 * Vector2 :: add ( Vector2 * rhs ) {
    return this ;

}

