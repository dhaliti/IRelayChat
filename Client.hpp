/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 13:39:09 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/16 11:57:32 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Client
{
	public:
		Client()
		{
			id = 0;
			op = false;
			nickname = "";
			msg = "";
			username = "";
			connected = false;
			password = false;
		};

		~Client() {};

		Client(const Client &other) 		{*this = other;};

		Client & operator=(const Client &other)
		{
			this->id = other.id;
			this->op = other.op;
			this->nickname = other.nickname;
			this->msg = other.msg;
			this->username = other.username;
			this->channels = other.channels;
			return *this;
		};

		int				id;
		bool			op;
		string			nickname;
		string 			msg;
		string			username;
		vector<string>	channels;
		bool 			connected;
		bool			password;

	private:
};
