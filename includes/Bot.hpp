/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 18:11:51 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/16 18:21:54 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "IRC.hpp"
#include <map>

class Bot{
	private:
		string _nickname;
		map<string, string> _command;

	public:
		Bot(string nickname);
		~Bot(void);
		const map<string, string> getCommands(void);
};
