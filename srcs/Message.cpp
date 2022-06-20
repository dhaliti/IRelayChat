/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Message.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 11:24:15 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/20 11:49:27 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

/*******************************MESSAGE********************************/

void channelMessage(Client *clients, int &index, char **ident2, int &j)
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
				string message = ":" + clients[index].getNickName() + "!" + clients[index].getUserName() + "@irc.server" + " PRIVMSG " + channel;
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
						if (clients[l].channels[m] == channel && l != index)
							send(l, message.c_str(), message.size(), 0);
					}
				}
				return;
			}
		}
		string error = ":irc.serv 442 " + channel + " :You're not on that channel\n";
		sendAll(index, error);
	}
}

void personnalMessage(Client *clients, int &index, char **ident2, int &j)
{
	string nick = string(ident2[j + 1]);
	int d = searchNick(clients, nick);
	if (d == -1)
	{
		string error = ":irc.serv 401 " + nick + " :No such user\n";
		sendAll(index, error);
	}
	else
	{
		string message = ":" + clients[index].getNickName() + "!" + clients[index].getUserName() + "@irc.server" + " PRIVMSG " + nick;
		int i = 1;
		while (ident2[++i + j])
		{
			message += " ";
			message += ident2[i + j];
		}
		message	+= "\n";
		sendAll(d, message);
	}
}

void privateMsg(Client *clients, int &index, char **ident2, int &j)
{
	if (clients[index].isConnected() == false)
	{
		sendAll(index, ":irc.serv 444 user :User not logged in\n");
		return ;
	}
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
		sendAll(index, ":irc.serv 401  :No such nick\n");
}
