/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 16:25:05 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/14 15:15:40 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "IRC.hpp"
#include <sys/socket.h>


static void channelPart(Client *clients, int &index, char **ident2, int &j)
{
	cout << "--part\n";
	if (!ident2[j + 1])
	{
		string error = ":irc.serv 461 PART :Not enough parameters\n";
		send(index, error.c_str(), error.size(), 0);
		return ;
	}
	string channel = string(ident2[j + 1]);
	for (vector<string>::iterator it = clients[index].channels.begin(); it != clients[index].channels.end(); it++)
	{
		if (*it == channel)
		{
			clients[index].channels.erase(it);
			string message = ":BOT!BOT@irc.server PRIVMSG " + clients[index].nickname + " :YOUR LEFT CHANNEL " + channel + endl;
			send(index, message.c_str(), message.size(), 0);
			return;
		}
	}
	string error = ":irc.serv 442 "  + channel + " :You're not on that channel\n";
}

static void setOper(Client *clients, int &index, char **ident2, int &j)
{
	if (!ident2[j + 1] || !ident2[j + 2])
	{
		string error = ":irc.serv 461 OPER :Not enough parameters\n";
		send(index, error.c_str(), error.size(), 0);
		return ;
	}
	if ((string(ident2[j + 1]) != "ADMIN") || (string(ident2[j + 2]) != "IRCSERV"))
	{
		string error = ":irc.serv 464 :Password incorrect\n";
		send(index, error.c_str(), error.size(), 0);
		return ;
	}
	clients[index].op = true;
	string message = ":irc.serv 381 :You are now an IRC operator\n";
	send(index, message.c_str(), message.size(), 0);
}

static void joinChannel(Client *clients, int &index, char **ident2, int &j)
{
	if (!ident2[j + 1])
		return ;
	if (ident2[j + 1][0] != '#')
		return;
	for (size_t i = 0; i < clients[index].channels.size(); i++)
	{
		if (clients[index].channels[i] == string(ident2[j + 1]))
		{
			string message = ":irc.server 443 " + clients[index].username + " " + string(ident2[j + 1]) + " :is already on channel\n";
			send(index, message.c_str(), message.size(), 0);
			return;
		}
	}
	clients[index].channels.push_back(ident2[j + 1]);
	cout << "added: " << ident2[j + 1] << endl;
}


/*******************************UNKNOWN COMMAND********************************/

static void unknownCommand(int &index, string &cmd)
{
	string message = ":irc.serv 421 " + cmd + " :Unknown command\n";
	send(index, message.c_str(), message.size(), 0);
}

/*******************************MESSAGE********************************/

static void channelMessage(Client *clients, int &index, char **ident2, int &j)
{
	string channel = string(ident2[j + 1]);

	if (ident2[j + 2] && string(ident2[j + 2]) == "PART")
		channelPart(clients, index, ident2, j);
	else
	{
		for (size_t i = 0; i < clients[index].channels.size(); i++)
		{
			if (clients[index].channels[i] == channel)
			{
				string message = ":" + clients[index].nickname + "!" + clients[index].username + "@irc.server" + " PRIVMSG " + channel;
				int k = 1;
				while (ident2[++k + j])
				{
					message += " ";
					message += ident2[k + j];
				}
				message += "\n";
				for (int l = 0; l < 1024; l++)
				{
					for (size_t m = 0; m < clients[l].channels.size(); m++)
					{
						if (clients[l].channels[i] == channel && l != index)
							send(l, message.c_str(), message.size(), 0);
					}
				}
				return;
			}
		}
		string error = ":irc.serv 442 " + channel + " :You're not on that channel\n";
		send(index, error.c_str(), error.size(), 0);
	}
}

