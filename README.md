
# IRC Vintage
[![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)](https://forthebadge.com)

This project is about using C++98 to create a personnal IRC server, fully compatible with an official client.

##Context
IRC (Internet Relay Chat) is a protocol created in 1988 for real-time text messaging between internet-connected computers. It is mainly used for group discussion in chat rooms called “channels”, although it also supports private messaging between users.

This server is fully comatible with [Textual](https://www.codeux.com/textual/)

Launch the server:
```
make
```
Then run it on an available port and choose protected to protect it:
```
./ircserv <port> <password>
```
Finally, connect with your favorite IRC client (Textual ideally) and start chatting!
