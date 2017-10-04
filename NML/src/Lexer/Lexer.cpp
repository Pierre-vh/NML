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

	stats.clength = file.length();
	stats.tokensfound = tokens.size();

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

bool Lexer::isEOF(const size_t &cur)
{
	if (cur >= data.size()) {
		if (inChar || inStr) // Checks for unclosed delimited variables
			BASE_ERROR(reporter,Easy::LEXING_ERROR_DELIMITOR,"");
		return true;
	}
	return false;
}


void Lexer::manageDelimiters(const char & c)
{
	if (c == cdel)
		inChar = !inChar;
	else if (c == sdel)
		inStr = !inStr;
}


bool Lexer::eat(const size_t &pos)
{
	// Checking for EOF
	if (isEOF(pos))
		return true; // Lexing succeeds otherwise.
	// lexing process
	// Easy access for the current char
	c = data[pos];
	// Same for back char
	next = (pos <= data.size()) ? data[pos + 1] : ' ';

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
	// stats
	stats.tabs += (c == '\t') ? 1 : 0;
	stats.nl += (c == '\n') ? 1 : 0;

	// comment
	if (comment)
	{
		if (c == '\n')
			comment = false;
		return eat(pos + 1);
	}
	else if (c == '/' && next == '/')
	{
		comment = true;
		return eat(pos + 1);
	}

	// If we're inside a char or a string, just blindly add to the curtok and go on.
	/////////
	// CHAR AND STR DELIMITERS
	////////
	if (isDelimiter(c)) // The first part checks if c is a delimiter, the second partchecks for an espace character situation.
	{
		if (curtok != "" && !(inStr || inChar)) // It is treated as a separator, so do the normal procedure before going further.
		{
			flush();
			return eat(pos);
		}
		else if ((data[pos - 1] == '\\')) // the character is escaped.
		{
			if (curtok.back() == '\\') curtok.pop_back(); // We don't need the inverted slash in the string as it's an escape character.
			curtok += c;
			return eat(pos + 1);
		}
		else if (c == '\'' && inStr || c == '"' && inChar) // If the delimiter doesn't concern the current opened tag, we shouldn't worry about it.
		{
			curtok += c;
			return eat(pos + 1);
		}
		else if (inStr || inChar) // If we're already in a str/char, it means that we need to close it/
		{
			curtok += c;
			manageDelimiters(c);
			flush();
			return eat(pos + 1);
		}
		else // Lastly, by deduction, it means that we're opening a char/string.
		{
			curtok += c;
			manageDelimiters(c);
			return eat(pos + 1);
		}
	}
	else if ((inChar || inStr))
	{
		curtok += c;
		return eat(pos+1);
	}
	/////////
	// SEPARATORS
	////////
	else
	{
		if (isSep(c) || space)
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
