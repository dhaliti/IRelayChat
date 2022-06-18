/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newClient.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 11:09:59 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/17 11:40:21 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void botWelcome(Client &client, int &index)
{
	string message = ":BOT!BOT@irc.server PRIVMSG " + client.getNickName() + " :Welcome to this IRC Server. I am the BOT assistant here to help you. To show the list of commands, simply type <cmd> in this chat window\n";
	send(index, message.c_str(), message.size(), 0);
}


void newClient(Client *clients, int &index)
{
	clients[index].setId(index);
	cout <<  "Client #" << clients[index].getId() - 4 << " just arrived\n";
}