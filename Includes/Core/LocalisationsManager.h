#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Define.h"

namespace Core
{
	class LocalisationsManager
	{
	public:
		LocalisationsManager() {}
		~LocalisationsManager() {}

#ifdef _EDITOR
		void ShowEditorControl();
#endif // _EDITOR

	private:
	};
}