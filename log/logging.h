#include <sstream>

namespace logging {
	typedef wchar_t PathChar;
	enum LoggingDestination {
		LOG_NONE                = 0,
		LOG_TO_FILE             = 1 << 0,
		LOG_TO_SYSTEM_DEBUG_LOG = 1 << 1,

		LOG_TO_ALL = LOG_TO_FILE | LOG_TO_SYSTEM_DEBUG_LOG,

		LOG_DEFAULT = LOG_TO_FILE,
	};

	enum LogLockingState { LOCK_LOG_FILE, DONT_LOCK_LOG_FILE };

	// On startup, should we delete or append to an existing log file (if any)?
	// Defaults to APPEND_TO_OLD_LOG_FILE.
	enum OldFileDeletionState { DELETE_OLD_LOG_FILE, APPEND_TO_OLD_LOG_FILE };

	struct LoggingSettings {
		// The defaults values are:
		//
		//  logging_dest: LOG_DEFAULT
		//  log_file:     NULL
		//  lock_log:     LOCK_LOG_FILE
		//  delete_old:   APPEND_TO_OLD_LOG_FILE
		LoggingSettings();

		LoggingDestination logging_dest;

		// The three settings below have an effect only when LOG_TO_FILE is
		// set in |logging_dest|.
		const PathChar* log_file;
		LogLockingState lock_log;
		OldFileDeletionState delete_old;
	};


#if NDEBUG
#define BaseInitLoggingImpl BaseInitLoggingImpl_built_with_NDEBUG
#else
#define BaseInitLoggingImpl BaseInitLoggingImpl_built_without_NDEBUG
#endif


	bool BaseInitLoggingImpl(const LoggingSettings& settings);

	inline bool InitLogging(const LoggingSettings& settings) {
		return BaseInitLoggingImpl(settings);
	}

	void SetMinLogLevel(int level);

	// Gets the current log level.
	int GetMinLogLevel();

	// Gets the VLOG default verbosity level.
	int GetVlogVerbosity();

	// Gets the current vlog level for the given file (usually taken from
	// __FILE__).

	// Note that |N| is the size *with* the null terminator.
	int GetVlogLevelHelper(const char* file_start, size_t N);

	template <size_t N>
	int GetVlogLevel(const char (&file)[N]) {
		return GetVlogLevelHelper(file, N);
	}

	void SetLogItems(bool enable_process_id, bool enable_thread_id,
		bool enable_timestamp, bool enable_tickcount);

	typedef void (*LogAssertHandlerFunction)(const std::string& str);
	void SetLogAssertHandler(LogAssertHandlerFunction handler);

	typedef bool (*LogMessageHandlerFunction)(int severity,
		const char* file, int line, size_t message_start, const std::string& str);
	BASE_EXPORT void SetLogMessageHandler(LogMessageHandlerFunction handler);
	BASE_EXPORT LogMessageHandlerFunction GetLogMessageHandler();

