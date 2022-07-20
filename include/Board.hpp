/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Board.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/19 11:58:22 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/20 16:01:58 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOARD_HPP
# define BOARD_HPP

# include "./BoardSquare.hpp"

struct Board {
	BoardSquare	square[8][8];
	int			kingPosX[2];
	int			kingPosY[2];
	bool		kingCheck[2] = { false, false };
	bool		checkmate = false;
	bool		stalemate = false;
	int			iterations = 0;
	bool		endgame = false;
};

#endif