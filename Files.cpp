#include "Files.hpp"

File::File(const string &name, const string &content, 
	const string &sender, const string &dest)
{
	this->_name = name;
	this->_content = content;
	this->_sender = sender;
	this->_dest = dest;
}

File::~File() {}

string		File::getName(void) {return this->_name;}
string		File::getContent(void) {return this->_content;}
string		File::getSender(void) {return this->_sender;}
string		File::getDest(void)
{
	cout << "GET DEST" << endl;
	cout << this->_dest << endl;
	return this->_dest;
}
