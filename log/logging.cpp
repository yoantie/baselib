#include "stdafx.h"
#include "logging.h"
#include <io.h>
#include <windows.h>
typedef HANDLE FileHandle;
typedef HANDLE MutexHandle;
// Windows warns on using write().  It prefers _write().
#define write(fd, buf, count) _write(fd, buf, static_cast<unsigned int>(count))
// Windows doesn't define STDERR_FILENO.  Define it here.
#define STDERR_FILENO 2


#include <algorithm>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <ostream>
#include <string>

#include "macros.h"
#include "string.h"
#include "debugger.h"

namespace logging {

	namespace {

		const char* const log_severity_names[LOG_NUM_SEVERITIES] = {
			"INFO", "WARNING", "ERROR", "FATAL" };

			const char* log_severity_name(int severity)
			{
				if (severity >= 0 && severity < LOG_NUM_SEVERITIES)
					return log_severity_names[severity];
				return "UNKNOWN";
			}


			int min_log_level = 0;

			LoggingDestination logging_destination = LOG_DEFAULT;

			// For LOG_ERROR and above, always print to stderr.
			const int kAlwaysPrintErrorLevel = LOG_ERROR;

			typedef std::wstring PathString;

			PathString* log_file_name = NULL;



			// this file is lazily opened and the handle may be NULL
			FileHandle log_file = NULL;

			// what should be prepended to each message?
			bool log_process_id = false;
			bool log_thread_id = false;
			bool log_timestamp = true;
			bool log_tickcount = false;

			// Should we pop up fatal debug messages in a dialog?
			bool show_error_dialogs = false;

			// An assert handler override specified by the client to be called instead of
			// the debug message dialog and process termination.
			LogAssertHandlerFunction log_assert_handler = NULL;
			// A log message handler that gets notified of every log message we process.
			LogMessageHandlerFunction log_message_handler = NULL;

			__int64 TickCount() {
				return GetTickCount();
			}



			void DeleteFilePath(const PathString& log_name) {
				DeleteFile(log_name.c_str());
			}


			PathString GetDefaultLogFile() {
				// On Windows we use the same path as the exe.
				wchar_t module_name[MAX_PATH];
				GetModuleFileName(NULL, module_name, MAX_PATH);

				PathString log_file = module_name;
				PathString::size_type last_backslash =
					log_file.rfind('\\', log_file.size());
				if (last_backslash != PathString::npos)
					log_file.erase(last_backslash + 1);
				log_file += L"debug.log";
				return log_file;
			}

			class CCSec {
				CRITICAL_SECTION m_cs;

			public:
				CCSec() {
					::InitializeCriticalSectionAndSpinCount(&m_cs, 2000);
				}

				~CCSec() {
					::DeleteCriticalSection(&m_cs);
				}

				void Lock() {
					::EnterCriticalSection(&m_cs);
				}

				void Unlock() {
					::LeaveCriticalSection(&m_cs);
				}

				BOOL TryLock(){
					return ::TryEnterCriticalSection(&m_cs);
				}
			};

			class LoggingLock {
			public:
				LoggingLock() {
					LockLogging();
				}

				~LoggingLock() {
					UnlockLogging();
				}

				static void Init(LogLockingState lock_log, const PathChar* new_log_file) {
					if (initialized)
						return;
					lock_log_file = lock_log;
					if (lock_log_file == LOCK_LOG_FILE) {
						if (!log_mutex) {
							std::wstring safe_name;
							if (new_log_file)
								safe_name = new_log_file;
							else
								safe_name = GetDefaultLogFile();
							// \ is not a legal character in mutex names so we replace \ with /
							std::replace(safe_name.begin(), safe_name.end(), '\\', '/');
							std::wstring t(L"Global\\");
							t.append(safe_name);
							log_mutex = ::CreateMutex(NULL, FALSE, t.c_str());

							if (log_mutex == NULL) {
#if _DEBUG
								// Keep the error code for debugging
								int error = GetLastError();  // NOLINT
								BreakDebugger();
#endif
								// Return nicely without putting initialized to true.
								return;
							}
						}
					} else {
						log_lock = new CCSec;
					}
					initialized = true;
				}

