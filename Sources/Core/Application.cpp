#include "Core/Application.h"

Core::Application Core::Application::m_singleton;

void Core::Application::Initialize()
{
	Debug::Log::SetInstance(&(m_singleton.m_log));

	R3DE_INFO("Initialisation Finished !!");
}

void Core::Application::Update()
{
}

void Core::Application::Draw()
{
}