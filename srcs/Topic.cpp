/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 11:39:30 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/20 11:49:24 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

/**********************************SET TOPIC***********************************/

void setTopic(Client *clients, int &index, char **ident2, int &j)
{
	if (clients[index].isConnected() == false)
	{
		sendAll(index, ":irc.serv 444 user :User not logged in\n");
		return ;
	}
	if (!ident2[j + 1] || !ident2[j + 2] || ident2[j + 3])
	{
		sendAll(index, ":irc.serv 461 TOPIC :Not enough parameters\n");
		return;
	}
	if (clients[index].isOp() == false)
	{
		sendAll(index, ":irc.serv 481 :Permission Denied- You're not an IRC operator.\n");
		return;
	}
	string channel = string(ident2[j + 1]);
	string name = string(ident2[j + 2]);
	for (vector<string>::iterator it = clients[index].channels.begin(); it != clients[index].channels.end(); it++)
	{
		if (*it == channel)
		{
			for (int i = 0; i < 1024; i++)
			{
				if (clients[i].getId() != -1 && clients[i].getId() != 0)
				sendAll(i, ":" + clients[index].getNickName() + " TOPIC " + channel + " " + name + "\n");
			}
			return;
		}
	}
	sendAll(index,  string(":irc.serv 442 "  + string(ident2[j + 1]) + " :You're not on that channel\n"));
}
