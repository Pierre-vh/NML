#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include <list>
#include <string>
#include <regex>
#include <locale>
#include <algorithm>

#include "../Errors/error.h"
#include "../Globals/Globals.h"

#define IDREGEX "^[a-zA-Z_$][a-zA-Z_$0-9]*$"

namespace Easy
{
	struct info
	{
		info(){}
		int column;
		int line;
	};
	enum tokentype
	{
		LESS_THAN,		// <
		GREATER_THAN,	// >
		SQB_OPEN,		// [
		SQB_CLOSE,		// ]
		SLASH,
		EQUAL,			// =
		id,				// id
		data,			// Any data (numbers) and some reserved ids : true,false,null
		err,			// used as a default when errors occured. Should never be used in 
		unknown
	};
	struct token // holds the data.
	{
		token() {}
		token(const std::string &vstr) : str(vstr)
		{
			this->type = this->detectType();	// ici j'utilise la notation this->type afin d'éviter la confusion avec le 'type' utilisé dans la fn statique.
		}
		token(const std::string &vstr,const info &i) : str(vstr),pos(i)
		{
			this->type = this->detectType();	// ici j'utilise la notation this->type afin d'éviter la confusion avec le 'type' utilisé dans la fn statique.
		}
		std::string str;
		tokentype type;
		info pos;

		std::string getTypeAsString()
		{
			return token::getTypeAsString(this->type);
		}

		static std::string getTypeAsString(const tokentype &type)
		{
			switch (type)
			{
				case LESS_THAN:
					return "LESS_THAN";
				case GREATER_THAN:
					return "GREATER_THAN Symbol";
				case SQB_OPEN:
					return "SQUARE_BRACKET_OPEN Symbol";
				case SQB_CLOSE:
					return "SQUARE_BRACKET_CLOSE Symbol";
				case EQUAL:
					return "EQUAL Symbol";
				case SLASH:
					return "SLASH Symbol";
				case id:
					return "id";
				case data:
					return "data";
				case unknown:
					return "unknown token";
				default:
					BASE_ERROR(reporter, Easy::GENERIC_ERROR, "Defaulted.");
					return "";
			}
		}
		static std::string getTypeFriendlyRepresentation(const tokentype &type) 
		{
			switch (type)
			{
				case LESS_THAN:
					return "<";
				case GREATER_THAN:
					return ">";
				case SQB_OPEN:
					return "[";
				case SQB_CLOSE:
					return "]";
				case EQUAL:
					return "=";
				case SLASH:
					return "/";
				case id:
					return "id";
				case data:
					return "data";
				default:
					BASE_ERROR(reporter, Easy::GENERIC_ERROR, "Defaulted.");
					return "";
			}
		}

		tokentype detectType()
		{
			if (str.size() == 0)
				BASE_ERROR(reporter, Easy::GENERIC_ERROR, "Cannot initialize an empty token.");
			else if (str.size() == 1)
			{
				switch (str[0])
				{
					case '<':
						return LESS_THAN;
					case '>':
						return GREATER_THAN;
					case '[':
						return SQB_OPEN;
					case ']':
						return SQB_CLOSE;
					case '/':
						return SLASH;
					case '=':
						return EQUAL;
					default:
						if (isdigit(str[0]))
							return data;
						return unknown;
						break;
				}
			}
			else
			{
				if (std::regex_match(str, std::regex(IDREGEX))) // This is an ID
				{
					std::string low_str = str;
					std::transform(low_str.begin(), low_str.end(), low_str.begin(), ::tolower);
					if (low_str == "null" || low_str == "true" || low_str == "false")
						return data;
					else
						return id;
				}
				else
					return data;
			}
			return err;
		}
	};

	class Lexer
	{
	public:
		Lexer();
		~Lexer();

		std::list<token> lex(const std::string &file);


	private:
		// func
		bool eat(const size_t &pos);
		void flush(); // Pushes the current token on the array and resets curtok
		// utility
		bool isEOF(const size_t &cur);
		void manageDelimiters(const char &c); // Flips a bool if the current char is a char/str delimiter
		inline bool isSep(const char &x) const;
		inline bool isDelimiter(const char &c) const;
		// var
		std::list<token> tokens; // All the current tokens
		std::string curtok = ""; // The current token being lexed

		std::string data = ""; // The data to lex

		// Variables used to manipulate the lexing process
		std::string separators = "<>[]/="; // + a space character, treated differently
		char cdel = '\'', sdel = '"';

		// Variable used by the lexing process to keep track of stuff
		bool inStr = false,inChar = false,comment = false;

		info curpos;
		size_t lastpos = 0;
		// isspace
		std::locale loc;
	};

}