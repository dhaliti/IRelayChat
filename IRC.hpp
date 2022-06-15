/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRC.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flcollar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 16:28:25 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/15 12:51:42 by flcollar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include "Client.hpp"
#include <signal.h>

using namespace std;



char	**ft_split2(const char *str, const char *charset);
void 	ft_commands(Client *clients, int &s, const char *bufRead, string &password);
void 	newClient(Client *clients, int &s, char *bufRead);

/*utils.cpp*/
bool 	isUpper(string &str);
void 	isConnected(Client &client, int &index);
void 	ft_clients(Client *clients);
bool 	notEmpty(char *str, int index);
bool 	newNick(char *str, Client *clients);
int 	searchNick(Client *clients, string &nick);
void 	newClient(Client *clients, int &index);
void 	botWelcome(Client &clients, int &index);
void 	botCommand(Client *clients, int &index, char **ident2, int &j);
int 	searchUser(Client *clients, string &user);
void	sendAll(int &dest, string message);
