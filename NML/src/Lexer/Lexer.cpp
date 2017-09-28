#include "Lexer.h"

using namespace Easy;

// TODO : Add comment support

Lexer::Lexer()
{
}


Lexer::~Lexer()
{
}

std::list<token> Lexer::lex(const std::string & file)
{
	data = file;
	if (!eat(0))
		BASE_ERROR(reporter, Easy::LEXING_FAILED, "");
	return tokens;
}

void Lexer::flush()
{
	if (curtok != "") // only flush if curtok is not empty.
	{
		info npos = curpos;
		npos.column -=  curtok.size(); // We need to count from the beginning of the word
		// Sometimes we end up with a negative column. We need to check that it did not happen.
		if (npos.column < 0)
			npos.column = 0;
		tokens.push_back(token(curtok,npos));
		curtok = "";
	}
}

bool Lexer::sanityCheck()
{
	if (inChar && inStr)
	{
		BASE_ERROR(reporter, Easy::GENERIC_ERROR, "InChar and InStr were both active at the same time.");
	}
	return false;
}

bool Lexer::isEOF(size_t cur,bool &state)
{
	if (cur == data.size()) {
		if (inChar || inStr) // Checks for unclosed delimited variables
		{
			BASE_ERROR(reporter,Easy::LEXING_ERROR_DELIMITOR,"");
			state = false;
		}
		state = true;
		return true;
	}
	return false;
}


void Lexer::manageDelimiters(const char & c)
{
	if (c == cdel && !inStr)
		inChar != inChar;
	else if (c == sdel && !inChar)
		inStr != inStr;
}


bool Lexer::eat(size_t pos)
{
	// Sanity check. Should never be triggered.
	if (sanityCheck())
		return false;
	// Checking for EOF
	bool state;
	if (isEOF(pos,state))
	{
		return state; // Lexing succeeds otherwise.
	}
	// lexing process
	// Easy access for the current char
	char c = data[pos];
	// Data to keep track of every token's position.
	if (lastpos != pos) // Check needed to not count the same column twice
	{
		if (c == '\n') 
			{ curpos.column = 0; curpos.line += 1; }
		else 
			curpos.column += 1;


	}
	lastpos = pos;

	// isspace
	bool space = std::isspace(c,loc);
	// If we're inside a char or a string, just blindly add to the curtok and go on.
	if (inChar || inStr)
	{
		curtok += c;
		return eat(pos+1);
	}
	else
	{
		if (isDelimiter(c) && (data[pos - 1] != '\\')) // The first part checks if c is a delimiter, the second partchecks for an espace character situation.
		{
			if (curtok != "" && (inStr || inChar)) // It is treated as a separator, so do the normal procedure.
			{
				flush();
				return eat(pos);
			}
			manageDelimiters(c);

			curtok += c;
			if (inStr || inChar) { flush(); }
			return eat(pos + 1);
		}
		else if (isSep(c) || space)
		{
			if (curtok == "") // The current token is empty : flush the separator individually
			{
				if (!space) curtok = c; // Unless the c is a space, flush it
				flush();
				return eat(pos + 1);
			}
			else // Flushes the curtok then just re-run the function 
			{
				flush();
				return eat(pos);
			}
		}
		else // normal behaviour
		{
			curtok += c;
			return eat(pos + 1);
		}
	}
	return false;
}

inline bool Lexer::isSep(const char &x) const
{
	return (separators.find(x) != std::string::npos);
}
inline bool  Lexer::isDelimiter(const char & c) const
{
	return ((c == cdel) || (c == sdel));
}
