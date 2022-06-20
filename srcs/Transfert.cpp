/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Transfert.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 11:41:03 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/20 11:49:25 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

static vector<File> files;

/***********************************GETFILES***********************************/

void getFile(Client *clients, int &index, char **ident2, int &j)
{
	if (clients[index].isConnected() == false)
	{
		sendAll(index, ":irc.serv 444 user :User not logged in\n");
		return ;
	}
	int hasContent = 0;
	for (size_t i = 0; i < files.size(); i++)
	{
		if (clients[index].getNickName() == files[i].getDest())
		{
			hasContent++;
			mkdir(clients[index].getNickName().c_str(), 0777);
			string path("./" + clients[index].getNickName() + "/" + files[i].getName());
			int fd = open(path.c_str(), O_WRONLY | O_CREAT, 0644);
			if (!fd)
			{
				sendAll(index, ":irc.serv 485 " + string(ident2[j]) + " :Failed to retrieve content\n");
				return;
			}
			ofstream out(path);
			out << files[i].getContent();
			files.erase(next(files.begin(), i--));
			close(fd);
		}
	}
	if (!hasContent)
		sendAll(index, ":irc.serv 484 " + string(ident2[j]) + " :You had no content to retrieve\n");
	else
		sendAll(index, ":BOT!BOT@irc.server PRIVMSG " + clients[index].getNickName() + " :You successfully retrieved your files\n");
}

/***********************************SENDFILE************************************/

void sendFile(Client *clients, int &index, char **ident2, int &j)
{
	if (clients[index].isConnected() == false)
	{
		sendAll(index, ":irc.serv 444 user :User not logged in\n");
		return ;
	}
	if (!ident2[j + 1] || !ident2[j + 2])
	{
		sendAll(index, ":irc.serv 461 " + string(ident2[j]) + " :Not enough parameters\n");
		return;
	}
	string target(ident2[j + 2]);
	int d = searchNick(clients, target);
	if (d == -1)
	{
		sendAll(index, ":irc.serv 401 " + target + " :No such user\n");
		return ;
	}
	int fd = open(ident2[j + 1], O_RDONLY);
	if (fd == -1)
	{
		sendAll(index, ":irc.serv 486 " + target + " :Failed to send content\n");
		return;
	}
	char buf;
	string content;
	while (read(fd, &buf, 1))
		content += buf;
	string name = &ident2[j + 1][string(ident2[j + 1]).rfind('/') + 1];
	files.push_back(File(name, content, clients[index].getNickName(), ident2[j + 2]));
	sendAll(d, ":BOT!BOT@irc.server PRIVMSG " + target + " :" + clients[index].getNickName() + " has sent you a file " + name + "\n");
}
