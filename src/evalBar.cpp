/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evalBar.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 05:14:35 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/23 18:15:11 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Data.hpp"

void	runEvalBar(Data &data, Board &chessBoard)
{
	if (data.barBoard != nullptr)
		delete data.barBoard;
	std::cout << "HALLO\n";
	data.barBoard = copyBoard(chessBoard);
	data.evaluation = std::async(calculateBoard, std::ref(*data.barBoard), BLACK_P, false);
}

void	drawEvalBar(Data &data, Board &chessBoard)
{

	if (data.evaluation.valid() && data.evaluation.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready)
	{
		data.barPower = data.evaluation.get();
		if (data.barPower > 10)
			data.barPower = 10;
		if (data.barPower < -10)
			data.barPower = -10;
		data.barPower += 10;
	}

	int endBlack = SCREEN_HEIGHT / 20 * data.barPower;

	DrawRectangle(SCREEN_WIDTH, 0, SCREEN_WIDTH * 1.02, endBlack, BLACK);
	DrawRectangle(SCREEN_WIDTH, endBlack, SCREEN_WIDTH * 1.02, SCREEN_HEIGHT, RAYWHITE);
}
