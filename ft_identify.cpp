/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_identify.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 16:25:05 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/10 17:24:06 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "IRC.hpp"
#include <sys/socket.h>

void ft_identify(Client *clients, int &s, char *bufRead)
{
	int i = -1;
	int j = -1;
	cout << bufRead << endl;
	clients[s].id = s;
	char **ident = ft_split2(bufRead, "\n");
	 while(ident && ident[++i])
	 {
		 j = -1;
		 char **ident2 = ft_split2(ident[i], "\t ");
		 while(ident2 && ident2[++j])
		 {
			if (string(ident2[j]) == "NICK")
			{
				if (ident2[j + 1])
					clients[s].nickname = string(ident2[j + 1]);
				else
				{
					string error = "Wrong nickname parsing. Default nickname will be guest" + to_string(s) + "\n";
					send(s, error.c_str(), error.size(), 0);
					clients[s].nickname = "Guest" + to_string(s);
				}
			}
			if (string(ident2[j]) == "USER")
			{
				if (ident2[j + 1] && string(ident2[j]) != "anonymous")
					clients[s].username = string(ident2[j + 1]);
				else
				{
					string error = "Wrong username parsing. Default username will be user" + to_string(s) + "\n";
					send(s, error.c_str(), error.size(), 0);
					clients[s].username = "User" + to_string(s);
				}
			}
			free(ident2[j]);
	 	}
		free(ident[i]);
	}
}
