#pragma once

#include "Define.h"

#include "raylib.h"
#include "raymath.h"
#include "raygui.h"

#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "Application.h"

namespace Core
{
	class Time
	{
	public:
		Time();
		~Time() {}

		static float  DeltaTime();
		static float  FixedDeltaTime();
		static int    Fixing();

		inline float& TimeScale() { return m_timeScale; }
		inline int& TargetFrameRate() { return m_targetFrameRate; }

		inline float TimeScale() const { return m_timeScale; }
		inline int TargetFrameRate() const { return m_targetFrameRate; }

#ifdef _EDITOR
		void ShowEditorControl();
#endif // _EDITOR

		void Update();

	private:

		float  m_timeScale;
		int    m_targetFrameRate;
		float  m_fixedDeltaTime;
		int    m_fixCount;
	};
}