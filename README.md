Hello everyone. This was a personal project of mine where I tried to create my own chess game.
This was supposed to be a ~3 days project, until I wanted to create my own simple chess engine on top.

Moving all the pieces to the according rules currently works, except en passent.
![chessGif1](https://user-images.githubusercontent.com/78398628/181710348-c59e62c7-b633-49b9-a274-96aa14df6700.gif)

The AI responds with the move it calculates best. It does so with a combination of piece values and square value for each piece type it currently stands on. It calculates the board for each possible move into depth 6 or whatever you put in. However, after depth 6 it takes quite a while longer until too long for enjoyable gameplay.
---
