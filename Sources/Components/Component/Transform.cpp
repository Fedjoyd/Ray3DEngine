#include "Components/Component/Transform.h"

void Components::Transform::FixedUpdate(Core::GameObject* p_gameObject)
{
}

#ifdef _EDITOR
void Components::Transform::EditorFixedUpdate(Core::GameObject* p_gameObject)
{
}
void Components::Transform::ShowEditorControl(const unsigned int p_indexComponent)
{
}
#endif // _EDITOR