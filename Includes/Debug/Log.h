#pragma once

#include <string>
#include <atomic>
#include <sstream>

#include "define.h"

#pragma warning( disable : 4005 )
#define R3DE_CURRENT_FILE __FILE__

#ifdef _EDITOR
#define R3DE_DEBUG(text, ...) Debug::Log::Debug(R3DE_CURRENT_FILE, __LINE__, text, __VA_ARGS__)
#define R3DE_INFO(text, ...) Debug::Log::Info(R3DE_CURRENT_FILE, __LINE__, text, __VA_ARGS__) 
#define R3DE_WARNING(text, ...) Debug::Log::Warning(R3DE_CURRENT_FILE, __LINE__, text, __VA_ARGS__)
#define R3DE_ERROR(text, ...) Debug::Log::Error(R3DE_CURRENT_FILE, __LINE__, text, __VA_ARGS__) 
#define R3DE_FATAL(text, ...) Debug::Log::Fatal(R3DE_CURRENT_FILE, __LINE__, text, __VA_ARGS__) 
#else // !_EDITOR
#define R3DE_DEBUG(text, ...) Debug::Log::Debug(text, __VA_ARGS__)
#define R3DE_INFO(text, ...) Debug::Log::Info(text, __VA_ARGS__) 
#define R3DE_WARNING(text, ...) Debug::Log::Warning(text, __VA_ARGS__)
#define R3DE_ERROR(text, ...) Debug::Log::Error(text, __VA_ARGS__)
#define R3DE_FATAL(text, ...) Debug::Log::Fatal(text, __VA_ARGS__)
#endif

namespace Debug
{
	enum class LOG_LEVEL : unsigned int
	{
		DEBUG = 0u,
		INFO,
		WARNING,
		ERROR,
		FATAL
	};

	class ILogger
	{
	public:
		virtual void Print(const std::string& p_str) = 0;
		virtual void Clear() = 0;
	};

	class Log
	{
	public:
		Log();
		~Log() {}

		static void SetInstance(Log* p_singleton) { m_singleton = p_singleton; }

#ifdef _EDITOR
		// safe for multithreading
		static void Debug(const char* p_file, const unsigned p_line, const char* p_fmt, ...);
		// safe for multithreading
		static void Info(const char* p_file, const unsigned p_line, const char* p_fmt, ...);
		// safe for multithreading
		static void Warning(const char* p_file, const unsigned p_line, const char* p_fmt, ...);
		// safe for multithreading
		static void Error(const char* p_file, const unsigned p_line, const char* p_fmt, ...);
		// safe for multithreading
		static void Fatal(const char* p_file, const unsigned p_line, const char* p_fmt, ...);

		void ShowEditorControl();
#else // !_EDITOR
		// safe for multithreading
		static void Debug(const char* p_fmt, ...);
		// safe for multithreading
		static void Info(const char* p_fmt, ...);
		// safe for multithreading
		static void Warning(const char* p_fmt, ...);
		// safe for multithreading
		static void Error(const char* p_fmt, ...);
		// safe for multithreading
		static void Fatal(const char* p_fmt, ...);
#endif

		void Clear();

		void SetLogger(ILogger* p_logger = nullptr) { m_externalLogger = p_logger; }
		void SetShowLevel(const LOG_LEVEL level, bool p_show = true) { LOG_LEVEL_show[(unsigned int)level] = p_show; }

	private:
		static Log* m_singleton;

		void Print(const LOG_LEVEL level, const bool writeFL = false, const char* p_file = "", const unsigned p_line = 0u);

		std::stringstream m_cout;
		ILogger* m_externalLogger;
		std::atomic_flag m_writting = ATOMIC_FLAG_INIT;

		bool LOG_LEVEL_show[5] = { true, true, true, true, true };
	};
}