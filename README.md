# chess42

This was a personal project of mine where I tried to create my own chess game.
After the game was done I wanted to build a simple chess engine to play with.

The game engine I used was [raylib](https://github.com/raysan5/raylib) with the [c++ wrapper](https://github.com/RobLoach/raylib-cpp).  
The engine goes into depth with a NegaMax algorithm in combination with alpha/beta pruning to make it more efficient.  
Both the engine and the evaluation bar are using multi-threading for higher efficiency and asynchronous calculation.

Moving all the pieces to the according rules works, except en passent.

## How to Run on MacOS

In order for this to work you need to install the raylib on your system. I did this with brew.  
Go into the terminal of your choice and copy these lines:

```bash
brew install raylib
```
```bash
git clone https://github.com/khirsig/chess42 && cd chess42
```
```bash
make && ./chess42
```
You can change the Screen width, height and the board color inside of the config.hpp.  
All default raylib color defines work.

## Preview

![chessGif1](https://user-images.githubusercontent.com/78398628/181710348-c59e62c7-b633-49b9-a274-96aa14df6700.gif)

The AI responds with the move it calculates best. It does so with a combination of piece values and square value for each piece type it currently stands on. It goes into depth * moves. However, after depth 6 it takes quite a while longer until too long for enjoyable gameplay.

![chessGif2](https://user-images.githubusercontent.com/78398628/181713372-9506932e-31e4-4039-bdb5-c64d8789523f.gif)

If a player gets checked, the field below the king turns red to indicate that something needs to be done. The possible moveable squares for the pieces adjust to the check situation, and verify that the new possible position would clear the check.

![chessGif3](https://user-images.githubusercontent.com/78398628/181715716-e30a52cd-dd8a-4490-ae5c-b66d86970e04.gif)

The evalbar on the right side re-calculates the board after each move and might notice a blunder before you do!
