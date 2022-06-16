/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cciobanu <cciobanu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/16 14:58:28 by cciobanu          #+#    #+#             */
/*   Updated: 2022/06/16 18:21:19 by dhaliti          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(void) : password(false), id(0), op(false), nickname(""), username(""), connected(false) {
}

Client::Client(const Client &other){
	*this = other;};

Client::~Client(void) {};

Client & Client::operator=(const Client &other)
{
	this->id = other.id;
	this->op = other.op;
	this->nickname = other.nickname;
	this->username = other.username;
	this->channels = other.channels;
	return *this;
};

int				Client::getId(void) const {
	return (this -> id);
}
bool			Client::isOp(void) const {
	return (this -> op);
}
bool			Client::isConnected(void) const {
	return (this -> connected);
}
string			Client::getNickName(void) const {
	return (this -> nickname);
}
string			Client::getUserName(void) const {
	return (this -> username);
}

void			Client::setOp(bool isOp){
	this -> op = isOp;
}

void			Client::setId(int id){
	this -> id = id;
}

void			Client::setConnected(bool conn){
	this -> connected = conn;
}

void			Client::setNickName(string name){
	this -> nickname = name;
}

void			Client::setUserName(string name){
	this -> username = name;
}
