/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Loop.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 10:36:00 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/16 18:21:32 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRC.hpp"

void IRCLoop(Client *clients, fd_set &readyRead, fd_set &readyWrite, fd_set &active, \
	int &_max, int &serverSock, int &next_id, char *bufRead, string &password, \
	socklen_t &addr_len, sockaddr_in &addr)
{
	while (1)
	{
		(void)next_id;
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
                FD_SET(clientSock, &active);
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
					cout << RED << "Client #" << clients[index].getId() - 4 << " just left!\n" << END;
					FD_CLR(index, &active);
					clients[index].setId(-1);
					clients[index].setNickName("");
					clients[index].setUserName("");
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
					if (clients[index].getId() == index)
					{
						const char *cmd2 = cmd.c_str();
						ft_commands(clients, index, cmd2, password, active);
					}
					else
						newClient(clients, index);
                    break;
            }
        }
    }
}
