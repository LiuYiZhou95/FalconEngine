#pragma once

#include <FalconEngine/Core/Objects/Rtti.h>

namespace FalconEngine {

class Object
{
public:
    FALCON_ENGINE_DECLARE_RTTI

    bool IsExactly(const Rtti& type) const;
    bool IsDerived(const Rtti& type) const;
    bool IsExactlyTypeOf(const Object *object) const;
    bool IsDerivedTypeOf(const Object *object) const;
};

}