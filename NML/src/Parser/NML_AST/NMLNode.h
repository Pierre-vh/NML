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

#define NML_RETURN_IF_ERROR if(!reporter->isOk()){ return this;}

enum dir { LEFT , RIGHT };

namespace Easy
{


	class NMLNode : public Node
	{
	public:
		NMLNode(const std::string &nodename);
		NMLNode(const std::string &nodename, const std::map<std::string, std::string> &nodeattrs, bool orphan = false);
		~NMLNode();

		Node* makeChild(const std::string &tag, const std::string &name, std::map<std::string, std::string> attrs, bool orphan);

		// Edit some properties of the node

		void setName(const std::string &str);
		std::string getName();

		Easy::Data* getAttr(const std::string &str);
		void setAttr(const std::string &str, const Easy::Data &d);

		// Navigate through the AST

		NMLNode* getChild(const dir &d);
		NMLNode* getChild(const std::string & npos); // Returns the first child with the name specified
		NMLNode* getChild(const unsigned int &s);
		std::vector<NMLNode*> searchChild(const std::string &n); // returns all the childs with the name specified


		std::string compileToStr(const int &tab = -1); // This will compile the ast

		operator bool(); // This check the 'health' of the node. For instance, if you performed a search and no result were found, this will return false.
	private:
		void throwWarning(const std::string &s);
		bool badFlag = false;
		// The NML Default AST supports : An array of children without tags and a left/right child.
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
