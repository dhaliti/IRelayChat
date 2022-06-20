/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Disconnect.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 11:30:19 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/20 11:49:30 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

/*************************************KILL*************************************/

void killUser(Client *clients, int &index, char **ident2, int &j, fd_set &active)
{
	if (clients[index].isConnected() == false)
	{
		sendAll(index, ":irc.serv 444 user :User not logged in\n");
		return ;
	}
	if (!ident2[j + 1])
	{
		sendAll(index, ":irc.serv 461 KILL :Not enough parameters\n");
		return;
	}
	if (clients[index].isOp() == false)
	{
		sendAll(index, ":irc.serv 481 :Permission Denied- You're not an IRC operator.\n");
		return;
	}
	int d;
	string user = string(ident2[j + 1]);
	if ((d = searchUser(clients, user)) == -1)
	{
		sendAll(index, ":BOT!BOT@irc.serv PRIVMSG " + clients[index].getNickName() + " :User you tried to kick could not be found on the channel\n");
		return;
	}
	cout << RED << "Client #" << clients[d].getId() - 4 << " has been removed from the server by operator " + clients[index].getNickName() + "\n" << END;
	sendAll(d, ":BOT!BOT@irc.serv NOTICE " + clients[d].getNickName() + " :You have been removed from the server by operator " + clients[index].getNickName() + "\n");
	clients[d].setId(-1);
	clients[d].setNickName("");
	clients[d].setUserName("");
	clients[d].password = false;
	clients[d].channels.clear();
	clients[d].setConnected(false);
	clients[d].setOp(false);
	FD_CLR(d, &active);
	close(d);
}

/*************************************QUIT*************************************/

void quit(Client *clients, int &index, fd_set & active)
{
	cout << RED << "Client #" << clients[index].getId() - 4 << " just left!\n" << END;
	clients[index].setId(-1);
	clients[index].setNickName("");
	clients[index].setUserName("");
	clients[index].password = false;
	clients[index].channels.clear();
	clients[index].setConnected(false);
	clients[index].setOp(false);
	FD_CLR(index, &active);
	close(index);
}

/******************************KICK FROM CHANNEL*******************************/

void channelKick(Client *clients, int &index, char **ident2, int &j)
{
	if (clients[index].isConnected() == false)
	{
		sendAll(index, ":irc.serv 444 user :User not logged in\n");
		return ;
	}
	if (!ident2[j + 1] || !ident2[j + 2])
	{
		cout << ident2[j + 1] << " " << ident2[j + 2] << " " << ident2[j + 3] << endl;
		string error = ":irc.serv 461 " + string(ident2[j]) + " :Invalid number of parameters\n";
		sendAll(index, error);
		return ;
	}
	string user = string(ident2[j + 2]);
	string channel = string(ident2[j + 1]);
	if (clients[index].isOp() == false)
	{
		sendAll(index, ":irc.serv 481 :Permission Denied- You're not an IRC operator.\n");
		return;
	}
	int d;
	if ((d = searchUser(clients, user)) == -1)
	{
		sendAll(index, ":BOT!BOT@irc.serv PRIVMSG " + clients[index].getNickName() + " :User you tried to kick could not be found on the channel\n");
		return;
	}
	for (size_t i = 0; i < clients[index].channels.size(); i++)
	{
		if (clients[index].channels[i] == channel)
		{
			for (vector<string>::iterator it = clients[d].channels.begin(); it != clients[d].channels.end(); it++)
			{
				if (*it == channel)
				{
					clients[d].channels.erase(it);
					string message = ":BOT!BOT@irc.server NOTICE " + channel + " :You have been kicked from the channel by " + clients[index].getNickName() + " with the following message -";
					if (ident2[j + 3])
					{
						int k = 2;
						while(ident2[++k + j])
						{
							if (ident2[k + j][0] == ':')
								ident2[k + j][0] = ' ';
							message += ident2[k + j];
							message += " ";
						}
						message += "\n";
						sendAll(d, message);
						return ;
					}
					else
					{
						sendAll(d, message + " Your behavior is not conducive to the desired environment.\n");
						return;
					}
				}
			}
		}
	}
	sendAll(index,  string(":irc.serv 442 "  + string(ident2[j + 1]) + " :You're not on that channel\n"));
}


/******************************PART FROM CHANNEL*******************************/

void channelPart(Client *clients, int &index, char **ident2, int &j)
{
	if (clients[index].isConnected() == false)
	{
		sendAll(index, ":irc.serv 444 user :User not logged in\n");
		return ;
	}
	if (!ident2[j + 1])
	{
		sendAll(index, ":irc.serv 461 PART :Not enough parameters\n");
		return ;
	}
	string channel = string(ident2[j + 1]);
	for (vector<string>::iterator it = clients[index].channels.begin(); it != clients[index].channels.end(); it++)
	{
		if (*it == channel)
		{
			clients[index].channels.erase(it);
			string message = ":BOT!BOT@irc.server PRIVMSG " + channel + " :you left channel" + channel + "\n";
			sendAll(index, message);
			return;
		}
	}
	string error = ":irc.serv 442 "  + channel + " :You're not on that channel\n";
	sendAll(index, error);
}
