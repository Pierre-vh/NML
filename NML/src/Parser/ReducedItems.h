#pragma once

#include <string>
#include "Parser.h"
#include <vector>
// This file defines the various structs used to store the reduced data 
namespace Easy
{
	class ParsedItem
	{
		public:
			virtual std::string getFriendlyName() const = 0;
	};
	class TagCont // StartTag & OrphanTag share a LOT of stuff (They're nearly identical except for the name), so we group them under one class
	{
	public:
			TagCont(const std::string &fam, const std::string &tgnm);
			void addAttr(std::string k, std::string d);
			std::map<std::string, std::string> getAttrs() const;

			std::string getDataForKey(std::string key, bool &b) const; // the key and a bool, the bool will be false if the key does not exists.

			std::string getName() const;

			std::string getTag() const;
			void setTag(const std::string &ntag);
		protected:
			std::string tag;
			std::string str; // tag name
			std::map<std::string, std::string> attrs;
	};

	class StartTag : public TagCont, public ParsedItem
	{
		public:
			using TagCont::TagCont;
			std::string getFriendlyName() const;
	};

	class OrphanTag : public TagCont, public ParsedItem // the orphan tag is a carbon-copy of the starttag class
	{
		public:
			using TagCont::TagCont;
			std::string getFriendlyName() const;
	};

	class EndTag : public ParsedItem
	{
		public:
			EndTag(const std::string &tstr);
			std::string getFriendlyName() const;

			std::string getName() const;
			void setName(const std::string &nstr);
		private:
			std::string str;
	};
}