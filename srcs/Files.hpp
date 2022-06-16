/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Files.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaliti <dhaliti@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 18:11:08 by dhaliti           #+#    #+#             */
/*   Updated: 2022/06/16 18:22:05 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "IRC.hpp"

class File
{
	private:
		string	_name;
		string	_content;
		string	_sender;
		string	_dest;

	public:
		File(const string&, const string&, const string&, const string&);
		~File();

		string	getName(void);
		string	getContent(void);
		string	getSender(void);
		string	getDest(void);
};
