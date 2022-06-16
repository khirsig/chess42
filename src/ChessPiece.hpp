/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChessPiece.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 19:33:09 by khirsig           #+#    #+#             */
/*   Updated: 2022/06/04 19:46:23 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHESSPIECE_HPP
# define CHESSPIECE_HPP

# define WHITE_P 0
# define BLACK_P 1

# define PAWN 0
# define BISHOP 1
# define KNIGHT 2
# define ROOK 3
# define QUEEN 4
# define KING 5

struct ChessPiece {
	int	owner;
	int	type;
};

#endif