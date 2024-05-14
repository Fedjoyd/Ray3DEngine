#include "Debug/Log.h"

#include <vector>
#include <cstdio>
#include <cstdarg>

Debug::Log* Debug::Log::m_singleton = nullptr;

const char* LOG_LEVEL_char[] = {
	" DEBUG ",
	" INFO  ",
	"WARNING",
	" ERROR ",
	" FATAL "
};

#ifdef _EDITOR
#include "imgui.h"
#include "imgui_stdlib.h"

ImVec4 LOG_LEVEL_color[] = {
	{0.7f, 0.7f, 0.7f, 1.f},
	{1.f, 1.f, 1.f, 1.f},
	{1.f, 1.f, 0.f, 1.f},
	{1.f, 0.f, 0.f, 1.f},
	{0.6f, 0.f, 0.f, 1.f},
};
#endif // _EDITOR

void Debug::Log::Print(const LOG_LEVEL level, const bool writeFL, const char* p_file, const unsigned p_line)
{
	if (m_singleton == nullptr)
		return;

	static std::string FormatedFileLine = "";

	if (writeFL)
		FormatedFileLine = "[" + std::string(p_file) + "][line " + std::to_string(p_line) + "]";

	if (m_singleton->m_externalLogger != nullptr)
		m_singleton->m_externalLogger->Print("[" + std::string(LOG_LEVEL_char[(unsigned int)level]) + "]" + FormatedFileLine + m_cout.str());

#ifdef _EDITOR
	m_singleton->m_logList.push_back({ level, FormatedFileLine, m_cout.str() });
	m_singleton->m_scrollToBottom = true;
#endif

	FormatedFileLine = "";
	m_cout.str("");
}

#if defined (_DEBUG) || defined (_DEBUG_LOG)
void Debug::Log::PrintLog(const LOG_LEVEL p_level, const char* p_file, const unsigned p_line, const char* p_fmt, ...)
{
	if (m_singleton == nullptr)
		return;

	while (m_singleton->m_writting.test_and_set()) {}

	std::vector<char> CurrentBuffer = std::vector<char>(strlen(p_fmt) + 512);

	va_list args;
	va_start(args, p_fmt);
	vsnprintf(CurrentBuffer.data(), (CurrentBuffer.size() - 1), p_fmt, args);
	va_end(args);

	m_singleton->m_cout << CurrentBuffer.data();
	m_singleton->Print(p_level, true, p_file, p_line);

	m_singleton->m_writting.clear();
}
#else // _DEBUG
void Debug::Log::PrintLog(const LOG_LEVEL p_level, const char* p_fmt, ...)
{
	if (m_singleton == nullptr)
		return;

	while (m_singleton->m_writting.test_and_set()) {}

	std::vector<char> CurrentBuffer = std::vector<char>(strlen(p_fmt) + 512);

	va_list args;
	va_start(args, p_fmt);
	vsnprintf(CurrentBuffer.data(), (CurrentBuffer.size() - 1), p_fmt, args);
	va_end(args);

	m_singleton->m_cout << CurrentBuffer.data();
	m_singleton->Print(p_level, false);

	m_singleton->m_writting.clear();
}
#endif // !_DEBUG

#ifdef _EDITOR
void Debug::Log::ShowEditorControl()
{
	if (ImGui::Button("Clear")) Clear();
	ImGui::SameLine();
	if (ImGui::Button("\\/##LogScrollToBottom"))
		m_scrollToBottom = true;

	ImGui::SameLine();
	ImGui::Checkbox("Debug##Log", &(LOG_LEVEL_show[0u]));
	ImGui::SameLine();
	ImGui::Checkbox("Info##Log", &(LOG_LEVEL_show[1u]));
	ImGui::SameLine();
	ImGui::Checkbox("Warning##Log", &(LOG_LEVEL_show[2u]));
	ImGui::SameLine();
	ImGui::Checkbox("Error##Log", &(LOG_LEVEL_show[3u]));
	ImGui::SameLine();
	ImGui::Checkbox("Fatal##Log", &(LOG_LEVEL_show[4u]));

	ImGui::Separator();

	ImGui::BeginChild("scrolling");
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));

	for (LogItem& logToDisplay : m_logList)
		if (LOG_LEVEL_show[(unsigned int)logToDisplay.Level])
		{
			ImGui::TextColored(LOG_LEVEL_color[(unsigned int)logToDisplay.Level], "[%s]%s : %s", LOG_LEVEL_char[(unsigned int)logToDisplay.Level], logToDisplay.FileLine.c_str(), logToDisplay.Text.c_str());
			//ImGui::Text("[%s]%s : %s", LOG_LEVEL_char[(unsigned int)logToDisplay.Level], logToDisplay.FileLine.c_str(), logToDisplay.Text.c_str());
		}

	if (m_scrollToBottom)
		ImGui::SetScrollHereY(1.f);
	m_scrollToBottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();
}
#endif // _EDITOR


void Debug::Log::Clear()
{
#ifdef _EDITOR
	m_logList.clear();
#endif // _EDITOR

	if (m_externalLogger != nullptr)
		m_externalLogger->Clear();
}

Debug::Log::Log() :
	m_externalLogger(nullptr),
	m_cout()
{
}