/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Commands.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 16:25:05 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/17 11:00:53 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

static vector<File> files;

static void quit(Client *clients, int &index, fd_set & active)
{
	cout << RED << "Client #" << clients[index].getId() - 4 << " just left!\n" << END;
	clients[index].setId(-1);
	clients[index].setNickName("");
	clients[index].setUserName("");
	clients[index].password = false;
	clients[index].channels.clear();
	clients[index].setConnected(false);
	FD_CLR(index, &active);
	close(index);
}

static void getFile(Client *clients, int &index, char **ident2, int &j)
{
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

static void sendFile(Client *clients, int &index, char **ident2, int &j)
{
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


static void pingPong(Client*, int &index, char **ident2, int &j)
{
	string message = "PONG ";
	if (ident2[j + 1] && !ident2[j + 2])
		message += ident2[j + 1];
	else if (ident2[j + 2])
		message += ident2[j + 2];
	message += "\r\n";
	sendAll(index, message);
}

/******************************KICK FROM CHANNEL*******************************/

static void channelKick(Client *clients, int &index, char **ident2, int &j)
{
	if (!ident2[j + 1] || !ident2[j + 2])
	{
		cout << ident2[j + 1] << " " << ident2[j + 2] << " " << ident2[j + 3] << endl;
		string error = ":irc.serv 461 " + string(ident2[j]) + " :Invalid number of parameters\n";
		sendAll(index, error);
		return ;
	}
	string user = string(ident2[j + 2]);
	string channel = string(ident2[j + 1]);
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
			if (clients[index].isOp() == true)
			{
				for (vector<string>::iterator it = clients[d].channels.begin(); it != clients[d].channels.end(); it++)
				{
					if (*it == channel)
					{
						clients[d].channels.erase(it);
						string message = ":BOT!BOT@irc.server PRIVMSG " + channel + " :You have been kicked from the channel by " + clients[index].getNickName() + " with the following message -";
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
			else
			{
				sendAll(index, ":irc.serv 481 :Permission Denied- You're not an IRC operator.\n");
				return;
			}
		}
	}
	sendAll(index,  string(":irc.serv 442 "  + string(ident2[j + 1]) + " :You're not on that channel\n"));
}

/******************************PART FROM CHANNEL*******************************/

static void channelPart(Client *clients, int &index, char **ident2, int &j)
{
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

/*********************************SET OPERATOR*********************************/

static void setOper(Client *clients, int &index, char **ident2, int &j)
{
	if (!ident2[j + 1] || !ident2[j + 2])
	{
		sendAll(index, ":irc.serv 461 OPER :Not enough parameters\n");
		return ;
	}
	if ((string(ident2[j + 1]) != "admin") || (string(ident2[j + 2]) != "admin"))
	{
		sendAll(index, ":irc.serv 464 :Password incorrect\n");
		return ;
	}
	clients[index].setOp(true);
	cout << BLU << clients[index].getNickName() << " is now an Operator\n" << END;
	sendAll(index, ":irc.serv 381 :You are now an IRC operator\n");
}

/*********************************JOIN CHANNEL*********************************/

static void joinChannel(Client *clients, int &index, char **ident2, int &j)
{
	if (clients[index].isConnected() == false)
	{
		sendAll(index, ":irc.serv 444 " + clients[index].getUserName() + ":User not logged in\n");
		return;
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


/*******************************UNKNOWN COMMAND********************************/

static void unknownCommand(int &index, string &cmd)
{
	string message = ":irc.serv 421 " + cmd + " :Unknown command\n";
	sendAll(index, message);
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

static void personnalMessage(Client *clients, int &index, char **ident2, int &j)
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

static void privateMsg(Client *clients, int &index, char **ident2, int &j)
{
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

/***********************************SETPASS************************************/

static void setPass(Client *clients, int &index, char **ident2, int &j, string &password)
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

static void setNick(Client *clients, int &index, char **ident2, int &j)
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

static void setUser(Client *clients, int &index, char **ident2, int &j)
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
	cmds["NOTICE"] = &privateMsg;
	cmds["PRIVMSG"] = &privateMsg;
	cmds["CLIENTS"] = &ft_clients;
	cmds["SENDFILE"] = &sendFile;
	cmds["GETFILES"] = &getFile;

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
