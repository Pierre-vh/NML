#include <iostream>
#include <fstream>
#include "NML.h"
int main(int argc, const char * argv[])
{
	// NML class is a class used to parse NML files. You can re-use a lot of its code to build your own class !
	NML n;
	n.params.describeProcess = true;
	NMLNode *source = n.parseFile("basic.txt");
	assert(source); // Just in case something  went wrong. You can also check if an error happened with reporter->isOk() (from globals.h) or simply usinf if(n) (it does the same thing, but the syntax is less verbose)
	std::cout << source->getName();
	if (n) // Parsing success, we can proceed.
	{
		std::cout << ">The file was success fully interpreted" << std::endl;
		std::cout  << source->compileToStr() << std::endl;
		// This will thrown an error !
		//source->getChild("OPDIV")->getChild(LEFT)->getChild("ERROR!!!!!");

		// This will not !
		std::cout << "NAME:" << source->getChild("OPDIV")->getChild(LEFT)->getChild(LEFT)->getName();
	}
	else
		std::cout << ">Failed to interpret file." << std::endl;
	std::cin.get();
	return 1;
}
