# Minecraft Clone

This is a clone of minecraft in C++ with openGL.

I have written this only for purpose of exploring how to write a 3D mincraft like game.
That is why there's a lot of prototype-ish code, with lot of copy/move semantics bugs.

Infinite world gen works but I had some errors due to the move/copy semantics that results
in game crashing so it is using `std::map` rn, and is a bit less performant.

I have not implemented lots of game mechanics, simply for the reason that I wasn't interested. Also I've only tested it on M1 mac :p

Anyhow, here's a small screenshot of what it looks like

![Image](https://media.discordapp.net/attachments/931126671250165783/1157647520311283823/image.png?ex=65195ec4&is=65180d44&hm=0e4d4fe402f1322ce163d149fad878df733c33cbafc0247a0dd5364f35a628f3&=&width=540&height=556)
