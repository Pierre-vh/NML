#include "error.h"

using namespace Easy;

error::error()
{
}

void error::setStream(std::ostream * s)
{
	stream = s;
}

void Easy::error::reportError(const std::string & s)
{
	triggerError();
	*stream << "[ERROR]" << s.c_str();
}

void Easy::error::reportError(const char * file, const int & line, const ERROR & err, const std::string & s)
{
	triggerError();
	if (names.find(err) == names.end())
		*stream << "[ERROR][FILE:" << file << " LINE:" << line << "]" << s.c_str() << std::endl;

	if (!options.muteErrors)
		*stream << "[ERROR][FILE:" << file << " LINE:" << line << "]\n[" << names[err].first.c_str() << "][" << names[err].second.c_str() << "]\n" << s.c_str() << std::endl;
}

void Easy::error::reportError(const ERROR & err, const std::string & s)
{
	triggerError();
	if (names.find(err) == names.end())
		*stream << "[ERROR]" << s.c_str() << std::endl;

	if (!options.muteErrors)
		*stream << "[ERROR]\n[" << names[err].first.c_str() << "][" << names[err].second.c_str() << "]\n" << s.c_str() << std::endl;
}

void Easy::error::reportWarning(const std::string & w)
{
	if(options.Warning_Are_Errors)
		triggerError();

	if (!options.muteWarnings)
		*stream << "[WARNING]" << w.c_str() << std::endl;
}

void Easy::error::sayInformation(const std::string & str)
{
	(*stream) << str.c_str();
}

bool Easy::error::hasReportedWarnings() const
{
	return warnOcc;
}

bool Easy::error::isOk() const
{
	return !errOcc;
}
error::~error()
{
}

void Easy::error::triggerError()
{
	errOcc = true;
}
