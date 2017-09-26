#pragma once
#include <iostream>

#include <string>
#include <memory>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <stack>

// This is the base node class, all nodes will need to inherit from this.
namespace Easy
{
	class Node
	{
		public:
			Node() {}
			virtual Node* makeChild(const std::string &tag, const std::string &name, std::map<std::string, std::string> attrs, bool orphan) = 0;
			// Adds a child to (this) and returns a reference to the child.
		private:
			Node(const Node &n) = delete;
			Node& operator=(const Node &n) = delete;
	};

}