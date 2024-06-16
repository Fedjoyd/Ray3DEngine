#include "Components/Component/Transform.h"

#include "Debug/Assert.h"
#include "Debug/Log.h"
#define R3DE_CURRENT_FILE "Transform.cpp"

Snowflake<8456134653461L> Components::Transform::transformUUIDgenerator;
std::unordered_map<int64_t, Components::Transform*> Components::Transform::listGlobalTransform = std::unordered_map<int64_t, Components::Transform*>();

Components::Transform::Transform() :
	m_id(transformUUIDgenerator.nextid()),
	m_parentId(0),
	m_childsId(),
	m_position({ 0 }),
	m_rotation(QuaternionIdentity()),
	m_scale({ 1.f, 1.f, 1.f }),
	m_localMatrix({ MatrixIdentity() }),
	m_globalMatrix({ MatrixIdentity() }),
	m_update(true)
{
	while (listGlobalTransform.find(m_id) != listGlobalTransform.end())
		m_id = transformUUIDgenerator.nextid();

	listGlobalTransform.insert({ m_id, this });
}

Components::Transform::~Transform()
{
	if (m_parentId != 0)
		if (listGlobalTransform.find(m_parentId) != listGlobalTransform.end())
			listGlobalTransform[m_parentId]->RemoveChild(m_id);

	for (int64_t childId : m_childsId)
		if (listGlobalTransform.find(childId) != listGlobalTransform.end())
			listGlobalTransform[childId]->SetParent(0, false);

	listGlobalTransform.erase(m_id);
}

void Components::Transform::SetParent(int64_t p_parentId, bool unsetParent)
{
	if (IsAlreadyParent(m_id, p_parentId))
	{
		R3DE_ERROR("impossible to make transform parent to himself !!");
		return;
	}

	if (m_parentId != 0 && unsetParent)
		if (listGlobalTransform.find(m_parentId) != listGlobalTransform.end())
			listGlobalTransform[m_parentId]->RemoveChild(m_id);

	m_parentId = p_parentId;
	m_update = true;

	if (m_parentId != 0)
	{
		if (listGlobalTransform.find(m_parentId) != listGlobalTransform.end())
			listGlobalTransform[m_parentId]->AddChild(m_id);
		else
		{
			R3DE_WARNING("transform %lld doesn't exist", m_parentId);
			m_parentId = 0;
		}
	}
}

void Components::Transform::AddChild(int64_t p_childId)
{
	m_childsId.push_back(p_childId);
}

void Components::Transform::RemoveChild(int64_t p_childId)
{
	for (size_t i = 0; i < m_childsId.size(); i++)
		if (m_childsId[i] == p_childId)
		{
			m_childsId.erase(m_childsId.begin() + i);
			i--;
		}
}

Quaternion Components::Transform::GetGlobalRotation() const
{
	return QuaternionFromMatrix(m_globalMatrix);

	/*if (m_parentId != 0)
		return QuaternionMultiply(listGlobalTransform[m_parentId]->GetGlobalRotation(), m_rotation);

	return m_rotation;/**/
}

Vector3 Components::Transform::GetGlobalScale() const
{
	//return { Vector3Length(m_globalMatrix.vec.right), Vector3Length(m_globalMatrix.vec.up), Vector3Length(m_globalMatrix.vec.forward) };

	if (m_parentId != 0)
		return Vector3Multiply(listGlobalTransform[m_parentId]->GetGlobalScale(), m_scale);

	return m_scale;
}

