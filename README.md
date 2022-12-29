
# IRC Vintage

This project is a simple and lightweight implementation of an IRC server written in C++98. It is fully compatible with an official client.
The IRC Server supports basic IRC functionality, including the ability to join and leave channels, send and receive messages, and handle user commands.

## Getting started
To get started, simply clone this repository and use the ```make``` command to build the executable.
```
make
```
Then run it on an available port and choose a password to protect it:
```
./ircserv <port> <password>
```
## Notes
We recommend using [Textual](https://www.codeux.com/textual/) to connect to the server.

## Features
<li>Channel support: The IRC Server supports the creation and management of channels, allowing users to join and leave channels as well as send messages to all users in a channel. Channel administrators can set modes on channels and manage the membership list.</li>

<li>User commands: The IRC Server supports a number of user commands, including _/join_ for joining a channel and _/leave_ for leaving a channel. It also supports server </li>

<li>Message handling: The IRC Server can send and receive messages between users and channels.</li>

<li>
Administrator rights: The IRC Server supports the concept of administrator rights, allowing designated users to administrate channels and perform certain actions such as banning users.</li>


## Licence
IRC-Vintage is released under the MIT License.

[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)
