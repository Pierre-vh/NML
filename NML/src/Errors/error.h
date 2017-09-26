#pragma once

#include <iostream>
#include <ios>
#include <map>
// Add error here

#define BASE_ERROR(a,x,y) a->reportError(__FILE__,__LINE__,x,y) // Macro for quick & easy error reporting. Takes the instance of error as argument, the error code and a message.

namespace Easy
{

	typedef enum errtype
	{
		ENUM_UNKNOWN_DATA_TYPE = -1,
		GENERIC_ERROR = 0,
		BAD_CAST,
		WARNINGCONSTRUCTOR_EMPTYFUNCTION,
		WARNING_VOIDCALLBACK,
		UNEXPECTED_LEXING_ERROR,
		LEXING_ERROR_DELIMITOR,
		LEXING_FAILED,
		PARSING_ERROR,
		CANT_OPEN_FILE
	}ERROR;

	class error
	{
		public:
			error();
			void setStream(std::ostream *s);
			void reportError(const std::string &s);
			void reportError(const char *file, const int &line, const ERROR &err = GENERIC_ERROR, const std::string &s = "");
			void reportError(const ERROR &err = GENERIC_ERROR, const std::string &s = "");

			void reportWarning(const std::string &w);

			bool hasReportedWarnings() const;
			bool isOk() const; // Checks if has error has been thrown or not.

			~error();
			
		private:
			/*
			struct {
				ERROR errtype;
				std::string errtype_str;
				std::string err_desc;
				std::string err_msg;
			} lastError;*/
			void triggerError();

			bool errOcc = false;
			bool warnOcc = false;
			std::ostream *stream = &std::cout;
			// Add errors Name & Description here.
			std::map<ERROR, std::pair<std::string, std::string>> names =
			{
				{ ENUM_UNKNOWN_DATA_TYPE			,{ "ENUM_UNKNOWN_DATA_TYPE",			"The enum used to store the stored data's type in Data.h/.cpp was set to an unknown value. This is a bug in the Data class. Please report this with a code sample that reproduces the problem." } },
				{ GENERIC_ERROR						,{ "GENERIC_ERROR",						"Standard Error. Information:" } },
				{ BAD_CAST							,{ "BAD_CAST",							"Attempted to cast a [Data]'s content to an incompatible [Data] type" } },
				{ UNEXPECTED_LEXING_ERROR			,{ "UNEXPECTED_LEXING_ERROR",			"An unexpected error occured while lexing the input. This is probably a bug." } },
				{ LEXING_ERROR_DELIMITOR			,{ "LEXING_ERROR_DELIMITOR",			"A Char/String value was not closed properly, causing the Lexing process to be aborted." } },
				{ LEXING_FAILED						,{ "LEXING_FAILED",						"The lexing process failed." }},
				{ PARSING_ERROR						,{ "PARSING_ERROR",						"An error occured while parsing the file."}},
				{ CANT_OPEN_FILE					,{ "CANT_OPEN_FILE",					"You tried to open a file, and it wasn't successful. Check your file's path and name."}}
			};
	};

}