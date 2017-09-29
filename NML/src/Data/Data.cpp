#include "Data.h"

using namespace Easy;

// Constructors
Data::Data(const int & ival)
{
	this->setData(ival);
}

Data::Data(const float & fval)
{
	this->setData(fval);
}

Data::Data(const char & cval)
{
	this->setData(cval);
}

Data::Data(const std::string & sval)
{
	this->setData(sval);
}

Data::Data(const bool & bval)
{
	this->setData(bval);
}

Data::Data(const char str[])
{
	this->setData(std::string(str));
}

Data::Data()
{
	this->makeNull();
}

// Data Setters
void Data::setData(const int & ival)
{
	cleanup();

	data_.data_int = ival;
	cur = INTEGER;
}

void Data::setData(const float & fval)
{
	cleanup();

	data_.data_float = fval;
	cur = FLOAT;
}

void Data::setData(const char & cval)
{
	cleanup();

	data_.data_char = cval;
	cur = CHAR;
}

void Data::setData(const std::string & sval)
{
	cleanup();

	data_.data_string = sval;
	cur = STRING;
}

void Data::setData(const bool & bval)
{
	cleanup();

	data_.data_boolean = bval;
	cur = BOOLEAN;
}

void Data::makeNull()
{

	cleanup();

	cur = NULLDATA;
}

DataTypes Data::setDataFromString(const std::string & str)
{
	std::string strlow = str;
	std::transform(strlow.begin(), strlow.end(), strlow.begin(), ::tolower);
	try
	{
		if (strlow == "null")
		{
			this->makeNull();
			return NULLDATA;
		}
		else if (strlow == "true")
		{
			this->setData(true);
			return BOOLEAN;
		}
		else if (strlow == "false")
		{
			this->setData(true);
			return BOOLEAN;
		}
		else
		{
			if (str.front() == '"' && str.back() == '"')
			{
				this->setData(str.substr(1, str.size() - 2));
				return STRING; // sets the value as a string
			}
			else if (str.front() == '\'' && str.back() == '\'')
			{
				if (str.size() > 3)
				{
					BASE_WARNING(reporter, "There was more than one character inside a char declaration, only the first character after the opening quote was used. Concerned declaration : " + str)
					this->setData((char)str[1]);
				}
				else if (str.size() < 3)
				{
					BASE_WARNING(reporter, "A char declaration was completly empty.The value was set to a \"space\" char : ' '");
					this->setData(char(0));
				}
				else // Normal case. 
					this->setData((char)str[1]);
				return CHAR;
			}
			// This is where it gets lazy !
			// To decide between if it's an int or a float, we just check if we have a dot or not :) :) :)
			else if ((str.find('.') != std::string::npos))
			{
				this->setData(std::stof(str));
				return FLOAT;
			}
			else // it's an int the
			{
				this->setData(std::stoi(str));
				return INTEGER;
			}
		}
	}
	catch (std::exception e)
	{
		BASE_WARNING(reporter, "[!] Error while attempting to convert the value ." + str + ". (lowercase: ." + strlow + ".to a known datatype.Defaulted to string");
		this->setData(str);
		return STRING;
	}

}

std::string Data::asString() const
{	
	std::stringstream s;
	switch (cur)
	{
		case INTEGER:
			s << data_.data_int;
			return s.str();
		case FLOAT:
			s << data_.data_float;
			return s.str();
		case CHAR:
			return std::string(1,data_.data_char);
		case STRING:
			return data_.data_string;
		case BOOLEAN:
			return (data_.data_boolean) ? BOOLSTR_TRUE : BOOLSTR_FALSE;
		case NULLDATA:
			return NULL_STRING;
		default:
			BASE_ERROR(reporter, Easy::ENUM_UNKNOWN_DATA_TYPE, "");
			return "";
	}
}

// Getters

template<>
int Data::getData<int>() const
{
	if (cur == INTEGER)
		return data_.data_int;

	badGetErr("Attempted to get<int() when the Data's type was not a int");
	return 0;
}


template<>
float Data::getData<float>() const
{
	if (cur == FLOAT)
		return data_.data_float;

	badGetErr("Attempted to get<char>() when the Data's type was not a char");
	return 0;
}

template<>
char Data::getData<char>() const
{
	if (cur == CHAR)
		return data_.data_char;

	badGetErr("Attempted to get<char>() when the Data's type was not a char");
	return 0;
}

template<>
std::string Data::getData<std::string>() const
{
	if (cur == STRING)
		return data_.data_string;

	badGetErr("Attempted to get<std::string>() when the Data's type was not a std::string");
	return 0;
}

template<>
bool Data::getData<bool>() const
{
	if (cur == BOOLEAN)
		return data_.data_boolean;

	badGetErr("Attempted to get<bool>() when the Data's type was not a boolean.");
	return 0;
}

std::string Data::asFormattedString() const
{
	std::string nstr = this->asString();
	if (cur == STRING)
		nstr = '"' + nstr + '"';
	else if (cur == CHAR)
		nstr = '\'' + nstr + '\'';
	return nstr;
}

// ISNULL
bool Data::isNull() const
{
	return (cur == NULLDATA);
}

// Utility Functions
bool Data::isEmpty() const
{
	return (cur == EMPTY);
}

DataTypes Data::getType() const
{
	return cur;
}

std::string Data::getTypeAsStr() const
{
	switch (cur)
	{
		case INTEGER:
			return "int";
		case FLOAT:
			return "float";
		case CHAR:
			return "char";
		case STRING:
			return "string";
		case BOOLEAN:
			return "bool";
		case NULLDATA:
			return "null";
		case EMPTY:
			return "(empty)";
	}
	return std::string();
}

Data& Data::operator=(const Data & cop)
{
	switch (cop.getType())
	{
		case INTEGER:
			this->setData(cop.getData<int>());
			break;
		case FLOAT:
			this->setData(cop.getData<float>());
			break;
		case CHAR:
			this->setData(cop.getData<char>());
			break;
		case STRING:
			this->setData(cop.getData<std::string>());
			break;
		case BOOLEAN:
			this->setData(cop.getData<bool>());
			break;
		case NULLDATA:
			this->makeNull();
			break;
		case EMPTY:
			this->makeNull();
			BASE_WARNING(reporter,"The right term of the assignement operator used a Data instance that was empty. Made the left instance null as a result.");
			break;
		default:
			BASE_ERROR(reporter,Easy::GENERIC_ERROR, "operator=() defaulted.");
			break;
	}
	return *this;
}

bool Data::operator==(const Data & b)
{
	return (this->getType() == b.getType()) && (this->asString() == b.asString());
}

bool Data::operator!=(const Data & b)
{
	return !(*this == b);
}

// Destructor
Data::~Data()
{

}

void Data::badGetErr(const std::string & str) const
{
	BASE_ERROR(reporter, DATA_GET_ERROR, str);
}

void Data::cleanup()
{
	data_.data_boolean = false;
	data_.data_char = ' ';
	data_.data_float = 0.0f;
	data_.data_string = "";
	data_.data_boolean = false;
	cur = EMPTY;
}


