/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   History.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 14:35:57 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/18 13:40:23 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HISTORY_HPP
# define HISTORY_HPP

# include "ChessPiece.hpp"

struct History {
	ChessPiece	*movedPiece;
	ChessPiece	*removedPiece;
	int			fromX;
	int			fromY;
	int			toX;
	int			toY;
};

#endif