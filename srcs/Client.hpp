/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 13:39:09 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/16 18:15:15 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "IRC.hpp"
#include <vector>

using namespace std;

class Client
{
	public:
		Client();

		~Client();

		Client(const Client &other);

		Client & operator=(const Client &other);

		bool			password;
		vector<string>	channels;

		int				getId(void) const;
		void			setId(int id);
		bool			isOp(void) const;
		void			setOp(bool isOp);
		bool			isConnected(void) const;
		void			setConnected(bool conn);
		string			getNickName(void) const;
		void			setNickName(string name);
		string			getUserName(void) const;
		void			setUserName(string name);

	private:
		int				id;
		bool			op;
		string			nickname;
		string			username;
		bool 			connected;

};
