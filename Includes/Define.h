#pragma once

#include <stdint.h>

#define COMPONENT_V1
//#define COMPONENT_V2

//#define _DEBUG_LOG

// -------------------------------

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600

#define DEFAULT_NEAR 0.001f
#define DEFAULT_FAR 1000.f

// -------------------------------

#define DEFAULT_SCENE_PATH "Ressources/Scenes/"

namespace Debug
{
	enum class LOG_LEVEL : unsigned int
	{
		DEBUG = 0u,
		INFO,
		WARNING,
		mERROR,
		FATAL
	};
}

#ifdef _EDITOR
namespace Core
{
	enum class TYPE_ITEM_SELECTED
	{
		NONE,
		GAMEOBJECT,
		RESSOURCE
	};

	struct ItemSelectionData
	{
		TYPE_ITEM_SELECTED type;
		union
		{
			int64_t RessourceUUID;
			size_t GameObjectIndex;
		} Data;
	};
}
#endif // _EDITOR