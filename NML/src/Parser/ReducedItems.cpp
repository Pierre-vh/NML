#include "ReducedItems.h"
using namespace Easy;

TagCont::TagCont(const std::string &fam, const std::string &tgnm) : tag(fam), str(tgnm)
{

}

void TagCont::addAttr(std::string k, std::string d)
{
	attrs[k] = d;
}

std::map<std::string, std::string> TagCont::getAttrs() const
{
	return attrs;
}

std::string TagCont::getDataForKey(std::string key, bool & b) const
{
	auto i = attrs.find(key);
	if (i == attrs.end())
	{
		b = false;
		return "";
	}
	else
	{
		b = true;
		return	i->second;
	}
}

std::string TagCont::getName() const
{
	return str;
}

std::string TagCont::getTag() const
{
	return tag;
}

void TagCont::setTag(const std::string & ntag)
{
	tag = ntag;
}

std::string StartTag::getFriendlyName() const
{
	return "parsedStartTag";
}

std::string OrphanTag::getFriendlyName() const
{
	return "parsedOrphanTag";
}

EndTag::EndTag(const std::string & tstr) : str(tstr){}

std::string EndTag::getFriendlyName() const
{
	return "parsedTagClose";
}

std::string EndTag::getName() const
{
	return str;
}

void EndTag::setName(const std::string & nstr)
{
	str = nstr;
}
