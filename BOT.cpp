/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BOT.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 11:27:43 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/14 14:33:50 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

static void botUNKNOWN(Client *clients, int &index, char *cmd)
{
	string message = ":BOT!BOT@irc.server PRIVMSG " + clients[index].nickname + cmd + " -- Unknown command.\n";
	send(index, message.c_str(), message.size(), 0);
}

static void botOPER(Client *clients, int &index)
{
   string message = ":BOT!BOT@irc.server PRIVMSG " + clients[index].nickname + " : OPER message is used by a normal user to obtain operator privileges. The combination of <user> and <password> are required to gain Operator privileges.\n";
   send(index, message.c_str(), message.size(), 0);
}

static void botKICK(Client *clients, int &index)
{
	string message = ":BOT!BOT@irc.server PRIVMSG " + clients[index].nickname + " : The KICK command can be  used  to  forcibly  remove  a  user  from  a channel.  It  'kicks  them  out'  of the channel (forced PART). Only a channel operator may kick another user out of a  channel.\n";
	send(index, message.c_str(), message.size(), 0);
}

static void botJOIN(Client *clients, int &index)
{
	string message = ":BOT!BOT@irc.server PRIVMSG " + clients[index].nickname + " : The JOIN command is used by client to start listening a specific channel. Whether or not a client is allowed to join a channel is checked only by the server the client is connected to; all other servers automatically add the user to the channel when it is received from other servers.\n";
	send(index, message.c_str(), message.size(), 0);
}

static void botPART(Client *clients, int &index)
{
	string message = ":BOT!BOT@irc.server PRIVMSG " + clients[index].nickname + " : The PART message causes the client sending the message to be removed from the list of active users for all given channels listed in the parameter string.\n";
	send(index, message.c_str(), message.size(), 0);
}

static void botQUIT(Client *clients, int &index)
{
	string message = ":BOT!BOT@irc.server PRIVMSG " + clients[index].nickname + " : A client session is ended with a quit message.  The server must close the connection to a client which sends a QUIT message. If a 'Quit Message' is given, this will be sent instead of the default message, the nickname.\n";
	send(index, message.c_str(), message.size(), 0);
}

static void botPASS(Client *clients, int &index)
{
	string message = ":BOT!BOT@irc.server PRIVMSG " + clients[index].nickname + " : The PASS command is used to set a 'connection password'.The password can and must be set before any attempt to register the connection is made.\n";
	send(index, message.c_str(), message.size(), 0);
}

static void botUSER(Client *clients, int &index)
{
	string message = ":BOT!BOT@irc.server PRIVMSG " + clients[index].nickname + " : The USER message is used at the beginning of connection to specify the username, hostname, servername and realname of s new user.\n";
	send(index, message.c_str(), message.size(), 0);
}


static void botNICK(Client *clients, int &index)
{
	string message = ":BOT!BOT@irc.server PRIVMSG " + clients[index].nickname + " :Nick message is used to give user a nickname or change the previous one.\n";
	send(index, message.c_str(), message.size(), 0);
}


static void botCMD(Client *clients, int &index)
{
	string message = ":BOT!BOT@irc.server PRIVMSG " + clients[index].nickname + " :pass • nick • user • join • quit • part • oper •  kick\n";
	send(index, message.c_str(), message.size(), 0);
	string message2 = ":BOT!BOT@irc.server PRIVMSG " + clients[index].nickname + " :Type the name of a command to get more info\n";
	send(index, message2.c_str(), message2.size(), 0);
}


void botCommand(Client *clients, int &index, char **ident2, int &j)
{
	if (string(ident2[j + 2]) == ":cmd")
		botCMD(clients, index);
	else if (string(ident2[j + 2]) == ":pass") //
		botPASS(clients, index);
	else if (string(ident2[j + 2]) == ":nick") //
		botNICK(clients, index);
	else if (string(ident2[j + 2]) == ":user") //
		botUSER(clients, index);
	else if (string(ident2[j + 2]) == ":quit") //
		botQUIT(clients, index);
	else if (string(ident2[j + 2]) == ":part") //
		botPART(clients, index);
	else if (string(ident2[j + 2]) == ":kick") //
		botKICK(clients, index);
	else if (string(ident2[j + 2]) == ":oper") //
		botOPER(clients, index);
	else if (string(ident2[j + 2]) == ":join") //
		botJOIN(clients, index);
	else
		botUNKNOWN(clients, index, ident2[j +2]); //
}