static void personnalMessage(Client *clients, int &index, char **ident2, int &j)
{
	string nick = string(ident2[j + 1]);
	int d = searchNick(clients, nick);
	if (d == -1)
	{
		string error = ":irc.serv 401 " + nick + " :No such user\n";
		send(index, error.c_str(), error.size(), 0);
	}
	else
	{
		string message = ":" + clients[index].nickname + "!" + clients[index].username + "@irc.server" + " PRIVMSG " + nick;
		int i = 1;
		while (ident2[++i + j])
		{
			message += " ";
			message += ident2[i + j];
		}
		message	+= "\n";
		send(d, message.c_str(), message.size(), 0);
	}
}

static void privateMsg(Client *clients, int &index, char **ident2, int &j)
{
	cout << "privateMessage\n";
	int d;
	if (ident2[j + 1] && ident2[j + 2])
	{
		if (string(ident2[j + 1]) == "BOT")
		{
			 botCommand(clients, index, ident2, j);
			 return;
		}
		if (ident2[j + 1][0] == '#')
			channelMessage(clients, index, ident2, j);
		else
			personnalMessage(clients, index, ident2, j);
	}
	else
	{
		string error = ":irc.serv 401   :No such nick\n";
		send(index, error.c_str(), error.size(), 0);
	}
}

/***********************************SETPASS************************************/

static void setPass(Client *clients, int &index, char **ident2, int &j, string &password)
{
	if(ident2[j + 1] && string(ident2[j + 1]) == password)
	{
		clients[index].password = true;
		isConnected(clients[index], index);
	}
	else
	{
		string message = ":irc.serv 464 :Password incorrect\n";
		send(index, message.c_str(), message.size(), 0);
	}
}

/***********************************SETNICK************************************/

static void setNick(Client *clients, int &index, char **ident2, int &j)
{
	if (ident2[j + 1] && notEmpty(ident2[j + 1], 0))
	{
		if (newNick(ident2[j + 1], clients))
		{
			string nick = string(ident2[j + 1]);
			clients[index].nickname = nick;
			isConnected(clients[index], index);
		}
		else
		{
			string error = ":irc.serv 433 " + string(ident2[j + 1]) + " :Nickname is already in use\n";
			send(index, error.c_str(), error.size(), 0);
		}
	}
	else
	{
		string error = ":irc.serv 431 :No nickname given\n";
		send(index, error.c_str(), error.size(), 0);
	}
}

/***********************************SETUSER************************************/

static void setUser(Client *clients, int &index, char **ident2, int &j)
{
	if (ident2[j + 1] && notEmpty(ident2[j + 1], 1))
	{
		string user = string(ident2[j + 1]);
		clients[index].username = user;
		isConnected(clients[index], index);
	}
}

/***********************************COMMANDS***********************************/

void ft_commands(Client *clients, int &index, char *bufRead, string &password)
{
	int i = -1;
	int j = -1;
	cout << "Command\n";
	cout << bufRead << endl;
//	clients[s].id = s;
	char **ident = ft_split2(bufRead, "\r\n");
	 while(ident && ident[++i])
	 {
		 j = -1;
		 char **ident2 = ft_split2(ident[i], "\t ");
		 while(ident2 && ident2[++j])
		 {
			 string cmd = string(ident2[j]);
			if (cmd == "NICK")
				setNick(clients, index, ident2, j);
			else if (cmd == "USER")
				setUser(clients, index, ident2, j);
			else if (cmd == "PRIVMSG")
				privateMsg(clients, index, ident2, j);
			else if (cmd == "CLIENTS")
				ft_clients(clients);
			else if (cmd == "PASS")
				setPass(clients, index, ident2, j, password);
			else if (cmd == "JOIN")
				joinChannel(clients, index, ident2, j);
			else if (cmd == "OPER")
				setOper(clients, index, ident2, j);
			else if (cmd == "PART")
				channelPart(clients, index, ident2, j);
			// else if (word == "KICK")
			else if (cmd == "CLIENTS")
			 	ft_clients(clients);
			// }
			else
			{
				if (isUpper(cmd))
					unknownCommand(index, cmd);
			}
	 	}
		free(ident2);
		free(ident[i]);
	}
	free (ident);
}
