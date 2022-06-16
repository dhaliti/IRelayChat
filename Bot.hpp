#pragma once
#include "IRC.hpp"
#include <map>

class Bot{
	private:
		string _nickname;
		map<string, string> _command;

	public:
		Bot(string nickname);
		~Bot(void);
		const map<string, string> getCommands(void);
};