void Components::Transform::DeserializeComponent(tinyxml2::XMLElement* p_XMLComponent)
{
	int64_t oldId = m_id;
	m_id = p_XMLComponent->Int64Attribute("Id", m_id);

	if (m_id != oldId)
	{
		listGlobalTransform.erase(oldId);

		if (listGlobalTransform.find(m_id) != listGlobalTransform.end())
		{
			R3DE_FATAL("Deserialize Trying to register Transform with already register ID");
			R3DE_ASSERT(false);
		}

		listGlobalTransform.insert({ m_id, this });
	}

	tinyxml2::XMLElement* ParentXML = p_XMLComponent->FirstChildElement("parent");
	if (ParentXML != nullptr)
		m_parentId = ParentXML->Int64Attribute("Id");

	tinyxml2::XMLElement* childXML = p_XMLComponent->FirstChildElement("childs");
	if (childXML != nullptr)
	{
		childXML = childXML->FirstChildElement("child");

		while (childXML != nullptr)
		{
			m_childsId.push_back(childXML->Int64Attribute("Id"));

			childXML = childXML->NextSiblingElement("child");
		}
	}

	tinyxml2::XMLElement* positionXML = p_XMLComponent->FirstChildElement("position");
	if (positionXML != nullptr)
	{
		m_position.x = positionXML->FloatAttribute("X");
		m_position.y = positionXML->FloatAttribute("Y");
		m_position.z = positionXML->FloatAttribute("Z");
	}

	tinyxml2::XMLElement* rotationXML = p_XMLComponent->FirstChildElement("rotation");
	if (rotationXML != nullptr)
	{
		m_rotation.x = rotationXML->FloatAttribute("I");
		m_rotation.y = rotationXML->FloatAttribute("J");
		m_rotation.z = rotationXML->FloatAttribute("K");
		m_rotation.w = rotationXML->FloatAttribute("R");
	}

	tinyxml2::XMLElement* scaleXML = p_XMLComponent->FirstChildElement("scale");
	if (scaleXML != nullptr)
	{
		m_scale.x = scaleXML->FloatAttribute("X", 1.f);
		m_scale.y = scaleXML->FloatAttribute("Y", 1.f);
		m_scale.z = scaleXML->FloatAttribute("Z", 1.f);
	}

	m_update = true;
}

void Components::Transform::SerializeComponent(tinyxml2::XMLElement* p_XMLComponent) const
{
	p_XMLComponent->SetAttribute("Id", m_id);

	if (m_parentId != 0)
	{
		tinyxml2::XMLElement* parentXML = p_XMLComponent->InsertNewChildElement("parent");
		if (parentXML != nullptr)
			parentXML->SetAttribute("Id", m_parentId);
	}

	if (m_childsId.size() > 0u)
	{
		tinyxml2::XMLElement* childsXML = p_XMLComponent->InsertNewChildElement("childs");
		tinyxml2::XMLElement* childXML = nullptr;

		if (childsXML != nullptr)
			for (std::vector<int64_t>::const_iterator ChildIte = m_childsId.begin(); ChildIte != m_childsId.end(); ChildIte++)
			{
				childXML = p_XMLComponent->InsertNewChildElement("child");
				if (childXML != nullptr)
					childXML->SetAttribute("Id", *ChildIte);
			}
	}

	tinyxml2::XMLElement* positionXML = p_XMLComponent->InsertNewChildElement("position");
	if (positionXML != nullptr)
	{
		positionXML->SetAttribute("X", m_position.x);
		positionXML->SetAttribute("Y", m_position.y);
		positionXML->SetAttribute("Z", m_position.z);
	}

	tinyxml2::XMLElement* rotationXML = p_XMLComponent->InsertNewChildElement("rotation");
	if (rotationXML != nullptr)
	{
		rotationXML->SetAttribute("I", m_rotation.x);
		rotationXML->SetAttribute("J", m_rotation.y);
		rotationXML->SetAttribute("K", m_rotation.z);
		rotationXML->SetAttribute("R", m_rotation.w);
	}

	tinyxml2::XMLElement* scaleXML = p_XMLComponent->InsertNewChildElement("scale");
	if (scaleXML != nullptr)
	{
		scaleXML->SetAttribute("X", m_scale.x);
		scaleXML->SetAttribute("Y", m_scale.y);
		scaleXML->SetAttribute("Z", m_scale.z);
	}
}

