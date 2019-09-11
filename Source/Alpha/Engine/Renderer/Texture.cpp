//
// Created by adval on 9/11/2019.
//

#include "Texture.h"

namespace Alpha
{

    Pointer<Texture2D> Texture2D::Create(const std::string &path)
    {
        ALPHA_ASSERT(false, "Texture2D::Create is currently not supported!");
        return Alpha::Pointer<Texture2D>();
    }
}

