#include "consoleappender.h"
#include "factoryparams.h"
#include  "zx_utils.h"

ConsoleAppender::ConsoleAppender(const std::string& name)
	:Appender(name)
{

}

ConsoleAppender::~ConsoleAppender()
{

}

bool ConsoleAppender::reopen()
{
	return true;
}

void ConsoleAppender::close()
{

}

#ifdef _WIN32
bool ConsoleAppender::SetConsoleColor(WORD wAttributes)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
		return FALSE;

	return SetConsoleTextAttribute(hConsole, wAttributes);
}


const WORD *ConsoleAppender::colors() {
	static const WORD output_colors[10] = {
		FOREGROUND_RED | FOREGROUND_INTENSITY ,
		FOREGROUND_RED | FOREGROUND_INTENSITY ,
		FOREGROUND_RED|FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	};
	return output_colors;
}

WORD ConsoleAppender::getPriorityColor(int priority) throw() {
	return colors()[((priority < 0) || (priority > 4)) ? 5 : priority];
}
#endif

void ConsoleAppender::doAppend(severity_level level, const char *file, int line, const std::string& message)
{
	std::ostringstream message_stream;

	std::string time_RFC3339;
	time_RFC3339 = ZxBase::now2str();
	const std::string& priorityName = getPriorityName((int)level);
	message_stream << "[" << time_RFC3339 << "] ";
	if (file) {
		message_stream << "[" << file << ":" << line  << "] ";
	}
	message_stream << priorityName << " " << message << std::endl;
	
#ifdef _WIN32
	if (level < LEVEL_INFO) {
		SetConsoleColor(getPriorityColor(level));
	}
#endif
	
	printf("%s", message_stream.str().c_str());
	
#ifdef _WIN32
	if (level < LEVEL_INFO) {
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
#endif
	
}

std::auto_ptr<Appender> create_console_appender(const FactoryParams& params)
{
	std::string name;
	params.get_for("roll file appender").required("name", name);
	return std::auto_ptr<Appender>(new ConsoleAppender(name));
}