/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 15:22:25 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/15 12:08:03 by dhaliti          ###   ########.fr       */
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

int _max = 0;
int next_id = 0;

fd_set active;
fd_set readyRead;
fd_set readyWrite;

char bufRead[1024];
char bufWrite[1024];

void fatal_error()
 {
    cerr << "Fatal error\n";
    exit (1);
}

void send_all(int es)
{
    for (int i = 0; i <= _max; i++)
        if (FD_ISSET(i, &readyWrite) && i != es)
            send(i, bufWrite, strlen(bufWrite), 0);
}

static void checkArgs(int &ac, char **av, int &port, string &password)
{
	if (ac != 3)
	{
        cerr << "Wrong number of arguments\n" << "Usage: ./ircserv port password\n";
		exit(1);
	}
	int port = atoi(av[1]);
	password = string(av[2]);
	if (port <= 0)
	{
		cerr << "Port cannot be negative !\n";
		exit(1);
	}
}

int main(int ac, char **av)
{
	int port;
	string password;
	Client clients[1024];

	check_args(ac, av, port, password);
    bzero(&clients, sizeof(clients));
    FD_ZERO(&active);

    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0)
        fatal_error();
	if (fcntl(serverSock, F_SETFL, O_NONBLOCK) == -1)
		fatal_error();


    _max = serverSock;
    FD_SET(serverSock, &active);

    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(2130706433);
    addr.sin_port = htons(port);

    if ((bind(serverSock, (const struct sockaddr *)&addr, sizeof(addr))) < 0)
        fatal_error();
    if (listen(serverSock, 128) < 0)
        fatal_error();

    while (1)
	{
        readyRead = readyWrite = active;
        if (select(_max + 1, &readyRead, &readyWrite, NULL, NULL) < 0)
            continue ;

        for (int index = 0; index <= _max; index++)
		{
            if (FD_ISSET(index, &readyRead) && index == serverSock)
			{
                int clientSock = accept(serverSock, (struct sockaddr *)&addr, &addr_len);
                if (clientSock < 0)
                    continue ;
                _max = (clientSock > _max) ? clientSock : _max;

                clients[clientSock].id = next_id++;

                FD_SET(clientSock, &active);
                cout << "Client #" << clients[clientSock].id << " just arrived";
                break ;
            }

            if (FD_ISSET(index, &readyRead) && index != serverSock)
			{
				int res;
				string cmd;
				memset(bufRead, '\0', 1024);
				res = recv(index, bufRead, 1024, 0);
				cmd += bufRead;
				if (res <= 0)
				{
					string message = ":irc.serv client " + to_string(index) + " just left!\n";
					for (int i = 0; i < 1024; i++)
						send(i, message.c_str(), message.size(), 0);
					FD_CLR(index, &active);
					clients[index].id = -1;
					clients[index].nickname = "";
					clients[index].username = "";
					close(index);
					break ;
				}
				else
				{
					while (!strstr(bufRead, "\n"))
					{
						memset(bufRead, '\0', 1024);
						res = recv(index, bufRead, 1024, 0);
						cmd += bufRead;
					}
				}
					if (clients[index].id == index)
					{
						const char *cmd2 = cmd.c_str();
						ft_commands(clients, index, cmd2, password);
					}
					else
						newClient(clients, index);
                    break;
            }
        }
    }
}
