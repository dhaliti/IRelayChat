/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Notice.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 11:26:41 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/20 11:49:22 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

/************************************NOTICE************************************/

static void channelNotice(Client *clients, int &index, char **ident2, int &j)
{
	string channel = string(ident2[j + 1]);
	for (size_t i = 0; i < clients[index].channels.size(); i++)
	{
		if (clients[index].channels[i] == channel)
		{
			string message = ":" + clients[index].getNickName() + "!" + clients[index].getUserName() + "@irc.server" + " NOTICE " + channel;
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
}

static void personnalNotice(Client *clients, int &index, char **ident2, int &j)
{
	string nick = string(ident2[j + 1]);
	int d = searchNick(clients, nick);
	if (d == -1)
		return ;
	else
	{
		string message = ":" + clients[index].getNickName() + "!" + clients[index].getUserName() + "@irc.server" + " NOTICE " + nick;
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

void noticeMsg(Client *clients, int &index, char **ident2, int &j)
{
	if (clients[index].isConnected() == false)
	{
		sendAll(index, ":irc.serv 444 user :User not logged in\n");
		return ;
	}
	if (ident2[j + 1] && ident2[j + 2])
	{
		if (ident2[j + 1][0] == '#')
			channelNotice(clients, index, ident2, j);
		else
			personnalNotice(clients, index, ident2, j);
	}
}
