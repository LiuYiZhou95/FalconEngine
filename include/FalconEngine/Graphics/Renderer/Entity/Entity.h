#pragma once

#include <FalconEngine/GraphicsInclude.h>

#include <FalconEngine/Core/EventHandler.h>
#include <FalconEngine/Core/Object.h>
#include <FalconEngine/Math/Vector3.h>

namespace FalconEngine
{

class Entity;
using EntitySharedPtr = std::shared_ptr<Entity>;

class GameEngineInput;

class Node;
using NodeSharedPtr = std::shared_ptr<Node>;

class Visual;
using VisualSharedPtr = std::shared_ptr<Visual>;

class Vector3f;

class Entity : public Object
{
    FALCON_ENGINE_RTTI_DECLARE;

public:
    /************************************************************************/
    /* Constructors and Destructor                                          */
    /************************************************************************/
    explicit Entity(NodeSharedPtr node);
    virtual ~Entity();

public:
    NodeSharedPtr
    GetNode() const;

    NodeSharedPtr
    GetNode();

    virtual Vector3f
    GetScale() const;

    void
    SetScale(Vector3f scale);

    Vector3f
    GetPosition() const;

    void
    SetPosition(Vector3f position);

    virtual void
    Update(GameEngineInput *input, double elapsed);

protected:
    virtual void
    UpdateLocalTransform(bool initiator);

    virtual void
    UpdateLocalTransformFeedback(bool initiator);

public:
    // TODO(Wuxiang): Id is not implemented.
    int         mId;
    std::string mName;

protected:
    NodeSharedPtr       mNode;
    EventCallback<bool> mNodeUpdateBegunHandler;
    EventCallback<bool> mNodeUpdateEndedHandler;

    // NOTE(Wuxiang): Note that all transform data is local to parent node.
    Vector3f      mLocalPosition;
    Vector3f      mLocalScale;
    bool          mLocalTransformIsCurrent;
};

}
