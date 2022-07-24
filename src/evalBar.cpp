/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evalBar.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 05:14:35 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/24 16:23:08 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Data.hpp"

void	runEvalBar(Data &data, Board &chessBoard)
{
	data.barBoard = copyBoard(chessBoard);
	if (data.currentBarDepth == 3)
	{
		data.nextBarDepth = data.currentBarDepth + 1;
	}

	std::cout << "Current Bar Depth: " << data.currentBarDepth << "  ";
	data.barThreadMove = std::async(lookForMoves, data.barBoard, data.barTurn, data.currentBarDepth, false);
	data.currentBarDepth++;
}

void	drawEvalBar(Data &data, Board &chessBoard)
{
	if (data.barThreadMove.valid() && data.barThreadMove.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready)
	{
		Move bestMove = data.barThreadMove.get();
		data.barEval = bestMove.getEvalPoints();
		if (data.nextBarDepth < 6)
			data.nextBarDepth = data.currentBarDepth + 1;
		if (data.barTurn == WHITE_P)
			data.barEval *= -1;
		data.barTurn = data.turn;
	}

	int endBlack = (float)SCREEN_HEIGHT / 20 * (data.barEval + 10);

	DrawRectangle(SCREEN_WIDTH, 0, SCREEN_WIDTH * 1.03, endBlack, BLACK);
	DrawRectangle(SCREEN_WIDTH, endBlack, SCREEN_WIDTH * 1.03, SCREEN_HEIGHT, RAYWHITE);
	float evalPts = data.barEval;
	bool	whiteInFront;
	if (data.barEval < 0)
	{
		whiteInFront = true;
		evalPts *= -1;
	}
	else
	{
		whiteInFront = false;
	}
	std::string points = std::to_string((int)evalPts);
	if (whiteInFront)
		DrawText(points.c_str(), SCREEN_WIDTH + 5, SCREEN_HEIGHT - 20, 15, BLACK);
	else
		DrawText(points.c_str(), SCREEN_WIDTH + 5, 20, 15, WHITE);
}
