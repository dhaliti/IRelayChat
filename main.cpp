/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 15:22:25 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/16 11:04:29 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include "Client.hpp"
#include "IRC.hpp"
#include <fcntl.h>
using namespace std;

fd_set active;

void force_quit(int)
{
	cout << endl << "Force quiting.." << endl;
	for (size_t i = 0; i < 1024; i++) {
		FD_CLR(i, &active);
		close(i);
	}
	exit (0);
}

static void Socketting(int &serverSock, int &_max)
{
	serverSock = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSock < 0)
		throw out_of_range("Socket failed");
	if (fcntl(serverSock, F_SETFL, O_NONBLOCK) == -1)
		throw out_of_range("Non-blocking Socket failed");
	_max = serverSock;
	FD_SET(serverSock, &active);
}

static void Binding(int &serverSock, sockaddr_in &addr, int &port)
{
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(2130706433);
	addr.sin_port = htons(port);
	fcntl(serverSock, F_SETFL, O_NONBLOCK);
	if ((bind(serverSock, (const struct sockaddr *)&addr, sizeof(addr))) < 0)
	   throw out_of_range("Binding failed");
	if (listen(serverSock, 128) < 0)
	   throw out_of_range("Listening failed");
}

static void checkArgs(Client *clients, fd_set &active, int &ac, char **av, int &port, string &password)
{
	if (ac != 3)
        throw invalid_argument("Wrong number of arguments\nUsage: ./ircserv port password");
	port = atoi(av[1]);
	password = string(av[2]);
	if (port <= 0)
		throw out_of_range("Port cannot be negative !");
	bzero(&clients, sizeof(clients));
	FD_ZERO(&active);
}

int main(int ac, char **av)
{
	int _max = 0;
	int next_id = 0;
	fd_set readyRead;
	fd_set readyWrite;
	char bufRead[1024];
	Client clients[1024];
	int port;
	string password;
	int serverSock;
	struct sockaddr_in addr;
	socklen_t addr_len = sizeof(addr);

	try
	{
		checkArgs(clients, active, ac, av, port, password);
		Socketting(serverSock, _max);
		Binding(serverSock, addr, port);
		signal(SIGINT, force_quit);
		IRCLoop(clients, readyRead, readyWrite, active, _max, serverSock, next_id, bufRead, password, addr_len, addr);
	}

	catch(exception &e)
	{
		cout << e.what() << endl;
		exit(1);
	}
}
