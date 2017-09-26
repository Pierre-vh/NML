#include <iostream>
#include <fstream>
#include "NML.h"
int main(int argc, const char * argv[])
{
	if(reporter->isOk())
		std::cout << "Still down." << std::endl;
	// NML class is a class used to parse NML files. You can re-use a lot of its code to build your own class !
	NML n;
	//n.params.describeProcess = true;
	NMLNode *source = n.parseFile("C:\\Users\\pierre.vanhoutryve\\OneDrive\\Projets Programmation\\Projets\\DataContainer\\Debug\\progfile\\basic.txt");
	assert(source);
	std::cout << source->getName();
	if (n) // Parsing success, we can proceed.
	{
		std::cout << ">The operation was a success." << std::endl;
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