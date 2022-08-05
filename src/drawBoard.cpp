/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawBoard.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 10:35:09 by khirsig           #+#    #+#             */
/*   Updated: 2022/08/05 15:10:33 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Data.hpp"

void drawPiece(Data &data, int pieceType, int pieceOwner, raylib::Vector2 pos, float scale) {
    // Looks for the correct piece type and owner and draws the right texture.
    switch (pieceType) {
        case PAWN:
            if (pieceOwner == WHITE_P)
                DrawTextureEx(data.tex[0], pos, 0, scale, WHITE);
            else
                DrawTextureEx(data.tex[1], pos, 0, scale, WHITE);
            break;
        case BISHOP:
            if (pieceOwner == WHITE_P)
                DrawTextureEx(data.tex[2], pos, 0, scale, WHITE);
            else
                DrawTextureEx(data.tex[3], pos, 0, scale, WHITE);
            break;
        case KNIGHT:
            if (pieceOwner == WHITE_P)
                DrawTextureEx(data.tex[4], pos, 0, scale, WHITE);
            else
                DrawTextureEx(data.tex[5], pos, 0, scale, WHITE);
            break;
        case ROOK:
            if (pieceOwner == WHITE_P)
                DrawTextureEx(data.tex[6], pos, 0, scale, WHITE);
            else
                DrawTextureEx(data.tex[7], pos, 0, scale, WHITE);
            break;
        case QUEEN:
            if (pieceOwner == WHITE_P)
                DrawTextureEx(data.tex[8], pos, 0, scale, WHITE);
            else
                DrawTextureEx(data.tex[9], pos, 0, scale, WHITE);
            break;
        case KING:
            if (pieceOwner == WHITE_P)
                DrawTextureEx(data.tex[10], pos, 0, scale, WHITE);
            else
                DrawTextureEx(data.tex[11], pos, 0, scale, WHITE);
            break;
    }
}

void drawAllPieces(Data &data) {
    int size = SCREEN_WIDTH / 8;

    // Iterating through all possible Boards.
    for (int x = 7; x >= 0; --x) {
        for (int y = 7; y >= 0; --y) {
            // Checking if there is an actual piece on the current square
            if (data.chessBoard.square[y][x].piece != nullptr) {
                raylib::Vector2 pos;
                float scale;

                int pieceType = data.chessBoard.square[y][x].piece->getType();
                int pieceOwner = data.chessBoard.square[y][x].piece->getOwner();
                pos.x = x * size;
                pos.y = y * size;
                scale = (float)size / 1280;
                // If it is not the grabbed piece, draw it with the correct texture.
                if (!data.chessBoard.square[y][x].piece->getGrabbed())
                    drawPiece(data, pieceType, pieceOwner, pos, scale);
            }
        }
    }
    // After all other pieces have been drawn, draw the grabbed piece last.
    if (data.grabbedPiece) {
        raylib::Vector2 pos;
        float scale;

        int pieceType = data.grabbedPiece->getType();
        int pieceOwner = data.grabbedPiece->getOwner();

        pos.x = GetMouseX() - size / 2;
        pos.y = GetMouseY() - size / 2;
        scale = (float)size / 1280;
        drawPiece(data, pieceType, pieceOwner, pos, scale * 1.10);
    }
}

bool possibleMoveCheck(Board &chessBoard, int pieceX, int pieceY, int targetX, int targetY) {
    bool isCheck = false;
    int player = chessBoard.square[pieceY][pieceX].piece->getOwner();

    ChessPiece *deletedPiece = movePiece(chessBoard, pieceX, pieceY, targetX, targetY);

    if (lookForCheck(chessBoard, player)) isCheck = true;

    revertMovePiece(chessBoard, pieceX, pieceY, targetX, targetY, deletedPiece, player);

    return (isCheck);
}

void drawPossibleMoves(Data &data, ChessPiece *piece, int pieceX, int pieceY, int size) {
    if (piece->getOwner() != data.turn) return;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if ((!data.chessBoard.square[y][x].piece ||
                 data.chessBoard.square[y][x].piece->getOwner() != piece->getOwner()) &&
                isMovePossible(data.chessBoard, pieceX, pieceY, x - pieceX, y - pieceY, false) &&
                !possibleMoveCheck(data.chessBoard, pieceX, pieceY, x, y)) {
                raylib::Color circCol;
                if (y % 2 == 0) {
                    if (x % 2 == 0)
                        circCol = data.secondaryColor;
                    else
                        circCol = data.primaryColor;
                } else {
                    if (x % 2 == 0)
                        circCol = data.primaryColor;
                    else
                        circCol = data.secondaryColor;
                }
                if (!data.chessBoard.square[y][x].piece)
                    DrawCircle(size * x + size / 2, size * y + size / 2, size / 6, circCol);
                else {
                    DrawCircle(size * x + size / 2, size * y + size / 2, size / 2.2, circCol);
                    if (data.lastMove[0] == &data.chessBoard.square[y][x] ||
                        data.lastMove[1] == &data.chessBoard.square[y][x]) {
                        raylib::Color lastMoveCol(GRAY);
                        DrawCircle(size * x + size / 2, size * y + size / 2, size / 2.7,
                                   lastMoveCol);
                    } else if (circCol == data.primaryColor)
                        DrawCircle(size * x + size / 2, size * y + size / 2, size / 2.7,
                                   data.secondaryColor);
                    else if (circCol == data.secondaryColor)
                        DrawCircle(size * x + size / 2, size * y + size / 2, size / 2.7,
                                   data.primaryColor);
                }
            }
        }
    }
}

void drawBoard(Data &data) {
    int size = SCREEN_WIDTH / 8;

    // Iterating through all possible Boards
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            // Checking if King is in check and on the current looked at field. If yes, draw in red.
            // Else draw in primary or secondary color.
            if ((y == data.chessBoard.kingPosY[WHITE_P] && x == data.chessBoard.kingPosX[WHITE_P] &&
                 data.chessBoard.kingCheck[WHITE_P]) ||
                (y == data.chessBoard.kingPosY[BLACK_P] && x == data.chessBoard.kingPosX[BLACK_P] &&
                 data.chessBoard.kingCheck[BLACK_P]))
                DrawRectangle(size * x, size * y, size, size, RED);
            else if (y % 2 == 0) {
                if (x % 2 == 0)
                    DrawRectangle(size * x, size * y, size, size, data.primaryColor);
                else
                    DrawRectangle(size * x, size * y, size, size, data.secondaryColor);
            } else {
                if (x % 2 == 0)
                    DrawRectangle(size * x, size * y, size, size, data.secondaryColor);
                else
                    DrawRectangle(size * x, size * y, size, size, data.primaryColor);
            }
            // If the current square's address matches with one where the last move was made paint
            // gray square over it.
            if (&data.chessBoard.square[y][x] == data.lastMove[0] ||
                &data.chessBoard.square[y][x] == data.lastMove[1]) {
                raylib::Color lastMoveCol(ORANGE);
                lastMoveCol.a = 200;
                DrawRectangle(size * x, size * y, size, size, lastMoveCol);
            }
        }
    }
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (data.grabbedPiece && data.grabbedPiecePosX == x && data.grabbedPiecePosY == y) {
                raylib::Color grabbedPieceCol(YELLOW);
                grabbedPieceCol.a = 150;
                DrawRectangle(size * x, size * y, size, size, grabbedPieceCol);
                drawPossibleMoves(data, data.chessBoard.square[y][x].piece, x, y, size);
            }
        }
    }
    int x = 0;
    for (int y = 0; y < 8; ++y) {
        std::string squareNbrY = std::to_string(x + 1);
        char c = y + '0' + 17;
        std::string squareNbrX;
        squareNbrX += c;
        int indent = size - size / 6;
        if (x % 2 == 0) {
            // Bottom Numbers
            DrawText(squareNbrX.c_str(), size * x + size / 20 + indent,
                     size * 7 + size / 20 + indent, size / 10, data.primaryColor);
            // Left Numbers
            DrawText(squareNbrY.c_str(), size * 0 + size / 20, size * y + size / 20, size / 10,
                     data.secondaryColor);
        } else {
            // Bottom Numbers
            DrawText(squareNbrX.c_str(), size * x + size / 20 + indent,
                     size * 7 + size / 20 + indent, size / 10, data.secondaryColor);
            // Left Numbers
            DrawText(squareNbrY.c_str(), size * 0 + size / 20, size * y + size / 20, size / 10,
                     data.primaryColor);
        }
        ++x;
    }
}

void drawDebugInfo(Data &data) {
    int size = SCREEN_WIDTH / 8;

    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (data.chessBoard.square[y][x].debug) {
                std::string text = "Debug on";
                int xOffset = size * x + size / 2 - MeasureText(text.c_str(), size / 10) / 2;
                int yOffset = size * y + (size / 10 * 0);
                DrawText(text.c_str(), xOffset, yOffset, size / 10, PINK);

                text = "HasMoved: ";
                if (data.chessBoard.square[y][x].piece) {
                    if (data.chessBoard.square[y][x].piece->getHasMoved())
                        text += "true";
                    else
                        text += "false";
                } else
                    text += "?";
                xOffset = size * x + size / 2 - MeasureText(text.c_str(), size / 10) / 2;
                yOffset = size * y + (size / 10 * 2);
                DrawText(text.c_str(), xOffset, yOffset, size / 10, PINK);

                text = "SquareVal: ";
                if (data.chessBoard.square[y][x].piece) {
                    int player = data.chessBoard.square[y][x].piece->getOwner();
                    float val = 0.0f;
                    if (player == WHITE_P)
                        val = getPieceSquareValue(data.chessBoard.square[y][x].piece, x, y,
                                                  data.chessBoard.endgame);
                    else
                        val = getPieceSquareValue(data.chessBoard.square[y][x].piece, x,
                                                  getOppositeSquare(y), data.chessBoard.endgame);
                    std::stringstream stream;
                    stream << std::fixed << std::setprecision(1) << val;
                    text += stream.str();
                } else
                    text += "?";
                xOffset = size * x + size / 2 - MeasureText(text.c_str(), size / 10) / 2;
                yOffset = size * y + (size / 10 * 3);
                DrawText(text.c_str(), xOffset, yOffset, size / 10, PINK);

                text = "Promoted: ";
                if (data.chessBoard.square[y][x].piece) {
                    if (data.chessBoard.square[y][x].piece->getPromotedPawn() != -1)
                        text +=
                            std::to_string(data.chessBoard.square[y][x].piece->getPromotedPawn());
                    else
                        text += "false";
                } else
                    text += "?";
                xOffset = size * x + size / 2 - MeasureText(text.c_str(), size / 10) / 2;
                yOffset = size * y + (size / 10 * 4);
                DrawText(text.c_str(), xOffset, yOffset, size / 10, PINK);
            }
        }
    }
}
