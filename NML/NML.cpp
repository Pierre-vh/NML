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

	if (t.is_open())
	{
		if(params.describeProcess)
			std::cout << "File \"" << filename << "\"opened Successfully." << std::endl;
	}
	else BASE_ERROR(reporter, Easy::CANT_OPEN_FILE, "File :" + filepath);

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

	if (params.describeProcess)
		std::cout << "Number of token found : " << toks.size() << ", The original string was made of " << str.size() << " characters." << std::endl;
	if (params.showTokens)
		for (auto it = toks.begin(); it != toks.end(); it++)
			std::cout << "Token found : " << (*it).str << " of type " << it->getTypeAsString() << std::endl;

	Parser p;
	if (params.describeProcess)
		showTitle("Parsing.. (Syntaxic Analysis)");
	if (params.showParsingTable)
	{
		std::cout << "Parsing table :" << std::endl << std::endl;
		p.printParsingTable();
	}
	if (params.describeProcess)
		std::cout << std::endl << "Parsing..." << std::endl;

	PARSEDITEMS_LIST parsed = p.parse(toks); // PARSING!

	RETURN_IF_ERROR

	if (params.describeProcess)
		if (!parsed.empty())
		{
			showTitle("Parsing Process Completed successfully. Results: ");

			std::string tab = "";
			for (auto i = parsed.begin(); i != parsed.end(); i++)
			{
				//  This code is purely for formatting purposes.
				auto sb = std::dynamic_pointer_cast<EndTag>(*i);
				std::shared_ptr<StartTag> sp;
				if (sp = std::dynamic_pointer_cast<StartTag>(*i))
					tab += '\t';
				//
				std::cout << ((sp || sb) ? tab : (tab + "\t")) << (*i)->reconstructOriginal() << std::endl;
				//	Same
				if (sb)
					tab.pop_back();
			}
		}
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
