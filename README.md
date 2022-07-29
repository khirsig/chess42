# chess42

This was a personal project of mine where I tried to create my own chess game.
It was supposed to be a ~3 days project, until I wanted to create my own simple chess engine on top.

The game engine I used was [raylib](https://github.com/raysan5/raylib) with the [c++ wrapper](https://github.com/RobLoach/raylib-cpp).  
The algorithm I used for the engine to go into depth is NegaMax in combination with alpha/beta pruning to make it more efficient.

Moving all the pieces to the according rules works, except en passent.
![chessGif1](https://user-images.githubusercontent.com/78398628/181710348-c59e62c7-b633-49b9-a274-96aa14df6700.gif)

The AI responds with the move it calculates best. It does so with a combination of piece values and square value for each piece type it currently stands on. It calculates the board for each possible move into depth 6 or whatever you put in. However, after depth 6 it takes quite a while longer until too long for enjoyable gameplay.

![chessGif2](https://user-images.githubusercontent.com/78398628/181713372-9506932e-31e4-4039-bdb5-c64d8789523f.gif)

If a player gets checked, the field below the king turns red to indicate that something needs to be done. The possible moveable squares for the pieces adjust to the check situation, and verify that the new possible position would clear the check.
