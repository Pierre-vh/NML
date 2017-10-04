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
#include <algorithm>
#include <functional>

#include "../Errors/error.h"
#include "../Globals/Globals.h"
#include "../Lexer/Lexer.h"
#include "Node.h"

#include "ReducedItems.h"

#define AT_ADD(x,y,z) {x,{y,z}}

// Theses macro are here to get a less verbose code.
#define ACTIONTABLE std::map<std::pair<short,tokentype>,std::pair<action,short> > 
#define AT_KEY std::pair<short,tokentype>
#define AT_DATA std::pair<action,short>

#define GTFN_TABLE std::map<short,std::pair<std::function<void(void)>,short> > // This table uses a terminal state as the key, and the data is the function to run + the state to go to.
#define GTFN_DATA std::pair<std::function<void(void)>,short> 
#define GTFN_FUNC std::function<void(void)>

#define PARSEDITEMS_LIST std::list<std::shared_ptr<Easy::ParsedItem>>


namespace Easy
{
	class ParsedItem;

	enum action {SHIFT,REDUCE};
	struct reduce_action
	{
		std::function<void(void)> func;	// The rule to reduce from
		short goto_state;				// The state to go to when the function has been ran
	};


	class Parser
	{
		public:
			Parser();
			~Parser();

			void parse(const std::list<token> &input);
			void makeTree(Node *n); // Makes a tree with n as its source node.

			void printParsingTable() const;

			unsigned short current_state = 0;

		private:
			std::string getExpEntriesForCurState(std::string sep) const; // sep is a separator for the different entries, like here 'a,b' , is the separator.
			void manageToken(const token &t); // This function pushes id onto the stack or set the value attribute.
			// Output
		
			PARSEDITEMS_LIST output;
			// Keep track of all the things
			std::vector<std::pair<std::string, std::string>> attributes; // parsedAttrs results

			std::stack<std::string> ids; // Each ID encountered is pushed onto this stack, to be used later in any parse function.
			std::string lastvalue; // whenever a value is encountered, it is stored here to be used in parseAttr
			std::string lasttaginfo = "";
			// reductions
			void parseInfoTag();
			void parseStartTag();
			void parseEndTag();
			void parseOrphanTag();
			void parseAttr();
			// The action table used
			ACTIONTABLE action_table =
			{
				{ { 0,SQB_OPEN	  },{ SHIFT	,1 } },
				{ { 0,LESS_THAN	  },{ SHIFT	,4 } },
				{ { 1,id		  },{ SHIFT	,2 } },
				{ { 2,SQB_CLOSE	  },{ REDUCE,3 } },	// parseInfoTag
				{ { 4,SLASH		  },{ SHIFT	,5 } },
				{ { 4,id		  },{ SHIFT	,8 } },
				{ { 5,id		  },{ SHIFT	,6 } },
				{ { 6,GREATER_THAN},{ REDUCE,7 } }, // parseEndTag
				{ { 8,GREATER_THAN},{ REDUCE,11} },	// parseStartTag
				{ { 8,SLASH		  },{ SHIFT ,9 } },
				{ { 8,id		  },{ SHIFT ,12} },
				{ { 9,GREATER_THAN},{ REDUCE,10} }, // parseOrphanTag
				{ {12,EQUAL		  },{ SHIFT ,13} },
				{ {13,data		  },{ REDUCE,14} }  // parseAttr	
			};

			GTFN_TABLE gtfn_table =
			{
				//  STATE		FUNCTION TO EXECUTE			  NEXT STATE TO GO TO
					{ 3,{ [=]() {this->parseInfoTag();		 },0 } }, // Lambda !
					{ 7,{ [=]() {this->parseEndTag();		 },0 } },
					{10,{ [=]() {this->parseOrphanTag();     },0 } },
					{11,{ [=]() {this->parseStartTag();		 },0 } },
					{14,{ [=]() {this->parseAttr();			 },8 } },
			};
	};
}
