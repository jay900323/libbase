#ifdef _WIN32
#include "MSThreads.h"

namespace threading {

	std::string getThreadId() {
		char buffer[16];
		sprintf(buffer, "%lu", GetCurrentThreadId());
		return std::string(buffer);
	};
}

#endif