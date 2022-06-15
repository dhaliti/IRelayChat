#pragma once
#include "IRC.hpp"

class File
{
	private:
		string	_name;
		string	_content;
		string	_sender;
		string	_dest;

	public:
		File(const string&, const string&, const string&, const string&);
		~File();

		string	getName(void);
		string	getContent(void);
		string	getSender(void);
		string	getDest(void);
};