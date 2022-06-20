/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connect.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 11:21:17 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/20 11:49:31 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

/***********************************SETPASS************************************/

void setPass(Client *clients, int &index, char **ident2, int &j, string &password)
{
	if (clients[index].password)
		sendAll(index, ":irc.serv 462 :You may not reregister\n");
	else if(ident2[j + 1] && string(ident2[j + 1]) == password)
	{
		clients[index].password = true;
		isConnected(clients[index], index);
	}
	else
		sendAll(index, ":irc.serv 464 :Password incorrect\n");
}

/***********************************SETNICK************************************/

void setNick(Client *clients, int &index, char **ident2, int &j)
{
	if (ident2[j + 1] && notEmpty(ident2[j + 1], 0))
	{
		if (newNick(ident2[j + 1], clients) && string(ident2[j + 1]) != clients[index].getNickName())
		{
			string nick = string(ident2[j + 1]);
			clients[index].setNickName(nick);
			cout << GRN << "Client #" << clients[index].getId() - 4 << " nickname is now " << clients[index].getNickName() << endl << END;
			isConnected(clients[index], index);
		}
		else
		{
			string error = ":irc.serv 433 " + string(ident2[j + 1]) + " :Nickname is already in use\n";
			sendAll(index, error);
		}
	}
	else
	{
		string error = ":irc.serv 431 :No nickname given\n";
		sendAll(index, error);
	}
}

/***********************************SETUSER************************************/

void setUser(Client *clients, int &index, char **ident2, int &j)
{
	if (!ident2[j + 1] || !ident2[j + 2] || !ident2[j + 3] || !ident2[j + 4])
	{
		sendAll(index, ":irc.serv 461 USER :Not enough parameters\n");
		return ;
	}
	if (!clients[index].getUserName().empty())
		sendAll(index, ":irc.serv 462 :You may not reregister\n");
	else if (ident2[j + 1] && notEmpty(ident2[j + 1], 1))
	{
		string user = string(ident2[j + 1]);
		clients[index].setUserName(user);
		isConnected(clients[index], index);
	}
}

/*********************************JOIN CHANNEL*********************************/

void joinChannel(Client *clients, int &index, char **ident2, int &j)
{
	if (clients[index].isConnected() == false)
	{
		sendAll(index, ":irc.serv 444 user :User not logged in\n");
		return ;
	}
	if (!ident2[j + 1])
	{
		sendAll(index, ":irc.serv 461 " + string(ident2[j]) + " :Not enough parameters\n");
		return ;
	}
	string hashtag = string(ident2[j + 1]);
	if (ident2[j + 1][0] != '#')
		hashtag = "#" + hashtag;
	for (size_t i = 0; i < clients[index].channels.size(); i++)
	{
		if (clients[index].channels[i] == string(ident2[j + 1]))
		{
			string message = ":irc.server 443 " + string(":") + clients[index].getUserName() + " " + hashtag + " :is already on channel\n";
			sendAll(index, message);
			return;
		}
	}
	clients[index].channels.push_back(hashtag);
	sendAll(index, ":BOT!BOT@irc.server PRIVMSG " + hashtag + " :You successfully joined channel" + hashtag + "\n");
}

/*************************************PING*************************************/

void pingPong(Client*, int &index, char **ident2, int &j)
{
	string message = "PONG ";
	if (ident2[j + 1] && !ident2[j + 2])
		message += ident2[j + 1];
	else if (ident2[j + 2])
		message += ident2[j + 2];
	message += "\r\n";
	sendAll(index, message);
}
