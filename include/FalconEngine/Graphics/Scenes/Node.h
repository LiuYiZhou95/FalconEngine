#pragma once

#include <vector>
#include <assimp/scene.h>

#include <FalconEngine/Graphics/Scenes/Mesh.h>
#include <FalconEngine/Graphics/Scenes/Spatial.h>

namespace FalconEngine
{

class Node : public Spatial
{
    FALCON_ENGINE_RTTI_DECLARE;

public:
    Node();
    Node(Model *model, const aiScene *scene, const aiNode *node);
    ~Node();

    // @summary This is the current number of elements in the child array. These
    // elements are not all guaranteed to be non-null. Thus, when you
    // iterate over the array and access children with GetChild(...), you
    // should test the return pointer to be non-null before dereferencing it.
    size_t
    ChildrenNum() const;

    // @summary Attach a child to this node. If the function succeeds, the return
    // value is the index i of the array where the child was stored, in which
    // case 0 <= i < ChildrenNum(). The first available empty slot of the
    // child array is used for storage. If all slots are filled, the child
    // is appended to the array (potentially causing a reallocation of the
    // array).
    //
    // The function fails when 'child' is null or when 'child' already has a
    // parent, in which case the return value is -1. The nodes form a tree,
    // not a more general directed acyclic graph. A consequence is that a
    // node cannot have more than one parent. For example,
    //     Node* node0 = <some node>;
    //     Spatial* child = <some child>;
    //     int index = node0->AttachChild(child);
    //     Node* node1 = <some node>;
    //
    //     // This asserts because 'child' already has a parent (node0).
    //     node1->AttachChild(child);
    //
    //     // The following is the correct way to give 'child' a new parent.
    //     node0->DetachChild(child);  // or node0->DetachChildAt(index);
    //     node1->AttachChild(child);
    //
    //     // In the last example before the DetachChild call, if 'child' is
    //     // referenced only by node0, the detach will cause 'child' to be
    //     // deleted (Node internally reference counts its children).  If
    //     // you want to keep 'child' around for later use, do the following.
    //     SpatialPtr saveChild = node0->GetChild(0);
    //     node0->DetachChild(saveChild);
    //     node1->AttachChild(saveChild);
    int
    AttachChild(SpatialSharedPtr child);

    // @summary Detach a child from this node. If the 'child' is non-null and in the
    // array, the return value is the index in the array that had stored the
    // child. Otherwise, the function returns -1.
    int
    DetachChild(SpatialSharedPtr child);

    // @summary Detach a child from this node. If 0 <= i < ChildrenNum(), the
    // return value is the child at index i; otherwise, the function returns
    // null.
    SpatialSharedPtr
    DetachChildAt(size_t i);

    // The same comments for AttachChild apply here regarding the inability
    // to have multiple parents. If 0 <= i < ChildrenNum(), the function
    // succeeds and returns i. If i is out of range, the function *still*
    // succeeds, appending the child to the end of the array. The return
    // value is the previous child stored at index i.
    SpatialSharedPtr
    SetChild(size_t i, SpatialSharedPtr child);

    // @summary Get the child at the specified index. If 0 <= i < GetNumChildren(),
    // the function succeeds and returns the child at that index--keep in mind
    // that child[i] could very well be null. If i is out of range, the
    // function returns null.
    SpatialSharedPtr
    ChildAt(size_t i);

protected:
    virtual void
    UpdateWorldTransform(double elapsed) override;

private:
    std::vector<SpatialSharedPtr> mChildren;
};

inline size_t
Node::ChildrenNum() const
{
    return mChildren.size();
}

typedef std::shared_ptr<Node> NodeSharedPtr;

}
