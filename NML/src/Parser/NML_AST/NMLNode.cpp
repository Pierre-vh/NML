#include "NMLNode.h"

using namespace Easy;

NMLNode::NMLNode(const std::string & nodename) : name(nodename)
{
}

NMLNode::NMLNode(const std::string &nodename, const std::map<std::string, std::string>&nodeattrs, bool orphan) : name(nodename), isOrphan(orphan)
{
	for (auto i = nodeattrs.begin(); i != nodeattrs.end(); i++)
	{
		Easy::Data d;
		d.setDataFromString(i->second);
		attrs[i->first] = d;
	}
}

NMLNode::~NMLNode()
{
}

Node* NMLNode::makeChild(const std::string &tag, const std::string &name, std::map<std::string, std::string> attrs, bool orphan)
{
	std::string lctag = tag;
	std::transform(lctag.begin(), lctag.end(), lctag.begin(), ::tolower); // puts Tag to lowercase to be case-insensitive
	if (lctag == "left")
	{
		left = std::make_unique<NMLNode>(name, attrs,orphan);
		return left.get();
	}
	else if (lctag == "right")
	{
		right = std::make_unique<NMLNode>(name, attrs,orphan);
		return right.get();
	}
	else
	{
		if (lctag != "")
			reporter->reportWarning("[NMLNode] Encountered an unsupported tag, it was ignored."); // If the tag is unsupported, we simply ignore it.

		childs.push_back(std::make_unique<NMLNode>(name, attrs,orphan));
		return childs.back().get();
	}

}

NMLNode * NMLNode::getChild(const dir & d)
{
	RETURN_IF_ERROR

	if (d == LEFT)
		if (left)
			return left.get();
	else if (d == RIGHT)
		if(right)
			return right.get();
	else
		throwWarning("Unknown direction for getChild()");
		return this;
}

NMLNode * NMLNode::getChild(const std::string & npos) // Get childs by name
{
	RETURN_IF_ERROR

	for (auto x = childs.begin(); x != childs.end(); x++)
	{
		if ((*x)->getName() == npos)
			return (*x).get();
	}
	throwWarning("Tried to find child \"" + npos + "\". It does not exists.");
	return nullptr;
}

NMLNode * NMLNode::getChild(const int & s)
{
	RETURN_IF_ERROR

	if (childs.size() <= s)
	{
		std::stringstream ss;
		ss << "Attempted to access a child that does not exists. (Attempted to access child no " << s << " when the vector size was " << childs.size() << " on node " << name << ")" << std::endl;
		throwWarning(ss.str());
		return this;
	}
	return childs[s].get();
}

std::vector<NMLNode*> NMLNode::searchChild(const std::string & n)
{
	if(!reporter->isOk())
		return std::vector <NMLNode*>(); // If you try to access a child that simply doesn't exist (because there was an error earlier or here), it can cause a crash (out of range, etc..). But you're responsible right, you check for the array's size beforehand, right? it shouldn't happen ? :)

	std::vector<NMLNode*> results;
	for (auto x = childs.begin(); x != childs.end(); x++)
	{
		if ((*x)->getName() == n)
			results.push_back((*x).get());
	}
	if (left->getName() == n)
		results.push_back(left.get());
	else if (right->getName() == n)
		results.push_back(right.get());

	if (results.size() == 0)
		throwWarning("No node called " + n + " found.");
	return results;
}

void NMLNode::setName(const std::string & str)
{
	name = str;
}

std::string NMLNode::getName()
{
	return name;
}

Easy::Data * NMLNode::getAttr(const std::string & str)
{
	auto i = attrs.find(str);
	if (i != attrs.end())
		return &(i->second);
	else
	{
		reporter->reportWarning("Couldn't find attribute with name " + str + " in Node " + name + ". Returned a null data as a result.");
		Easy::Data *d = new Easy::Data();
		d->makeNull();
		return d;
	}
}

void NMLNode::setAttr(const std::string & str, const Easy::Data & d)
{
	attrs[str] = d;
}

std::string NMLNode::compileToStr(const int & tab)
{
	if(!reporter->isOk()) // If there was a failure somewhere, it's too risky to try and access the (probably) incomplete tree.
		return "The reporter reported an error somewhere. Can't generate the tree in a unhealthy environment"; 
	std::stringstream ss;
	if (name != ROOTNODE)
	{
		ss << tabsForInt(tab) << "<" << name << (attrs.size() != 0 ? " " : "");
		for (auto x = attrs.begin(); x != attrs.end(); x++)
			ss << x->first << "=" << x->second.asFormattedString() << ((std::next(x) == attrs.end()) ? "" : " ");;
		// The last bit might be confusing : we only insert the separator if that's not the last element of the attributes
		if (isOrphan)
			ss << "/>" << std::endl;
		else
		{
			ss << ">" << std::endl;
			if (left)
				ss << tabsForInt(tab+1) << "[LEFT]" << std::endl << left->compileToStr(tab + 1);
			if (right)
				ss << tabsForInt(tab+1) << "[RIGHT]" << std::endl << right->compileToStr(tab + 1);
			if (childs.size() != 0)
			{
				for (auto k = childs.begin(); k != childs.end(); k++)
				{
					ss << (*k)->compileToStr(tab+1);
				}
			}
			ss << tabsForInt(tab) << "</" << name << ">" << std::endl;
		}
	}
	else // if this is the root node, we should only compile childs
	{
		for (auto k = childs.begin(); k != childs.end(); k++)
		{
			ss << (*k)->compileToStr(tab+1) << std::endl;
		}
	}

	return ss.str();
}

void NMLNode::presentYourself()
{
	std::cout << "[NMLNode]:" << name << " with Attributes : " << std::endl;
	for (auto i = attrs.begin(); i != attrs.end(); i++)
	{
		std::cout << "[" << i->first << "=" << i->second.asString() << " (type :"  << i->second.getTypeAsStr() << ")]" << std::endl;
	}
	if (left)
	{
		std::cout << "\t"<< name << "'s LEFT CHILD:" << std::endl;
		left->presentYourself();
	}
	if (right)
	{
		std::cout << "\t" << name << "'s RIGHT CHILD:" << std::endl;
		right->presentYourself();
	}
	for (auto i = childs.begin(); i != childs.end(); i++)
	{
		std::cout << name << "'s other child : " << std::endl;
		(*i)->presentYourself();
	}
	
}

Easy::NMLNode::operator bool()
{
	return badFlag;
}

void Easy::NMLNode::throwWarning(const std::string & s)
{
	BASE_WARNING(reporter, s);
	badFlag = true;
}

std::string NMLNode::tabsForInt(const int & i)
{
	std::string s;
	for (int x = 0; x < i; x++)
		s += "\t";
	return s;
}
