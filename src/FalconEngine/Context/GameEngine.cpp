#include <FalconEngine/Context/GameEngine.h>
#include <FalconEngine/Context/GameEnginePlatform.h>
#include <FalconEngine/Context/GameEngineProfiler.h>
#include <FalconEngine/Context/GameEngineGraphics.h>
#include <FalconEngine/Context/GameEngineInput.h>
#include <FalconEngine/Context/GameEngineSettings.h>

#include <mutex>

#if defined(FALCON_ENGINE_WINDOW_GLFW)
#include <FalconEngine/Context/Platform/GLFW/GLFWGameEngineData.h>
#endif

namespace FalconEngine
{

/************************************************************************/
/* Constructors and Destructor                                          */
/************************************************************************/
GameEngine::GameEngine(Game *game) :
    mGame(game),
    mInput(nullptr)
{
    mGame->SetEngine(this);
}

GameEngine::~GameEngine()
{
    mGame = nullptr;
}

/************************************************************************/
/* Public Members                                                       */
/************************************************************************/
const GameEngineData *
GameEngine::GetData() const
{
    return mData;
}

void
GameEngine::Run()
{
    if (!mInitialized)
    {
        Initialize();
    }

    Loop();
    Destory();
}

void
GameEngine::Exit()
{
    mRunning = false;
}

/************************************************************************/
/* Protected Members                                                    */
/************************************************************************/
void
GameEngine::Initialize()
{
    std::lock_guard<std::mutex> lock(mMutex);

    // NOTE(Wuxiang): Thread-safe singleton pattern is used in all the game engine
    // context components.
    mData = new GameEngineData();
    mSettings = mGame->GetEngineSettings();

    mPlatform = GameEnginePlatform::GetInstance();
    if (mPlatform != nullptr)
    {
        mPlatform->Initialize(mData, mSettings);
    }

    mProfiler = GameEngineProfiler::GetInstance();
    if (mProfiler != nullptr)
    {
        mProfiler->Initialize(mData, mSettings);
    }

    mGraphics = GameEngineGraphics::GetInstance();
    if (mGraphics != nullptr)
    {
        mGraphics->Initialize(mData, mSettings);
    }

    mInput = GameEngineInput::GetInstance();
    if (mInput != nullptr)
    {
        mInput->Initialize(mData, mSettings);
    }

    if(mGame != nullptr)
    {
        mGame->Initialize();
    }

    mInitialized = true;
}

void
GameEngine::Loop()
{
    char lastFramePerformanceString[256];

    if (mGame != nullptr)
    {
        double lastFrameBegunMillisecond = GameTimer::GetMilliseconds();
        double lastRenderBegunMillisecond = lastFrameBegunMillisecond;
        int    lastFrameUpdateTotalCount = 0;

        // First update has no previous elapsed time
        double lastUpdateElapsedMillisecond = 0;

        while (mRunning)
        {
            double lastFrameEndedMillisecond = GameTimer::GetMilliseconds();
            double lastRenderEndedMillisecond = lastFrameEndedMillisecond;

            // Get the time elapsed during the LAST frame.
            double lastFrameElapsedMillisecond = lastFrameEndedMillisecond - lastFrameBegunMillisecond;
            double lastRenderElapsedMillisecond = lastRenderEndedMillisecond - lastRenderBegunMillisecond;

            // Reset frame start point.
            lastFrameBegunMillisecond = lastFrameEndedMillisecond;

            mInput->Update();

            // Reset update accumulated time elapsed.
            int    currentFrameUpdateTotalCount = 0;
            double currentUpdateTotalElapsedMillisecond = 0;
            double lastUpdateBegunMillisecond = GameTimer::GetMilliseconds();
            double lastUpdateEndedMillisecond = 0;
            do
            {
                mGame->Update(mInput, currentFrameUpdateTotalCount == 0 ? lastUpdateElapsedMillisecond + lastRenderElapsedMillisecond : lastUpdateElapsedMillisecond);
                ++currentFrameUpdateTotalCount;

                lastUpdateEndedMillisecond = GameTimer::GetMilliseconds();

                // Get the time elapsed during the LAST update.
                lastUpdateElapsedMillisecond = lastUpdateEndedMillisecond - lastUpdateBegunMillisecond;
                currentUpdateTotalElapsedMillisecond += lastUpdateElapsedMillisecond;

                // Reset update start point.
                lastUpdateBegunMillisecond = lastUpdateEndedMillisecond;
            }
            while (currentUpdateTotalElapsedMillisecond + lastUpdateElapsedMillisecond <= mSettings->mGraphicsMillisecondPerRender);

            // Output performance profile
            double lastFrameFPS = 1000 / lastFrameElapsedMillisecond;

            mProfiler->mLastFrameElapsedMillisecond  = lastFrameElapsedMillisecond;
            mProfiler->mLastFrameUpdateTotalCount    = lastFrameUpdateTotalCount;
            mProfiler->mLastFrameFPS                 = lastFrameFPS;
            mProfiler->mLastUpdateElapsedMillisecond = lastUpdateElapsedMillisecond;
            mProfiler->mLastRenderElapsedMillisecond = lastRenderElapsedMillisecond;

            const int lastFramePerformanceStringLength = 100;
            snprintf(lastFramePerformanceString,
                     lastFramePerformanceStringLength,
                     "f%.02fms, u%.02fms, r%.02fms, %du/f, %0.2ff/s\n",
                     lastFrameElapsedMillisecond,
                     lastUpdateElapsedMillisecond,
                     lastRenderElapsedMillisecond,
                     lastFrameUpdateTotalCount,
                     lastFrameFPS);
            GameDebug::OutputString(lastFramePerformanceString);

            // Store last update count
            lastFrameUpdateTotalCount = currentFrameUpdateTotalCount;

            // Reset render start point.
            lastRenderBegunMillisecond = lastUpdateEndedMillisecond;

            mGame->RenderBegin(mGraphics);
            mGame->Render(mGraphics, 1.0f);
            mGame->RenderEnd(mGraphics);
        }
    }
}

void
GameEngine::Destory()
{
    if (mGame != nullptr)
    {
        mGame->Destory();
    }

    delete mData;
}

}
