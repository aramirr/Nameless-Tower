#include <stdio.h>
#include <stdlib.h>
#include "input.h"

extern bool isPressed( int key );

CInput::CInput( ) { }

void CInput::update( ) {
  for( auto& k : keys ) {
    k.prev = k.curr;
    k.curr = isPressed( k.key );
  }
  _mouse.prev = _mouse.curr;
}

CInput::TKey& CInput::key( int akey ) {
  for( auto& k : keys ) {
    if( k.key == akey )
      return k;
  }
  TKey k;
  k.key = akey;
  k.prev = false;
  k.curr = isPressed( akey );
  keys.push_back( k );

  return keys.back( );
}

CInput::TMouse& CInput::mouse( ) {
  return _mouse;
}
