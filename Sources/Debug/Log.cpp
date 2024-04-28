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
	" FATAL ",
};

//Color LOG_LEVEL_color[] = {
//	{0.7f, 0.7f, 0.7f, 1.f},
//	{1.f, 1.f, 1.f, 1.f},
//	{1.f, 1.f, 0.f, 1.f},
//	{1.f, 0.f, 0.f, 1.f},
//	{0.6f, 0.f, 0.f, 1.f},
//};

void Debug::Log::Print(const LOG_LEVEL level, const bool writeFL, const char* p_file, const unsigned p_line)
{
	if (m_singleton == nullptr)
		return;

	if (!LOG_LEVEL_show[(unsigned int)level])
	{
		m_cout.str("");
		return;
	}

	std::string fmt = "[" + std::string(LOG_LEVEL_char[(unsigned int)level]) + "]" + (writeFL ? "[" + std::string(p_file) + "][line " + std::to_string(p_line) + "]" : "") + " : " + m_cout.str() + "\n";
	m_cout.str("");

	if (m_singleton->m_externalLogger != nullptr)
		m_singleton->m_externalLogger->Print(fmt);
}

#ifdef _EDITOR
void Debug::Log::ShowEditorWindow(bool* p_opened)
{
	if (m_singleton == nullptr)
		return;

	// TODO : faire les controles pour la Fenetre des logs !!
}

void Debug::Log::Debug(const char* p_file, const unsigned p_line, const char* p_fmt, ...)
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
	m_singleton->Print(LOG_LEVEL::DEBUG, true, p_file, p_line);

	m_singleton->m_writting.clear();
}

void Debug::Log::Info(const char* p_file, const unsigned p_line, const char* p_fmt, ...)
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
	m_singleton->Print(LOG_LEVEL::INFO, true, p_file, p_line);

	m_singleton->m_writting.clear();
}

void Debug::Log::Warning(const char* p_file, const unsigned p_line, const char* p_fmt, ...)
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
	m_singleton->Print(LOG_LEVEL::WARNING, true, p_file, p_line);

	m_singleton->m_writting.clear();
}

void Debug::Log::Error(const char* p_file, const unsigned p_line, const char* p_fmt, ...)
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
	m_singleton->Print(LOG_LEVEL::ERROR, true, p_file, p_line);

	m_singleton->m_writting.clear();
}

void Debug::Log::Fatal(const char* p_file, const unsigned p_line, const char* p_fmt, ...)
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
	m_singleton->Print(LOG_LEVEL::FATAL, true, p_file, p_line);

	m_singleton->m_writting.clear();
}
#else // !_EDITOR
void Debug::Log::Debug(const char* p_fmt, ...)
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
	m_singleton->Print(LOG_LEVEL::DEBUG, true, p_file, p_line);

	m_singleton->m_writting.clear();
}

void Debug::Log::Info(const char* p_fmt, ...)
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
	m_singleton->Print(LOG_LEVEL::INFO, true, p_file, p_line);

	m_singleton->m_writting.clear();
}

void Debug::Log::Warning(const char* p_fmt, ...)
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
	m_singleton->Print(LOG_LEVEL::WARNING, true, p_file, p_line);

	m_singleton->m_writting.clear();
}

void Debug::Log::Error(const char* p_fmt, ...)
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
	m_singleton->Print(LOG_LEVEL::ERROR, true, p_file, p_line);

	m_singleton->m_writting.clear();
}

void Debug::Log::Fatal(const char* p_fmt, ...)
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
	m_singleton->Print(LOG_LEVEL::FATAL, true, p_file, p_line);

	m_singleton->m_writting.clear();
}
#endif


void Debug::Log::Clear()
{
	if (m_singleton == nullptr)
		return;

	if (m_singleton->m_externalLogger != nullptr)
		m_singleton->m_externalLogger->Clear();
}

Debug::Log::Log() :
	m_externalLogger(nullptr),
	m_cout()
{
}