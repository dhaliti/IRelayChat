/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cciobanu <cciobanu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 11:46:24 by cciobanu          #+#    #+#             */
/*   Updated: 2022/06/16 18:21:51 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

Bot::Bot(string nickname) : _nickname(nickname){
	_command[":cmd"] = ":BOT!BOT@irc.server PRIVMSG " + this -> _nickname + " :pass • nick • user • join • quit • part • oper •  kick\n" +
		":BOT!BOT@irc.server PRIVMSG " + this -> _nickname + " :Type the name of a command to get more info\n";
	_command[":pass"] = ":BOT!BOT@irc.server PRIVMSG " + this -> _nickname + " : The PASS command is used to set a 'connection password'.The password can and must be set before any attempt to register the connection is made.\n";
	_command[":nick"] = ":BOT!BOT@irc.server PRIVMSG " + this -> _nickname + " : Nick message is used to give user a nickname or change the previous one.\n";
	_command[":user"] = ":BOT!BOT@irc.server PRIVMSG " + this -> _nickname + " : The USER message is used at the beginning of connection to specify the username, hostname, servername and realname of s new user.\n";
	_command[":quit"] = ":BOT!BOT@irc.server PRIVMSG " + this -> _nickname + " : A client session is ended with a quit message.  The server must close the connection to a client which sends a QUIT message. If a 'Quit Message' is given, this will be sent instead of the default message, the nickname.\n";
	_command[":part"] = ":BOT!BOT@irc.server PRIVMSG " + this -> _nickname + " : The PART message causes the client sending the message to be removed from the list of active users for all given channels listed in the parameter string.\n";
	_command[":kick"] = ":BOT!BOT@irc.server PRIVMSG " + this -> _nickname + " : The KICK command can be  used  to  forcibly  remove  a  user  from  a channel.  It  'kicks  them  out'  of the channel (forced PART). Only a channel operator may kick another user out of a  channel.\n";
	_command[":oper"] = ":BOT!BOT@irc.server PRIVMSG " + this -> _nickname + " : OPER message is used by a normal user to obtain operator privileges. The combination of <user> and <password> are required to gain Operator privileges.\n";
	_command[":join"] = ":BOT!BOT@irc.server PRIVMSG " + this -> _nickname + " : The JOIN command is used by client to start listening a specific channel. Whether or not a client is allowed to join a channel is checked only by the server the client is connected to; all other servers automatically add the user to the channel when it is received from other servers.\n";
}

const map<string, string> Bot::getCommands(void){
	return (this -> _command);
}

Bot::~Bot(void){};
