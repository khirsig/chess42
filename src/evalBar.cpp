/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evalBar.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 05:14:35 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/23 21:35:49 by khirsig          ###   ########.fr       */
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
		if (data.nextBarDepth < 5)
			data.nextBarDepth = data.currentBarDepth + 1;
		if (data.barTurn == WHITE_P)
			data.barEval *= -1;
		std::cout << "Bar Eval: " << data.barEval << "\n";
		data.barTurn = data.turn;
	}

	int endBlack = (float)SCREEN_HEIGHT / 20 * (data.barEval + 10);

	DrawRectangle(SCREEN_WIDTH, 0, SCREEN_WIDTH * 1.02, endBlack, BLACK);
	DrawRectangle(SCREEN_WIDTH, endBlack, SCREEN_WIDTH * 1.02, SCREEN_HEIGHT, RAYWHITE);
}