	typedef int LogSeverity;
	const LogSeverity LOG_VERBOSE = -1;  // This is level 1 verbosity
	// Note: the log severities are used to index into the array of names,
	// see log_severity_names.
	const LogSeverity LOG_INFO = 0;
	const LogSeverity LOG_WARNING = 1;
	const LogSeverity LOG_ERROR = 2;
	const LogSeverity LOG_FATAL = 3;
	const LogSeverity LOG_NUM_SEVERITIES = 4;

#ifdef NDEBUG
	const LogSeverity LOG_DFATAL = LOG_ERROR;
#else
	const LogSeverity LOG_DFATAL = LOG_FATAL;
#endif

#define COMPACT_GOOGLE_LOG_EX_INFO(ClassName, ...) \
	logging::ClassName(__FILE__, __LINE__, logging::LOG_INFO , ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_EX_WARNING(ClassName, ...) \
	logging::ClassName(__FILE__, __LINE__, logging::LOG_WARNING , ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_EX_ERROR(ClassName, ...) \
	logging::ClassName(__FILE__, __LINE__, logging::LOG_ERROR , ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_EX_FATAL(ClassName, ...) \
	logging::ClassName(__FILE__, __LINE__, logging::LOG_FATAL , ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_EX_DFATAL(ClassName, ...) \
	logging::ClassName(__FILE__, __LINE__, logging::LOG_DFATAL , ##__VA_ARGS__)

#define COMPACT_GOOGLE_LOG_INFO \
	COMPACT_GOOGLE_LOG_EX_INFO(LogMessage)
#define COMPACT_GOOGLE_LOG_WARNING \
	COMPACT_GOOGLE_LOG_EX_WARNING(LogMessage)
#define COMPACT_GOOGLE_LOG_ERROR \
	COMPACT_GOOGLE_LOG_EX_ERROR(LogMessage)
#define COMPACT_GOOGLE_LOG_FATAL \
	COMPACT_GOOGLE_LOG_EX_FATAL(LogMessage)
#define COMPACT_GOOGLE_LOG_DFATAL \
	COMPACT_GOOGLE_LOG_EX_DFATAL(LogMessage)


#define ERROR 0
#define COMPACT_GOOGLE_LOG_EX_0(ClassName, ...) \
	COMPACT_GOOGLE_LOG_EX_ERROR(ClassName , ##__VA_ARGS__)
#define COMPACT_GOOGLE_LOG_0 COMPACT_GOOGLE_LOG_ERROR
	// Needed for LOG_IS_ON(ERROR).
	const LogSeverity LOG_0 = LOG_ERROR;

#define LOG_IS_ON(severity) \
	((::logging::LOG_ ## severity) >= ::logging::GetMinLogLevel())

#define LAZY_STREAM(stream, condition)                                  \
	!(condition) ? (void) 0 : ::logging::LogMessageVoidify() & (stream)


#define LOG_STREAM(severity) COMPACT_GOOGLE_LOG_ ## severity.stream()

#define LOG(severity) LAZY_STREAM(LOG_STREAM(severity), LOG_IS_ON(severity))
#define LOG_IF(severity, condition) \
	LAZY_STREAM(LOG_STREAM(severity), LOG_IS_ON(severity) && (condition))


#define LOG_ASSERT(condition)  \
	LOG_IF(FATAL, !(condition)) << "Assert failed: " #condition ". "
#define SYSLOG_ASSERT(condition) \
	SYSLOG_IF(FATAL, !(condition)) << "Assert failed: " #condition ". "

#if defined(NDEBUG)
#define ENABLE_DLOG 0
#else
#define ENABLE_DLOG 1
#endif

#if ENABLE_DLOG

#define DLOG_IS_ON(severity) LOG_IS_ON(severity)
#define DLOG_IF(severity, condition) LOG_IF(severity, condition)
#define DLOG_ASSERT(condition) LOG_ASSERT(condition)
#define DPLOG_IF(severity, condition) PLOG_IF(severity, condition)

#else  // ENABLE_DLOG

#define DLOG_IS_ON(severity) false
#define DLOG_IF(severity, condition) EAT_STREAM_PARAMETERS
#define DLOG_ASSERT(condition) EAT_STREAM_PARAMETERS
#define DPLOG_IF(severity, condition) EAT_STREAM_PARAMETERS

#endif

enum { DEBUG_MODE = ENABLE_DLOG };

#undef ENABLE_DLOG

#define DLOG(severity)                                          \
	LAZY_STREAM(LOG_STREAM(severity), DLOG_IS_ON(severity))

#define DPLOG(severity)                                         \
	LAZY_STREAM(PLOG_STREAM(severity), DLOG_IS_ON(severity))


	// Redefine the standard assert to use our nice log files
#undef assert
#define assert(x) DLOG_ASSERT(x)



	class LogMessage {
	public:
		// Used for LOG(severity).
		LogMessage(const char* file, int line, LogSeverity severity);

		LogMessage(const char* file, int line, std::string* result);

		// Used for DCHECK_EQ(), etc. Takes ownership of the given string.
		LogMessage(const char* file, int line, LogSeverity severity,
			std::string* result);

		~LogMessage();

		std::ostream& stream() { return stream_; }

	private:
		void Init(const char* file, int line);

		LogSeverity severity_;
		std::ostringstream stream_;
		size_t message_start_;  // Offset of the start of the message (past prefix
		// info).
		// The file and line information passed in to the constructor.
		const char* file_;
		const int line_;


		class SaveLastError {
		public:
			SaveLastError();
			~SaveLastError();

			unsigned long get_error() const { return last_error_; }

		protected:
			unsigned long last_error_;
		};

		SaveLastError last_error_;

		//  DISALLOW_COPY_AND_ASSIGN(LogMessage);
	};

	inline void LogAtLevel(int const log_level, std::string const &msg) {
		LogMessage(__FILE__, __LINE__, log_level).stream() << msg;
	}

	// This class is used to explicitly ignore values in the conditional
	// logging macros.  This avoids compiler warnings like "value computed
	// is not used" and "statement has no effect".
	class LogMessageVoidify {
	public:
		LogMessageVoidify() { }
		// This has to be an operator with a precedence lower than << but
		// higher than ?:
		void operator&(std::ostream&) { }
	};

	void CloseLogFile();
}