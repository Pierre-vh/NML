#pragma once
#include <iostream>

#include "src\Errors\error.h"
#include "src\Data\Data.h"
#include "src\Globals\Globals.h"
#include "src\Lexer\Lexer.h"
#include "src\Parser\Parser.h"
#include "src\Parser\NML_AST\NMLNode.h"

using namespace Easy;

#define REPORT if(!reporter->isOk()) std::cout << "ERROR!!!!" << __LINE__ << std::endl;

class NML
{
		public:
			NML();
			~NML();

			NMLNode* parseFile(const std::string &filepath);
			NMLNode* parseString(const std::string &str);


			operator bool() const;

			struct
			{
				bool describeProcess = false;
				bool showTokens = false;
				bool showParsingTable = false;
			} params;
	private:

		void showTitle(const std::string &str)
		{
			std::cout << "####################################" << std::endl << str << std::endl;
			std::cout << "####################################" << std::endl << std::endl;
		}

};