void Components::Transform::Start(Core::GameObject* p_gameObject)
{
	if (m_parentId != 0)
		if (listGlobalTransform.find(m_parentId) == listGlobalTransform.end())
			m_parentId = 0;

	for (size_t indexChildId = 0u; indexChildId < m_childsId.size(); indexChildId++)
	{
		if (listGlobalTransform.find(m_childsId[indexChildId]) != listGlobalTransform.end())
			continue;

		m_childsId.erase(m_childsId.begin() + indexChildId);
		indexChildId--;
	}

	m_update = true;
	UpdateTransform();
}

void Components::Transform::FixedUpdate(Core::GameObject* p_gameObject)
{
	UpdateTransform();
}

bool Components::Transform::IsAlreadyParent(int64_t m_transID, int64_t p_parentIdToTest)
{
	if (p_parentIdToTest == 0 || m_transID == 0)
		return false;

	int64_t currentParent = p_parentIdToTest;

	while (currentParent != 0)
	{
		if (currentParent == m_transID)
			return true;

		currentParent = listGlobalTransform[currentParent]->GetParent();
	}

	return false;
}

#ifdef _EDITOR
void Components::Transform::UpdateTransform(bool updateEditorRotation)
{
#else
void Components::Transform::UpdateTransform()
{
#endif // _EDITOR

	if (!m_update)
		return;

	m_localMatrix = MatrixMultiply(MatrixScale(m_scale.x, m_scale.y, m_scale.z), MatrixMultiply(QuaternionToMatrix(m_rotation), MatrixTranslate(m_position.x, m_position.y, m_position.z)));

	if (m_parentId != 0)
		m_globalMatrix = MatrixMultiply(m_localMatrix, listGlobalTransform[m_parentId]->GetGlobalMatrix());
	else
		m_globalMatrix = m_localMatrix;

	for (int64_t curChildId : m_childsId)
	{
		listGlobalTransform[curChildId]->m_update = true;
		listGlobalTransform[curChildId]->UpdateTransform();
	}

#ifdef _EDITOR
	if (updateEditorRotation)
		m_editorRotation = QuaternionToEuler(m_rotation);

	m_currentGuizmoMatrix[0] = m_globalMatrix.m0; m_currentGuizmoMatrix[1] = m_globalMatrix.m1; m_currentGuizmoMatrix[2] = m_globalMatrix.m2; m_currentGuizmoMatrix[3] = m_globalMatrix.m3;
	m_currentGuizmoMatrix[4] = m_globalMatrix.m4; m_currentGuizmoMatrix[5] = m_globalMatrix.m5; m_currentGuizmoMatrix[6] = m_globalMatrix.m6; m_currentGuizmoMatrix[7] = m_globalMatrix.m7;
	m_currentGuizmoMatrix[8] = m_globalMatrix.m8; m_currentGuizmoMatrix[9] = m_globalMatrix.m9; m_currentGuizmoMatrix[10] = m_globalMatrix.m10; m_currentGuizmoMatrix[11] = m_globalMatrix.m11;
	m_currentGuizmoMatrix[12] = m_globalMatrix.m12; m_currentGuizmoMatrix[13] = m_globalMatrix.m13; m_currentGuizmoMatrix[14] = m_globalMatrix.m14; m_currentGuizmoMatrix[15] = m_globalMatrix.m15;
#endif // _EDITOR

	m_update = false;
}

#ifdef _EDITOR
#include "imgui.h"
#include "imgui_stdlib.h"
#include "ImGuizmo.h"

#include "Core/Application.h"

void Components::Transform::EditorFixedUpdate(Core::GameObject* p_gameObject)
{
	if (toTest)
	{
		if (m_parentId != 0)
			if (listGlobalTransform.find(m_parentId) == listGlobalTransform.end())
				m_parentId = 0;

		for (size_t indexChildId = 0u; indexChildId < m_childsId.size(); indexChildId++)
		{
			if (listGlobalTransform.find(m_childsId[indexChildId]) != listGlobalTransform.end())
				continue;

			m_childsId.erase(m_childsId.begin() + indexChildId);
			indexChildId--;
		}

		m_transformName = p_gameObject->GetName() + "(TID : " + std::to_string(m_id) + ")";

		toTest = false;
	}

	UpdateTransform();
}

void Components::Transform::ShowEditorControl(const unsigned int p_indexComponent)
{
	ImGuizmo::SetID((int)p_indexComponent);

	ImGui::Text("Name :");
	ImGui::SameLine();
	ImGui::InputText(("##transName_transform-" + std::to_string(p_indexComponent)).c_str(), &m_transformName);
	ImGui::SetItemTooltip("For tracking purposes only");

	ImGui::Text("Id : %lld", m_id);

	if (m_parentId != 0)
	{
		ImGui::Text("Parent : %lld", m_parentId);
		ImGui::SameLine();
		if (ImGui::Button(("X##transform-" + std::to_string(p_indexComponent)).c_str()))
		{
			m_position = GetGlobalPosition();
			m_rotation = GetGlobalRotation();
			Vector3 tempScale = GetGlobalScale();

			SetParent(0);

			m_scale = tempScale;
		}
		ImGui::SameLine();
		if (ImGui::Button(("X (keep value)##transform-" + std::to_string(p_indexComponent)).c_str()))
			SetParent(0);
	}
	else
	{
		ImGui::Text("Parent : NONE");
		ImGui::SameLine();
		if (ImGui::Button(("Select parent##transform-" + std::to_string(p_indexComponent)).c_str()))
			ImGui::OpenPopup(("Select Tranform Parent##For" + std::to_string(p_indexComponent)).c_str());

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		if (ImGui::BeginPopupModal(("Select Tranform Parent##For" + std::to_string(p_indexComponent)).c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static int64_t currenttransformSelection = 0;

			if (ImGui::BeginListBox("##list transform", ImVec2(400, 0)))
			{
				for (std::unordered_map<int64_t, Transform*>::iterator n = listGlobalTransform.begin(); n != listGlobalTransform.end(); n++)
				{
					const bool is_selected = ((*n).first == currenttransformSelection);
					if (ImGui::Selectable((*n).second->m_transformName.c_str(), is_selected))
						currenttransformSelection = (*n).first;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndListBox();
			}

			ImGui::Separator();

			ImGui::SetItemDefaultFocus();
			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();

				Matrix TranfByParent = MatrixMultiply(m_localMatrix, MatrixInvert(listGlobalTransform[currenttransformSelection]->GetGlobalMatrix()));

				m_position = { TranfByParent.m12, TranfByParent.m13, TranfByParent.m14 };

				m_scale = {
					Vector3Length({ TranfByParent.m0, TranfByParent.m1, TranfByParent.m2 }),
					Vector3Length({ TranfByParent.m4, TranfByParent.m5, TranfByParent.m6 }),
					Vector3Length({ TranfByParent.m8, TranfByParent.m9, TranfByParent.m10 })
				};

				TranfByParent.m0 = TranfByParent.m0 / m_scale.x; TranfByParent.m1 = TranfByParent.m1 / m_scale.x; TranfByParent.m2 = TranfByParent.m2 / m_scale.x;
				TranfByParent.m4 = TranfByParent.m4 / m_scale.y; TranfByParent.m5 = TranfByParent.m5 / m_scale.y; TranfByParent.m6 = TranfByParent.m6 / m_scale.y;
				TranfByParent.m8 = TranfByParent.m8 / m_scale.z; TranfByParent.m9 = TranfByParent.m9 / m_scale.z; TranfByParent.m10 = TranfByParent.m10 / m_scale.z;

				m_rotation = QuaternionFromMatrix(TranfByParent);

				//float TranfByParentAsFloat[16] = {
				//	TranfByParent.m0,	TranfByParent.m1,	TranfByParent.m2,	TranfByParent.m3,
				//	TranfByParent.m4,	TranfByParent.m5,	TranfByParent.m6,	TranfByParent.m7,
				//	TranfByParent.m8,	TranfByParent.m9,	TranfByParent.m10,	TranfByParent.m11,
				//	TranfByParent.m12,	TranfByParent.m13,	TranfByParent.m14,	TranfByParent.m15
				//};

				//ImGuizmo::DecomposeMatrixToComponents((float*)TranfByParentAsFloat, (float*)&m_position, (float*)&m_editorRotation, (float*)&m_scale);

				//m_editorRotation.x = fmodf((m_editorRotation.x + (2 * PI)) + PI, (2 * PI)) - PI; // Loop around -180,180
				//m_editorRotation.y = fmodf((m_editorRotation.y + (2 * PI)) + PI, (2 * PI)) - PI; // Loop around -180,180
				//m_editorRotation.z = fmodf((m_editorRotation.z + (2 * PI)) + PI, (2 * PI)) - PI; // Loop around -180,180

				//m_rotation = QuaternionFromEuler(m_editorRotation.x, m_editorRotation.y, m_editorRotation.z);
				////m_rotation = Quaternion::GetQuatFromEuler(m_editorRotation);

				SetParent(currenttransformSelection);
			}

			ImGui::SameLine();
			if (ImGui::Button("OK (keep value)", ImVec2(140, 0)))
			{
				ImGui::CloseCurrentPopup();

				SetParent(currenttransformSelection);
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}
	}

	ImGui::Spacing();

	static ImGuizmo::OPERATION mCurrentGizmoOperation = ImGuizmo::TRANSLATE;

	if (ImGui::RadioButton(("Translate##Gizmotransform-" + std::to_string(p_indexComponent)).c_str(), mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton(("Rotate##Gizmotransform-" + std::to_string(p_indexComponent)).c_str(), mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton(("Scale##Gizmotransform-" + std::to_string(p_indexComponent)).c_str(), mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	if (ImGui::DragFloat3(("position##transform-" + std::to_string(p_indexComponent)).c_str(), (float*)&m_position, 0.1f))
		m_update = true;

	static bool showAsQuaternion = false;
	if (showAsQuaternion)
	{
		if (ImGui::DragFloat4(("rotation##Qtransform-" + std::to_string(p_indexComponent)).c_str(), (float*)&m_rotation, 0.01f))
			m_update = true;
	}
	else
	{
		if (ImGui::DragFloat3(("rotation##EAtransform-" + std::to_string(p_indexComponent)).c_str(), (float*)&m_editorRotation, 0.01f))
		{
			m_editorRotation.x = fmodf((m_editorRotation.x + (2 * PI)) + PI, (2 * PI)) - PI; // Loop around -180,180
			m_editorRotation.y = fmodf((m_editorRotation.y + (2 * PI)) + PI, (2 * PI)) - PI; // Loop around -180,180
			m_editorRotation.z = fmodf((m_editorRotation.z + (2 * PI)) + PI, (2 * PI)) - PI; // Loop around -180,180

			m_rotation = QuaternionFromEuler(m_editorRotation.x, m_editorRotation.y, m_editorRotation.z);
			//m_rotation = Quaternion::GetQuatFromEuler(m_editorRotation);

			m_update = true;
			UpdateTransform(false);
		}
	}

	ImGui::SameLine();
	ImGui::Checkbox(("##rotationTypeTransform-" + std::to_string(p_indexComponent)).c_str(), &showAsQuaternion);

	static bool fullScaleModification = false;
	if (fullScaleModification)
	{
		if (ImGui::DragFloat3(("scale##3transform-" + std::to_string(p_indexComponent)).c_str(), (float*)&m_scale, 0.1f))
			m_update = true;
	}
	else
	{
		if (ImGui::DragFloat(("scale##1transform-" + std::to_string(p_indexComponent)).c_str(), (float*)&(m_scale.x), 0.1f))
		{
			m_scale.y = m_scale.z = m_scale.x;

			m_update = true;
		}
	}

	ImGui::SameLine();
	ImGui::Checkbox(("##scaleTypeTransform-" + std::to_string(p_indexComponent)).c_str(), &fullScaleModification);

	if (Core::Application::FullscreenGame())
	{
		/*static Vector3 tempPosition = {0.f, 0.f, 0.f};
		static Vector3 tempDegreeRotation = { 0.f, 0.f, 0.f };
		static Vector3 tempScale = { 0.f, 0.f, 0.f };

		tempPosition = GetGlobalPosition();
		tempDegreeRotation = Vector3Scale(QuaternionToEuler(GetGlobalRotation()), RAD2DEG);
		tempScale = GetGlobalScale();

		ImGuizmo::RecomposeMatrixFromComponents((float*)&tempPosition, (float*)&tempDegreeRotation, (float*)&tempScale, (float*)&m_currentGuizmoMatrix);*/
		//m_currentGuizmoMatrix = m_globalMatrix;

		//m_currentGuizmoMatrix[0] = m_globalMatrix.m0; m_currentGuizmoMatrix[1] = m_globalMatrix.m1; m_currentGuizmoMatrix[2] = m_globalMatrix.m2; m_currentGuizmoMatrix[3] = m_globalMatrix.m3;
		//m_currentGuizmoMatrix[4] = m_globalMatrix.m4; m_currentGuizmoMatrix[5] = m_globalMatrix.m5; m_currentGuizmoMatrix[6] = m_globalMatrix.m6; m_currentGuizmoMatrix[7] = m_globalMatrix.m7;
		//m_currentGuizmoMatrix[8] = m_globalMatrix.m8; m_currentGuizmoMatrix[9] = m_globalMatrix.m9; m_currentGuizmoMatrix[10] = m_globalMatrix.m10; m_currentGuizmoMatrix[11] = m_globalMatrix.m11;
		//m_currentGuizmoMatrix[12] = m_globalMatrix.m12; m_currentGuizmoMatrix[13] = m_globalMatrix.m13; m_currentGuizmoMatrix[14] = m_globalMatrix.m14; m_currentGuizmoMatrix[15] = m_globalMatrix.m15;

		Core::CamerasManager& CM = Core::Application::GetCamerasManager();

		ImGuizmo::SetOrthographic(CM.GetCameraData().projection == CAMERA_ORTHOGRAPHIC);
		ImGuizmo::SetRect(0.f, 0.f, (float)GetRenderWidth(), (float)GetRenderHeight());
		//ImGuizmo::SetRect(100.f, 100.f, (float)GetRenderWidth() - 200.f, (float)GetRenderHeight() - 200.f);

		if (ImGuizmo::Manipulate(CM.GetImGuiViewMatrix(), CM.GetImGuiProjectionMatrix(), mCurrentGizmoOperation, ImGuizmo::MODE::WORLD, (float*)&m_currentGuizmoMatrix))
		{
			m_ProcessGuizmoMatrix.m0 = m_currentGuizmoMatrix[0]; m_ProcessGuizmoMatrix.m1 = m_currentGuizmoMatrix[1]; m_ProcessGuizmoMatrix.m2 = m_currentGuizmoMatrix[2]; m_ProcessGuizmoMatrix.m3 = m_currentGuizmoMatrix[3];
			m_ProcessGuizmoMatrix.m4 = m_currentGuizmoMatrix[4]; m_ProcessGuizmoMatrix.m5 = m_currentGuizmoMatrix[5]; m_ProcessGuizmoMatrix.m6 = m_currentGuizmoMatrix[6]; m_ProcessGuizmoMatrix.m7 = m_currentGuizmoMatrix[7];
			m_ProcessGuizmoMatrix.m8 = m_currentGuizmoMatrix[8]; m_ProcessGuizmoMatrix.m9 = m_currentGuizmoMatrix[9]; m_ProcessGuizmoMatrix.m10 = m_currentGuizmoMatrix[10]; m_ProcessGuizmoMatrix.m11 = m_currentGuizmoMatrix[11];
			m_ProcessGuizmoMatrix.m12 = m_currentGuizmoMatrix[12]; m_ProcessGuizmoMatrix.m13 = m_currentGuizmoMatrix[13]; m_ProcessGuizmoMatrix.m14 = m_currentGuizmoMatrix[14]; m_ProcessGuizmoMatrix.m15 = m_currentGuizmoMatrix[15];

			if (m_parentId != 0)
				m_ProcessGuizmoMatrix = MatrixMultiply(m_ProcessGuizmoMatrix, MatrixInvert(listGlobalTransform[m_parentId]->GetGlobalMatrix()));

			m_position = { m_ProcessGuizmoMatrix.m12, m_ProcessGuizmoMatrix.m13, m_ProcessGuizmoMatrix.m14 };

			m_scale = {
				Vector3Length({ m_ProcessGuizmoMatrix.m0, m_ProcessGuizmoMatrix.m1, m_ProcessGuizmoMatrix.m2 }),
				Vector3Length({ m_ProcessGuizmoMatrix.m4, m_ProcessGuizmoMatrix.m5, m_ProcessGuizmoMatrix.m6 }),
				Vector3Length({ m_ProcessGuizmoMatrix.m8, m_ProcessGuizmoMatrix.m9, m_ProcessGuizmoMatrix.m10 })
			};
			
			m_ProcessGuizmoMatrix.m0 = m_ProcessGuizmoMatrix.m0 / m_scale.x; m_ProcessGuizmoMatrix.m1 = m_ProcessGuizmoMatrix.m1 / m_scale.x; m_ProcessGuizmoMatrix.m2 = m_ProcessGuizmoMatrix.m2 / m_scale.x;
			m_ProcessGuizmoMatrix.m4 = m_ProcessGuizmoMatrix.m4 / m_scale.y; m_ProcessGuizmoMatrix.m5 = m_ProcessGuizmoMatrix.m5 / m_scale.y; m_ProcessGuizmoMatrix.m6 = m_ProcessGuizmoMatrix.m6 / m_scale.y;
			m_ProcessGuizmoMatrix.m8 = m_ProcessGuizmoMatrix.m8 / m_scale.z; m_ProcessGuizmoMatrix.m9 = m_ProcessGuizmoMatrix.m9 / m_scale.z; m_ProcessGuizmoMatrix.m10 = m_ProcessGuizmoMatrix.m10 / m_scale.z;

			m_rotation = QuaternionFromMatrix(m_ProcessGuizmoMatrix);

			m_update = true;
		}
	}

#ifdef _DEBUG
	ImGui::Spacing();

	ImGui::Text("[%.2f][%.2f][%.2f][%.2f]", m_globalMatrix.m0, m_globalMatrix.m1, m_globalMatrix.m2, m_globalMatrix.m3);
	ImGui::Text("[%.2f][%.2f][%.2f][%.2f]", m_globalMatrix.m4, m_globalMatrix.m5, m_globalMatrix.m6, m_globalMatrix.m7);
	ImGui::Text("[%.2f][%.2f][%.2f][%.2f]", m_globalMatrix.m8, m_globalMatrix.m9, m_globalMatrix.m10, m_globalMatrix.m11);
	ImGui::Text("[%.2f][%.2f][%.2f][%.2f]", m_globalMatrix.m12, m_globalMatrix.m13, m_globalMatrix.m14, m_globalMatrix.m15);
#endif // _DEBUG
}
#endif // _EDITOR