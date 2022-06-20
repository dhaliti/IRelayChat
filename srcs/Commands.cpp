/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 16:25:05 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/20 11:49:32 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

/*******************************UNKNOWN COMMAND********************************/

static void unknownCommand(int &index, string &cmd)
{
	string message = ":irc.serv 421 " + cmd + " :Unknown command\n";
	sendAll(index, message);
}


/***********************************COMMANDS***********************************/

typedef void (*cmd)(Client*, int&, char**, int &);
void ft_commands(Client *clients, int &index, const char *bufRead, string &password, fd_set &active)
{
	/* LIST OF COMMANDS WITH THEIR ASSOCIATED FUNCTIONS */
	map<string, cmd> cmds;

	cmds["NICK"] = &setNick;
	cmds["USER"] = &setUser;
	cmds["OPER"] = &setOper;
	cmds["PONG"] = &pingPong;
	cmds["JOIN"] = &joinChannel;
	cmds["PART"] = &channelPart;
	cmds["KICK"] = &channelKick;
	cmds["NOTICE"] = &noticeMsg;
	cmds["PRIVMSG"] = &privateMsg;
	cmds["CLIENTS"] = &ft_clients;
	cmds["SENDFILE"] = &sendFile;
	cmds["GETFILES"] = &getFile;
	cmds["TOPIC"] = &setTopic;
	cmds["MODE"] = &setMode;

	/* RUN ACROSS READ BUFFER AND EXECUTE COMMANDS */
	int i = -1;
	int j = -1;
	char **ident = ft_split2(bufRead, "\r\n");
	while(ident && ident[++i])
	{
		j = -1;
		char **ident2 = ft_split2(ident[i], "\t ");
		while(ident2 && ident2[++j])
		{
			string value(ident2[j]);
			map<string, cmd>::iterator it = cmds.find(string(ident2[j]));
			if (it != cmds.end())
				(*it->second)(clients, index, ident2, j);
			else if (value == "PASS")
				setPass(clients, index, ident2, j, password);
			else if (value == "QUIT")
				quit(clients, index, active);
			else if (value == "KILL")
				killUser(clients, index, ident2, j, active);
			else if (isUpper(value))
				unknownCommand(index, value);
	 	}
		free(ident2);
		free(ident[i]);
	}
	free (ident);
}

void botCommand(Client *clients, int &index, char **ident2, int &j){
	Bot bot = Bot(clients[index].getNickName());
	map<string, string> command = bot.getCommands();
	map<string, string>::iterator it;

	it = command.find(string(ident2[j +2]));
	string message;
	if (it != command.end())
		message = it -> second;
	else
		message = ":BOT!BOT@irc.server PRIVMSG " + clients[index].getNickName() + string(ident2[j +2])+ " -- Unknown command.\n";
	sendAll(index, message);
}
