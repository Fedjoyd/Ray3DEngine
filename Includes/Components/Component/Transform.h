#pragma once

#include <memory>
#include <string>
#include <typeinfo>

#include "Define.h"

#include "Components/IComponent.h"

namespace Components
{
	class Transform : public IComponent, public IPhysicComponent
	{
	public:
		Transform() {}
		~Transform() {}

		REGISTER_COMPONENT(Transform, COMPONENT_TYPE_PHYSIC)

		virtual void FixedUpdate(Core::GameObject* p_gameObject) override;
#ifdef _EDITOR
		virtual void EditorFixedUpdate(Core::GameObject* p_gameObject) override;
		virtual void ShowEditorControl(const unsigned int p_indexComponent) override;
#endif // _EDITOR
	private:
	};
}