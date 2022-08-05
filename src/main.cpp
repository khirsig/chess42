/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 19:25:28 by khirsig           #+#    #+#             */
/*   Updated: 2022/08/05 15:10:53 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Data.hpp"

void turnDebugOn(Data &data) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        int size = SCREEN_WIDTH / 8;
        int x = GetMouseX();
        int y = GetMouseY();

        x /= size;
        y /= size;
        if (data.chessBoard.square[y][x].debug == true)
            data.chessBoard.square[y][x].debug = false;
        else
            data.chessBoard.square[y][x].debug = true;
    }
}

int main() {
    Data data;

    InitWindow(SCREEN_WIDTH * 1.03, SCREEN_HEIGHT, "Chess 42");
    SetTargetFPS(120);
    initTex(data);
    initPieces(data);
    while (!WindowShouldClose()) {
        // CALCULATIONS
        if (data.currentBarDepth < data.nextBarDepth) runEvalBar(data, data.chessBoard);
        // ACTIONS
        turnDebugOn(data);
        grabPiece(data);
        placePiece(data, WHITE_P);
        // if (data.turn == WHITE_P && data.chessBoard.checkmate == false)
        // {
        // 	if (data.aiThinking == false)
        // 		getBestAIMove(data, data.chessBoard, WHITE_P);
        // 	executeAIMove(data, data.chessBoard, WHITE_P);
        // }
        if (data.turn == BLACK_P && data.chessBoard.checkmate == false) {
            if (data.aiThinking == false) getBestAIMove(data, data.chessBoard, BLACK_P);
            executeAIMove(data, data.chessBoard, BLACK_P);
        }
        // START DRAWING
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawBoard(data);
        drawAllPieces(data);
        drawDebugInfo(data);
        drawEvalBar(data, data.chessBoard);
        // END DRAWING
        EndDrawing();
    }
    CloseWindow();
}
