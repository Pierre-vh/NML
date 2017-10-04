#pragma once
#include <iostream>

#include "src\Errors\error.h"
#include "src\Data\Data.h"
#include "src\Globals\Globals.h"
#include "src\Lexer\Lexer.h"
#include "src\Parser\Parser.h"
#include "src\Parser\NML_AST\NMLNode.h"

using namespace Easy;

// This is a 'wrapper' class that loads 'standard' NML files. You'll have to make your own if you decide to implement another syntax/tree structure. 
// You'll also need to make another Node class, inherited from the Node class (see the Parser folder for that)

#define REPORT if(!reporter->isOk()) std::cout << "ERROR!!!!" << __LINE__ << std::endl;
#define RETURN_IF_ERROR if(!reporter->isOk()) return nullptr;

// This is a 'wrapper' class that loads 'standard' NML files. You'll have to make your own if you decide to implement another syntax/tree structure. 
// You'll also need to make another Node class, inherited from the Node class (see the Parser folder for that)

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
				bool showFileStats = false;
			} params;
	private:

		void showTitle(const std::string &str)
		{
			std::cout << "####################################" << std::endl << str << std::endl;
			std::cout << "####################################" << std::endl << std::endl;
		}

};

