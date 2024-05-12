#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Define.h"

namespace Core
{
	class SettingsManager
	{
	public:
		SettingsManager() {}
		~SettingsManager() {}

#ifdef _EDITOR
		void ShowEditorControl();
#endif // _EDITOR

	private:
	};
}