/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/13 15:22:25 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/15 10:19:37 by dhaliti          ###   ########.fr       */
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

int main(int ac, char **av)
{
    if (ac != 3)
	{
        cerr << "Wrong number of arguments\n";
		cerr << "Usage: ./ircserv port password\n";
        return (1);
    }

	Client clients[1024];
    int port = atoi(av[1]);
	string password = string(av[2]);

    bzero(&clients, sizeof(clients));
    FD_ZERO(&active);

    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0)
        fatal_error();

    _max = serverSock;
    FD_SET(serverSock, &active);

    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(2130706433);
    addr.sin_port = htons(port);

	fcntl(serverSock, F_SETFL, O_NONBLOCK);

    if ((bind(serverSock, (const struct sockaddr *)&addr, sizeof(addr))) < 0)
        fatal_error();
    if (listen(serverSock, 128) < 0)
        fatal_error();

	std::string tempString = "";

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
                // sprintf(bufWrite, "server: client %d just arrived\n", clients[clientSock].id);
                // send_all(clientSock);
                break ;
            }

            if (FD_ISSET(index, &readyRead) && index != serverSock)
			{
                int res = recv(index, bufRead, 1024, 0);
				
				while (res > 0 && res != 1024 && bufRead[res - 1] != 10){
					
					bufRead[res] = 10;
					tempString += string(bufRead);
					std::cout << "TempString is : " << tempString << std::endl;
					res = recv(index, bufRead, 1024, 0);
				} 

				// std::cout << "The size of RES is : "<< res << std::endl;
				// for (int i = 0; i < res; i++)
				// {
				// 	std::cout << (int)bufRead[i] << std::endl;
				// }
				
				//bufRead[strlen(bufRead)] = 0;
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
					if (clients[index].id == index)
					{
						if (tempString.size() != 0)
						{
							tempString += bufRead;
							strcpy(bufRead, tempString.c_str());
							tempString = "";
						}
						ft_commands(clients, index, bufRead, password);
					}
					else
						newClient(clients, index);
                    break;
                }
            }
        }
    }
}
