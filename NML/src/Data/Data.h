#pragma once

#include <iostream>
#include <type_traits>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm> // std::transform
#include <streambuf>

#include "../Errors/error.h"
#include "../Globals/Globals.h"

// This class is used to store objects of different types in one vector.
// Use Case : Symbol table. It will have to stock each variable value's

#define BAD_GET(x)  EasyError(__FILE__,__LINE__,ERROR::BAD_CAST,x)


// Value used for the bool to string conversion
#define BOOLSTR_TRUE "true"
#define BOOLSTR_FALSE "false"
#define NULL_STRING "null" // used when asString is used on a null data.


// By default, this class throws error if you attempt to use getData<type>() on a different data type. It can be changed.
namespace Easy
{ 
	union DataUnion
	{
		// This Union holds all the possible data types.
		// Reminder : Unions are only as big as their largest data member, and share the same memory location. 
		// Reading from the non-active data member (the most recently written) is an undefined behaviour. This is why an enum is used to keep track of which member is used.
		// When nothing is given to the Data class for initialization, the default value is null.
		int data_int;
		float data_float;
		char data_char;
		std::string data_string = "";
		bool data_boolean;

		// Needed to fix compile errors?
		DataUnion(){}
		~DataUnion(){}
	};
	enum DataTypes
	{
		// This enum is used to keep track of what's the active member of the union.
		INTEGER,
		FLOAT,
		CHAR,
		STRING,
		BOOLEAN, 
		NULLDATA, // Null
		EMPTY
	};

	class Data
	{
		public:
			Data(const int &ival);			// INT
			Data(const float &fval);		// FLOAT
			Data(const char &cval);			// CHAR
			Data(const std::string &sval);	// STRING
			Data(const bool &bval);			// BOOL
			Data(const char str[]);
			Data();							// NULL

			//SET
			void setData(const int &ival);			// INT
			void setData(const float &fval);		// FLOAT
			void setData(const char &cval);			// CHAR
			void setData(const std::string &sval);	// STRING
			void setData(const bool &bval);			// BOOL
			void makeNull();						// NULL

			DataTypes setDataFromString(const std::string &str); // This function make a corresponding data object from a string and returns the new type.

			// GET
			template<typename T>
			T getData() const;

			std::string asString() const; // Prints the data as String.
			// One might be wondering, why not just use getData<string>() ?
			// Because getData<string>() is designed, like other getData<X>() functions, to return the value of the data Only if X is the type of the data that's stored.
			// So this function is created to avoid confusion, and to have the freedom to just do data.asString();

			std::string asFormattedString() const; // will return the type as a formatted string (with '' or "" for strings, etc)

			// ISNULL
			bool isNull() const;


			bool isEmpty() const; // Checks for emptiness

			DataTypes getType() const;
			std::string getTypeAsStr() const;

			// Opérateur d'assignement
			Data& operator=(const Data& cop);

			// Comparaisons !
			bool operator==(const Data &b);
			bool operator!=(const Data &b);

			~Data();
		private:

			void badGetErr(const std::string &str) const;

			void cleanup(); // Reset all data to 0 & cur to EMPTY

			DataTypes cur;
			DataUnion data_;
	};
}