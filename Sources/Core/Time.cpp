#include "Core/Time.h"

#include "Core/Application.h"

Core::Time::Time() :
    m_timeScale(1.f),
    m_targetFrameRate(60),
    m_fixedDeltaTime(1.f/30.f),
    m_fixCount(0)
{
}

float Core::Time::DeltaTime()
{
    return GetFrameTime() * Application::GetInstance().GetTime().TimeScale();
}

float Core::Time::FixedDeltaTime()
{
    return (Application::GetInstance().GetTime().m_fixedDeltaTime < GetFrameTime() ? Application::GetInstance().GetTime().m_fixedDeltaTime : GetFrameTime()) * Application::GetInstance().GetTime().TimeScale();
}

int Core::Time::Fixing()
{
    return (Application::GetInstance().GetTime().m_fixCount == 0 ? 1 : Application::GetInstance().GetTime().m_fixCount);
}


#ifdef _EDITOR
void Core::Time::ShowEditorControl()
{
}
#endif // _EDITOR


void Core::Time::Update()
{
    m_fixCount = 0;

    static float tempDeltaTime = 0.f;
    tempDeltaTime = GetFrameTime();

    while (tempDeltaTime > m_fixedDeltaTime)
    {
        tempDeltaTime -= m_fixedDeltaTime;
        ++m_fixCount;
    }
}