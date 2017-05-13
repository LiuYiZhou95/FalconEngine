#pragma once

#include <FalconEngine/Context/Header.h>

#include <FalconEngine/Context/GameEngineSettings.h>

namespace FalconEngine
{

class GameEngineSettings;
using GameEngineSettingsSharedPtr = std::shared_ptr<GameEngineSettings>;

class GameEngineData;
class FALCON_ENGINE_API GameEnginePlatform
{
public:
    /************************************************************************/
    /* Static Members                                                       */
    /************************************************************************/
    static GameEnginePlatform *
    GetInstance()
    {
        static GameEnginePlatform sInstance;
        return &sInstance;
    }

    /************************************************************************/
    /* Constructors and Destructor                                          */
    /************************************************************************/
    GameEnginePlatform();
    virtual ~GameEnginePlatform();

    /************************************************************************/
    /* Public Members                                                       */
    /************************************************************************/
    void
    Initialize(GameEngineData *data, GameEngineSettingsSharedPtr settings);

private:
    void
    InitializePlatform(GameEngineData *data, GameEngineSettingsSharedPtr settings);

    void
    InitializeExceptPlatform();
};

}
