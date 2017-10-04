#include "Parser.h"

using namespace Easy;

Parser::Parser()
{
}

Parser::~Parser()
{
}

PARSEDITEMS_LIST Easy::Parser::parse(const std::list<token>& input)
{
	for (auto i = input.begin(); i != input.end(); i++)
	{
		this->manageToken(*i);
		// For each token in the stream
		auto find = action_table.find(std::make_pair(current_state, i->type)); // Find if the current event is legal
		if (find != action_table.end())
		{
			AT_DATA move = find->second;
			// It's legal, do what is needed.
			if (move.first == SHIFT)
			{
				current_state = move.second; // Shift to Sx, continue.
				continue;
			}
			else if (move.first == REDUCE)
			{
				// Reduction operation, check in the table for the corresponding op.
				auto redfind = gtfn_table.find(move.second); // Find if the state exists. It should exist but let's make sure !
				if (redfind != gtfn_table.end())
				{
					// It exists !
					GTFN_DATA redmove = redfind->second;
					GTFN_FUNC fn = redmove.first;
					fn();
					current_state = redmove.second; // Shift to Sx,continue;
					continue;
				}
				{
					BASE_ERROR(reporter, Easy::GENERIC_ERROR, "");
					return PARSEDITEMS_LIST();
				}
			}
		}
		else
		{
			std::stringstream ss;
			ss << "Unexpected token [state(" << current_state << ")]: " << i->str << " of type <" << token::getTypeAsString(i->type) << ">";
			ss << "(line :" << i->pos.line /*<< ",column :" << i->pos.column*/ << ")" << std::endl;
			ss << "Expected: " << this->getExpEntriesForCurState(", ") << std::endl;
			BASE_ERROR(reporter, Easy::UNEXPECTED_TOKEN, ss.str());
			return PARSEDITEMS_LIST(); // stop the parsing here.
		}
	}
	return output;
}

void Easy::Parser::makeTree(Node *n)
{
	if (!reporter->isOk())
	{
		reporter->reportWarning("[IMPORTANT] An error has been reported earlier. Can't build the tree in an unhealthy environement.");
		return;
	}
	std::stack<Node*> parents;
	std::stack<std::string> opentags; 
	parents.push(n); // N is the parent of everything here.
	for (auto i = output.begin(); i != output.end(); i++)
	{
		if (auto st = std::dynamic_pointer_cast<StartTag>(*i)) // if *i is a starttag
		{
			opentags.push((*st).getName()); // Open it !
			auto newnode = parents.top()->makeChild((*st).getTag(), (*st).getName(), (*st).getAttrs(), false);
			parents.push(newnode);
			// Simple explanation : makes a node using parents.top() as a parent and pushes it at the top of the stack 
			// So it becomes the new parent of everything that's contained inside the block.
			continue;
		}
		else if (auto orp = std::dynamic_pointer_cast<OrphanTag>(*i))
		{
			parents.top()->makeChild((*orp).getTag(), (*orp).getName(), (*orp).getAttrs(),true);
			// Exact same as the one above, except that we don't push it at the top of the stack 
			// (an orphan node doesn't have children + we don't add it to opentags for the same reason)
		}
		else if (auto close = std::dynamic_pointer_cast<EndTag>(*i))
		{
			if (opentags.size() != 0)
			{
				if ((*close).getName() == opentags.top()) // That should always be the case
				{
					opentags.pop();	parents.pop();
					continue;
				}
				else
				{
					std::string str = "Attempted to close the tag " + (*close).getName() + " when it was not the latest tag opened.\n";
					str += "Whenever you open a block, it needs to be closed first before closing other blocks.\n Last tag opened :" + opentags.top();
					BASE_ERROR(reporter, Easy::PARSING_ERROR,str);
				}
			}
			else BASE_ERROR(reporter, Easy::PARSING_ERROR, "Attempted to close a tag when there was no tags open.");
		}
		else BASE_ERROR(reporter, Easy::PARSING_ERROR, "Encountered an unknown type");
	}

	if (opentags.size() != 0)
		BASE_ERROR(reporter, Easy::PARSER_UNCLOSED_TAG, "opentags.top() = " + opentags.top());
}

std::string Easy::Parser::getExpEntriesForCurState(std::string sep) const
{
	std::string t;
	for (auto i = action_table.begin(); i != action_table.end(); i++)
	{
		AT_KEY key = i->first;
		if (key.first == current_state)
		{
			if (t != "")
				t.append(sep + token::getTypeFriendlyRepresentation(key.second));
			else
				t.append(token::getTypeFriendlyRepresentation(key.second));
		}
	}
	return t;
}

void Easy::Parser::manageToken(const token & t)
{
	if (t.type == id)
		ids.push(t.str);
	else if (t.type == data)
		lastvalue = t.str;
}

void Easy::Parser::parseInfoTag()
{
	lasttaginfo = ids.top();
	ids.pop();
}

void Easy::Parser::parseStartTag()
{
	auto sp = std::make_shared<StartTag>(lasttaginfo,ids.top());
	for (auto i = attributes.begin(); i != attributes.end(); i++)
		sp->addAttr(i->first, i->second);
	attributes.clear();
	ids.pop();
	output.push_back(sp);
}

void Easy::Parser::parseEndTag()
{
	auto sp = std::make_shared<EndTag>(ids.top());
	ids.pop();
	output.push_back(sp);
}

void Easy::Parser::parseOrphanTag()
{
	auto sp = std::make_shared<OrphanTag>(lasttaginfo, ids.top());
	for (auto i = attributes.begin(); i != attributes.end(); i++)
		sp->addAttr(i->first, i->second);
	attributes.clear();
	ids.pop();
	output.push_back(sp);
}

void Easy::Parser::parseAttr()
{
	attributes.push_back(std::make_pair(ids.top(), lastvalue));
	ids.pop();
}

void Easy::Parser::printParsingTable() const
{
	std::cout << "Parsing table :" << std::endl << std::endl;
	for (auto i = action_table.begin(); i != action_table.end(); i++)
	{
		AT_KEY key = i->first;
		AT_DATA data = i->second;

		std::cout << "[" << key.first << "](" << token::getTypeAsString(key.second) << ") -> " << ((data.first == REDUCE) ? "REDUCE " : "SHIFT ") << data.second << std::endl;
	}
}