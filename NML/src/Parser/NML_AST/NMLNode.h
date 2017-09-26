#pragma once
#include <iostream>
#include <memory>
#include <map>
#include <algorithm>
#include <sstream>
#include <assert.h>

#include "../Node.h"
#include "../../Errors/error.h"
#include "../../Globals/Globals.h"
#include "../../Data/Data.h"


#define ROOTNODE "root"

#define RETURN_IF_ERROR if(!reporter->isOk()){ std::cout << "Won't do it!" << __FILE__ << "@" << __LINE__ << std::endl; return this;}

enum dir { LEFT , RIGHT };

namespace Easy
{


	class NMLNode : public Node
	{
	public:
		NMLNode(const std::string &nodename);
		NMLNode(const std::string &nodename, const std::map<std::string, std::string> &nodeattrs, bool orphan = false);
		~NMLNode();

		// in the makeASt function that we'll do later in the parser, no need to return anything, just take a pointer to the main node as argument.   
		Node* makeChild(const std::string &tag, const std::string &name, std::map<std::string, std::string> attrs, bool orphan);

		// Navigate through the AST
		std::vector<NMLNode*> searchChild(const std::string &n); // returns all the childs with the name specified

		void setName(const std::string &str);
		std::string getName();

		Easy::Data* getAttr(const std::string &str);
		void setAttr(const std::string &str, const Easy::Data &d);

		NMLNode* getChild(const dir &d);
		NMLNode* getChild(const std::string & npos); // Returns the first child with the name specified
		NMLNode* getChild(const int &s);


		std::string compileToStr(const int &tab = -1); // This will compile the ast
		void presentYourself();
	private:

		// The NML Default AST suppors : An array of childs without tags and a left/right child.
		std::string name = "";
		std::map<std::string, Easy::Data> attrs; // Attributes are converted to C++ types, stored in Easy::Data.

		std::vector<std::unique_ptr<NMLNode>> childs;
		std::unique_ptr<NMLNode> left = 0;
		std::unique_ptr<NMLNode> right = 0;

		bool isOrphan = false;

		std::string tabsForInt(const int &i);

		// This class must not be copyable. 
		NMLNode(const NMLNode &n) = delete;
		NMLNode& operator=(const NMLNode &n) = delete;
	};
} 
