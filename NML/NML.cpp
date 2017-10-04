#include "NML.h"

NML::NML(){}

NML::~NML(){}

NMLNode * NML::parseFile(const std::string & filepath)
{
	std::fstream t;
	std::string filename = "C:\\Users\\pierre.vanhoutryve\\OneDrive\\Projets Programmation\\Projets\\DataContainer\\Debug\\progfile\\basic.txt";
	t.open(filename);

	if (params.describeProcess)
		showTitle("Opening file..");

	if (t.is_open() && params.describeProcess)
		std::cout << "File \"" << filename << "\"opened Successfully." << std::endl;
	else if(!t.is_open())
		BASE_ERROR(reporter, Easy::CANT_OPEN_FILE, "File :" + filepath);

	std::string str((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	return this->parseString(str);
}

NMLNode * NML::parseString(const std::string & str)
{

	std::stringstream ss;
	if (params.describeProcess)
	{
		ss << "NML Parser - v0.1 [Last built on the " << __DATE__ << " at " << __TIME__ << "]";
		showTitle(ss.str());
		showTitle("Lexing.. (Lexical Analysis)");
	}

	Lexer lex;
	std::list<Easy::token> toks;
	toks = lex.lex(str);

	RETURN_IF_ERROR

	if (params.showFileStats)
	{
		std::cout << "Some stats about the original string : " << std::endl;
		std::cout << "-> It was " << lex.stats.clength << " characters in length." << std::endl;
		std::cout << "-> While lexing,we found : " << std::endl;
		std::cout << "\t->" << lex.stats.tokensfound << " tokens." << std::endl;
		std::cout << "\t->" << lex.stats.nl << " New lines (\\n) and " << lex.stats.tabs << " tabs (\\t)." << std::endl;
	}
	if (params.showTokens)
		for (auto it = toks.begin(); it != toks.end(); it++)
			std::cout << "Token found : " << (*it).str << " of type " << it->getTypeAsString() << std::endl;

	Parser p;
	if (params.describeProcess)
		showTitle("Parsing.. (Syntaxic Analysis)");
	if (params.showParsingTable)
		p.printParsingTable();

	if (params.describeProcess)
		std::cout << std::endl << "Parsing..." << std::endl;

	PARSEDITEMS_LIST parsed = p.parse(toks); // PARSING!

	RETURN_IF_ERROR

	if (params.describeProcess)
		std::cout << "Parsing completed successfully." << std::endl;

	if (params.describeProcess)
		showTitle("BUILDING THE AST..");

	RETURN_IF_ERROR

	NMLNode *n = new NMLNode(ROOTNODE);
	p.makeTree(n);

	RETURN_IF_ERROR

	return n;
}

NML::operator bool() const
{
	return reporter->isOk();
}
