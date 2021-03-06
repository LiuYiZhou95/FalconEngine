#pragma once

#include <FalconEngine/Graphics/Common.h>

#include <FalconEngine/Graphics/Renderer/Primitive.h>

namespace FalconEngine
{

class AABB;

class IndexBuffer;

class Material;

class VertexBuffer;
class VertexFormat;
class VertexGroup;

// @summary Represents bundle of geometry and all the metadata used in rendering.
#pragma warning(disable: 4251)
class FALCON_ENGINE_API Mesh : public Object
{
    FALCON_ENGINE_RTTI_DECLARE;

public:
    /************************************************************************/
    /* Constructors and Destructor                                          */
    /************************************************************************/
    explicit Mesh(std::shared_ptr<Primitive> primitive, std::shared_ptr<Material> material);
    virtual ~Mesh();

protected:
    Mesh();

public:
    /************************************************************************/
    /* Public Members                                                       */
    /************************************************************************/
    const AABB *
    GetAABB() const;

    const Material *
    GetMaterial() const;

    std::shared_ptr<Material>
    GetMaterial();

    void
    SetMaterial(std::shared_ptr<Material> material);

    const Primitive *
    GetPrimitive() const;

    std::shared_ptr<Primitive>
    GetPrimitive();

    /************************************************************************/
    /* Deep and Shallow Copy                                                */
    /************************************************************************/
    virtual void
    CopyTo(Mesh *lhs) const;

    virtual Mesh *
    GetClone() const;

protected:
    std::shared_ptr<Material>  mMaterial;
    std::shared_ptr<Primitive> mPrimitive;
};
#pragma warning(default: 4251)

}
