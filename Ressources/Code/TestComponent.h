#pragma once

#include "Define.h"

#include "Components/IComponent.h"

#include "Core/GameObject.h"
#include "Components/Component/Transform.h"
#include "TestRessource.h"

class TestComponent : public Components::IComponent, public Components::IRenderComponent
{
public:
    TestComponent() {}
    ~TestComponent() {}

    REGISTER_COMPONENT(TestComponent, Components::COMPONENT_TYPE_RENDER)

    void Draw(const Core::GameObject* p_gameObject) override
    {
        Components::Transform* TransformComp = nullptr;
        if (p_gameObject->TryGetComponent(&TransformComp))
        {
            Vector3 curVecPos = TransformComp->GetGlobalPosition();
            float rayo = TransformComp->GetGlobalScale().x;

            DrawSphere(curVecPos, rayo, BLUE);
        }
    }
    uint64_t GetDrawLayer() const override { return UINT64_MAX; }

#ifdef _EDITOR
    void ShowEditorControl(const unsigned int p_indexComponent) override;
#endif // _EDITOR

private:
    std::shared_ptr<TestRessource> m_TestRessource = nullptr;

#ifdef _EDITOR
    std::string m_TestRessourceName = "Empty";
#endif // _EDITOR
};