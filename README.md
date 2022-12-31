
# IRelayChat
This project is a simple and lightweight implementation of an IRC server written in C++98. It is fully compatible with an official client.
The IRC Server supports basic IRC functionality, including the ability to join and leave channels, send and receive messages, and handle user commands.

<img src="https://user-images.githubusercontent.com/105823790/210153358-5cb550f0-a069-462b-aac5-d926004bcc10.svg" width=300>

## Getting started
To get started, simply clone this repository and use the ```make``` command to build the executable.
```
make
```
Then run it on an available port and choose a password to protect it:
```
./ircserv <port> <password>
```
### Notes
We recommend using [Textual](https://www.codeux.com/textual/) to connect to the server.

## Features
<ul>
<li><b>Channel support:</b> The IRC Server supports the creation and management of channels, allowing users to join and leave channels as well as send messages to all users in a channel. Channel administrators can set modes on channels and manage the membership list.</li>

<li><b>User commands:</b> The IRC Server supports a number of user commands, including <i>/join</i> for joining a channel and <i>/leave</i> for leaving a channel.</li>

<li><b>Message handling:</b> The IRC Server can send and receive messages between users and channels.</li>

<li>
<b>Administrator rights:</b> The IRC Server supports the concept of administrator rights, allowing designated users to administrate channels and perform certain actions such as banning users.</li>
</ul>

## Ressources
[IRC protocol specifications](https://tools.ietf.org/html/rfc1459)

[C++98 standard](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/1998/n1385.pdf)

[POSIX socket API](https://www.mankier.com/2/socket)

[The Berkeley sockets API](https://www.mankier.com/2/socket)

## Licence
IRC-Vintage is released under the MIT License.

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
