#pragma once

#include "mcv_platform.h"

namespace GUI
{
  struct TParams
  {
    bool _visible = true;
    VEC2 _size;
    VEC2 _position;
    float _rotation = 0.f;
    VEC2 _scale;
  };

  struct TImageParams
  {
    const CTexture* _texture = nullptr;
    VEC4 _color;
  };
}