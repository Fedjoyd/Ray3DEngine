#pragma once

#include <string>
#include <vector>
#include <atomic>
#include <sstream>

#include "define.h"

#pragma warning( disable : 4005 )
#define R3DE_CURRENT_FILE __FILE__

#if defined (_DEBUG) || defined (_DEBUG_LOG)
#define R3DE_DEBUG(text, ...) Debug::Log::PrintLog(Debug::LOG_LEVEL::DEBUG, R3DE_CURRENT_FILE, __LINE__, text, __VA_ARGS__)
#define R3DE_INFO(text, ...) Debug::Log::PrintLog(Debug::LOG_LEVEL::INFO, R3DE_CURRENT_FILE, __LINE__, text, __VA_ARGS__) 
#define R3DE_WARNING(text, ...) Debug::Log::PrintLog(Debug::LOG_LEVEL::WARNING, R3DE_CURRENT_FILE, __LINE__, text, __VA_ARGS__)
#define R3DE_ERROR(text, ...) Debug::Log::PrintLog(Debug::LOG_LEVEL::mERROR, R3DE_CURRENT_FILE, __LINE__, text, __VA_ARGS__) 
#define R3DE_FATAL(text, ...) Debug::Log::PrintLog(Debug::LOG_LEVEL::FATAL, R3DE_CURRENT_FILE, __LINE__, text, __VA_ARGS__) 
#else // !_DEBUG
#define R3DE_DEBUG(text, ...) Debug::Log::PrintLog(Debug::LOG_LEVEL::DEBUG, text, __VA_ARGS__)
#define R3DE_INFO(text, ...) Debug::Log::PrintLog(Debug::LOG_LEVEL::INFO, text, __VA_ARGS__) 
#define R3DE_WARNING(text, ...) Debug::Log::PrintLog(Debug::LOG_LEVEL::WARNING, text, __VA_ARGS__)
#define R3DE_ERROR(text, ...) Debug::Log::PrintLog(Debug::LOG_LEVEL::mERROR, text, __VA_ARGS__)
#define R3DE_FATAL(text, ...) Debug::Log::PrintLog(Debug::LOG_LEVEL::FATAL, text, __VA_ARGS__)
#endif

namespace Debug
{
	class ILogger
	{
	public:
		virtual void Print(const std::string& p_str) = 0;
		virtual void Clear() = 0;
	};

#ifdef _EDITOR
	struct LogItem
	{
		LOG_LEVEL Level;
		std::string FileLine;
		std::string Text;
	};
#endif // _EDITOR

	class Log
	{
	public:
		Log();
		~Log() {}

		static void SetInstance(Log* p_singleton) { m_singleton = p_singleton; }

#if defined (_DEBUG) || defined (_DEBUG_LOG)
		// safe for multithreading
		static void PrintLog(const LOG_LEVEL p_level, const char* p_file, const unsigned p_line, const char* p_fmt, ...);
#else // _DEBUG
		// safe for multithreading
		static void PrintLog(const LOG_LEVEL p_level, const char* p_fmt, ...);
#endif // !_DEBUG

#ifdef _EDITOR
		void ShowEditorControl();
#endif // _Editor

		void Clear();

		void SetLogger(ILogger* p_logger = nullptr) { m_externalLogger = p_logger; }
		void SetShowLevel(const LOG_LEVEL level, bool p_show = true) { LOG_LEVEL_show[(unsigned int)level] = p_show; }

	private:
		static Log* m_singleton;

		void Print(const LOG_LEVEL level, const bool writeFL = false, const char* p_file = "", const unsigned p_line = 0u);

		std::stringstream m_cout;
		ILogger* m_externalLogger;
#ifdef _EDITOR
		std::vector<LogItem> m_logList;
		bool m_scrollToBottom = true;
#endif // _EDITOR
		std::atomic_flag m_writting = ATOMIC_FLAG_INIT;

		bool LOG_LEVEL_show[5] = { true, true, true, true, true };
	};
}