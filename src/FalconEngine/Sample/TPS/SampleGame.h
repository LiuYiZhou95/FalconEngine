#pragma once

#include <FalconEngine/Content.h>
#include <FalconEngine/Context.h>
#include <FalconEngine/Core.h>
#include <FalconEngine/Graphics.h>
#include <FalconEngine/Input.h>
#include <FalconEngine/Math.h>

namespace FalconEngine
{

class SampleGame : public Game
{
public:
    SampleGame();
    ~SampleGame();

public:
    virtual void
    Initialize() override;

    virtual void
    Render(GameEngineGraphics *graphics, double percent) override;

    virtual void
    Update(GameEngineInput *input, double elapsed) override;

private:
    HandednessRight mHandedness;

    // Camera
    ThirdPersonCameraSharedPtr mCamera;

    // Fonts
    const BitmapFont *mFont_Console = nullptr;
    const BitmapFont *mFont_Chinese = nullptr;

    // Scene
    NodeSharedPtr mSceneNode;
    NodeSharedPtr mRootNode;

    PhongLightingEffectSharedPtr mSceneLightingEffect;
    std::shared_ptr<Light> mDirectionalLight;

    std::vector<const Light *> mScenePointLightList;
    std::vector<const Light *> mSceneSpotLightList;

    // Entities
    // std::shared_ptrEntity> mCharacter;
    // std::shared_ptr<LightEntity> mLamp1;
    // std::shared_ptr<LightEntity> mLamp2;
    std::shared_ptr<PointLightEntity> mPointLight1;
    std::shared_ptr<PointLightEntity> mPointLight2;
    std::shared_ptr<PointLightEntity> mPointLight3;
    std::shared_ptr<SceneEntity> mRoom;
    std::shared_ptr<Entity> mOrigin;
};

}