#pragma once

#include "Define.h"

namespace Physics
{
	class PhysicsManager
	{
	public:
		PhysicsManager() {}
		~PhysicsManager() {}

#ifdef _EDITOR
		void ShowEditorControl();
#endif // _EDITOR

	private:
	};
}