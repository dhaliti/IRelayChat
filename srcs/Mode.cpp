/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 11:28:50 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/20 11:49:37 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/IRC.hpp"

/*************************************MODE*************************************/

void setMode(Client *clients, int &index, char **ident2, int &j)
{
	if (clients[index].isConnected() == false)
	{
		cout << "notlogged\n";
		sendAll(index, ":irc.serv 444 user :User not logged in\n");
		return;
	}
	if (!ident2[j + 1] || !ident2[j + 2])
	{
		sendAll(index, ":irc.serv 461 MODE :Not enough parameters\n");
		return;
	}
	if (clients[index].isOp() == false)
	{
		cout << "notOP\n";
		sendAll(index, ":irc.serv 481 :Permission Denied- You're not an IRC operator.\n");
		return;
	}
	string op = ident2[j + 2];
	if (op.size() != 2 || (op[0] != '+' && op[0] != '-') || op[1] != 'o')
	{
		cout << "unknown char\n";
		sendAll(index, ":irc.serv 472  :is unknown mode char to me");
		return;
	}
	string nick = string(ident2[j + 1]);
	int d;
	d = searchNick(clients, nick);
	if (d == -1)
	{
		cout << "noSuchUser\n";
		sendAll(index, ":irc.serv 401 " + nick + " :No such user\n");
		return ;
	}
	if (op[0] == '+')
	{
		if (clients[d].isOp() == false)
		{
			clients[d].setOp(true);
			sendAll(d, ":irc.serv 381 :You are now an IRC operator\n");
			cout << BLU << clients[d].getNickName() << " is now an operator" << END << endl;
			return;
		}
	}
	else
	{
		if (clients[d].isOp() == true)
		{
			clients[d].setOp(false);
			cout << clients[d].getNickName() << " is no longer an Operator\n";
			return;
		}
	}
}

/*********************************SET OPERATOR*********************************/

void setOper(Client *clients, int &index, char **ident2, int &j)
{
	if (clients[index].isConnected() == false)
	{
		sendAll(index, ":irc.serv 444 user :User not logged in\n");
		return ;
	}
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