			private:
				static void LockLogging() {
					if (lock_log_file == LOCK_LOG_FILE) {
						::WaitForSingleObject(log_mutex, INFINITE);
					} else {
						log_lock->Lock();
					}
				}

				static void UnlockLogging() {
					if (lock_log_file == LOCK_LOG_FILE) {
						ReleaseMutex(log_mutex);
					} else {
						log_lock->Unlock();
					}
				}

				static CCSec* log_lock;
				static MutexHandle log_mutex; //可用于全局，进程间文件共享

				static bool initialized;
				static LogLockingState lock_log_file;
			};


			// static
			bool LoggingLock::initialized = false;
			// static
			CCSec* LoggingLock::log_lock = NULL;
			// static
			LogLockingState LoggingLock::lock_log_file = LOCK_LOG_FILE;


			MutexHandle LoggingLock::log_mutex = NULL;

			bool InitializeLogFileHandle() {
				if (log_file)
					return true;

				if (!log_file_name) {
					log_file_name = new PathString(GetDefaultLogFile());
				}

				if ((logging_destination & LOG_TO_FILE) != 0) {
					log_file = CreateFile(log_file_name->c_str(), GENERIC_WRITE,
						FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
						OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					if (log_file == INVALID_HANDLE_VALUE || log_file == NULL) {
						// try the current directory
						log_file = CreateFile(L".\\debug.log", GENERIC_WRITE,
							FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
							OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						if (log_file == INVALID_HANDLE_VALUE || log_file == NULL) {
							log_file = NULL;
							return false;
						}
					}
					SetFilePointer(log_file, 0, 0, FILE_END);
				}

				return true;
			}

			void CloseFile(FileHandle log) {
				CloseHandle(log);
			}

			void CloseLogFileUnlocked() {
				if (!log_file)
					return;

				CloseFile(log_file);
				log_file = NULL;
			}

	} // namespace

	LoggingSettings::LoggingSettings()
		: logging_dest(LOG_DEFAULT),
		log_file(NULL),
		lock_log(LOCK_LOG_FILE),
		delete_old(APPEND_TO_OLD_LOG_FILE) {}

	bool BaseInitLoggingImpl(const LoggingSettings& settings) {
		logging_destination = settings.logging_dest;

		// ignore file options unless logging to file is set.
		if ((logging_destination & LOG_TO_FILE) == 0)
			return true;

		LoggingLock::Init(settings.lock_log, settings.log_file);
		LoggingLock logging_lock;

		// Calling InitLogging twice or after some log call has already opened the
		// default log file will re-initialize to the new options.
		CloseLogFileUnlocked();

		if (!log_file_name)
			log_file_name = new PathString();
		*log_file_name = settings.log_file;
		if (settings.delete_old == DELETE_OLD_LOG_FILE)
			DeleteFilePath(*log_file_name);

		return InitializeLogFileHandle();

	}



	void SetMinLogLevel(int level) {
		min_log_level = min(LOG_FATAL, level);
	}

	int GetMinLogLevel() {
		return min_log_level;
	}

	void SetLogItems(bool enable_process_id, bool enable_thread_id,
		bool enable_timestamp, bool enable_tickcount) {
			log_process_id = enable_process_id;
			log_thread_id = enable_thread_id;
			log_timestamp = enable_timestamp;
			log_tickcount = enable_tickcount;
	}

	void SetLogAssertHandler(LogAssertHandlerFunction handler) {
		log_assert_handler = handler;
	}

	void SetLogMessageHandler(LogMessageHandlerFunction handler) {
		log_message_handler = handler;
	}

	LogMessageHandlerFunction GetLogMessageHandler() {
		return log_message_handler;
	}


	LogMessage::SaveLastError::SaveLastError() : last_error_(::GetLastError()) {
	}

	LogMessage::SaveLastError::~SaveLastError() {
		::SetLastError(last_error_);
	}



	LogMessage::LogMessage(const char* file, int line, LogSeverity severity)
		: severity_(severity), file_(file), line_(line) {
			Init(file, line);
	}

	LogMessage::LogMessage(const char* file, int line, std::string* result)
		: severity_(LOG_FATAL), file_(file), line_(line) {
			Init(file, line);
			stream_ << "Check failed: " << *result;
			delete result;
	}

	LogMessage::LogMessage(const char* file, int line, LogSeverity severity,
		std::string* result)
		: severity_(severity), file_(file), line_(line) {
			Init(file, line);
			stream_ << "Check failed: " << *result;
			delete result;
	}


	LogMessage::~LogMessage() {
		stream_ << std::endl;
		std::string str_newline(stream_.str());

		// Give any log message handler first dibs on the message.
		if (log_message_handler &&
			log_message_handler(severity_, file_, line_,
			message_start_, str_newline)) {
				// The handler took care of it, no further processing.
				return;
		}


		if ((logging_destination & LOG_TO_SYSTEM_DEBUG_LOG) != 0) {
			OutputDebugStringA(str_newline.c_str());
			fwrite(str_newline.data(), str_newline.size(), 1, stderr);
			fflush(stderr);
		} else if (severity_ >= kAlwaysPrintErrorLevel) {
			fwrite(str_newline.data(), str_newline.size(), 1, stderr);
			fflush(stderr);
		}

		if ((logging_destination & LOG_TO_FILE) != 0) {
			LoggingLock::Init(LOCK_LOG_FILE, NULL);
			LoggingLock logging_lock;
			if (InitializeLogFileHandle()) {
				SetFilePointer(log_file, 0, 0, SEEK_END);
				DWORD num_written;
				WriteFile(log_file,
					static_cast<const void*>(str_newline.c_str()),
					static_cast<DWORD>(str_newline.length()),
					&num_written,
					NULL);
			}
		}


		if (severity_ == LOG_FATAL) {
			// Ensure the first characters of the string are on the stack so they
			// are contained in minidumps for diagnostic purposes.
			char str_stack[1024];
			str_newline.copy(str_stack, arraysize(str_stack));
			//    base::debug::Alias(str_stack);

			if (log_assert_handler) {
				log_assert_handler(std::string(stream_.str()));
			} else {
				// Don't use the string with the newline, get a fresh version to send to
				// the debug message process. We also don't display assertions to the
				// user in release mode. The enduser can't do anything with this
				// information, and displaying message boxes when the application is
				// hosed can cause additional problems.
#ifndef NDEBUG
				//  DisplayDebugMessageInDialog(stream_.str());
#endif
				// Crash the process to generate a dump.
				BreakDebugger();
			}
		}

	}


	// writes the common header info to the stream
	void LogMessage::Init(const char* file, int line) {
		std::string filename(file);
		size_t last_slash_pos = filename.find_last_of("\\/");
		if (last_slash_pos != std::string::npos)
			filename = filename.substr(last_slash_pos + 1);

		stream_ <<  '[';
		stream_ << ::GetCurrentProcessId() << ':';
		stream_ << ::GetCurrentThreadId() << ':';
		if (log_timestamp) {
			time_t t = time(NULL);
			struct tm local_time = {0};
			localtime_s(&local_time, &t);
			struct tm* tm_time = &local_time;
			stream_ << std::setfill('0')
				<< std::setw(2) << 1 + tm_time->tm_mon
				<< std::setw(2) << tm_time->tm_mday
				<< '/'
				<< std::setw(2) << tm_time->tm_hour
				<< std::setw(2) << tm_time->tm_min
				<< std::setw(2) << tm_time->tm_sec
				<< ':';
		}
		if (log_tickcount)
			stream_ << TickCount() << ':';
		if (severity_ >= 0)
			stream_ << log_severity_name(severity_);
		else
			stream_ << "VERBOSE" << -severity_;

		stream_ << ":" << filename << "(" << line << ")] ";

		message_start_ = stream_.tellp();
	}


	void CloseLogFile() {
		LoggingLock logging_lock;
		CloseLogFileUnlocked();
	}


}