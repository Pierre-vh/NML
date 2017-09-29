#include <iostream>
#include <fstream>
#include "NML.h"
int main(int argc, const char * argv[])
{
	// NML class is a class used to parse NML files. You can re-use a lot of its code to build your own class !
	NML n; // NML Parsing "Assistant"
	//n.params.describeProcess = true;
	NMLNode *source = n.parseFile("C:\\Users\\pierre.vanhoutryve\\OneDrive\\Projets Programmation\\Projets\\DataContainer\\Debug\\progfile\\basic.txt");
	if (reporter->isOk()) // Parsing success, we can proceed.
	{
		std::cout << ">The operation was a success." << std::endl;
		std::cout  << source->compileToStr() << std::endl;
		// This will thrown an error ! (Node does not exists)
		NMLNode *n = source->getChild("OPDIV")->getChild(LEFT)->getChild("ERROR!!!!!");
		if (!n)
			std::cout << "Error while attempting to find a node!" << std::endl;
		// This will not !
		auto node = source->getChild("OPDIV")->getChild(LEFT)->getChild(LEFT);
		node->setAttr("NewAttribute",Easy::Data());
		node->setName("NewName");

		std::cout << "Edited tree" << std::endl;
		std::cout << source->compileToStr() << std::endl;
	}
	else
		std::cout << ">Failed to interpret file." << std::endl;
	std::cin.get();
	return 1;
}