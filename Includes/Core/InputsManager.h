#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Define.h"

namespace Core
{
	class InputsManager
	{
	public:
		InputsManager() {}
		~InputsManager() {}

#ifdef _EDITOR
		void ShowEditorControl();
#endif // _EDITOR

	private:
	};
}