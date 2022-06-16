# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/10 09:02:38 by tjensen           #+#    #+#              #
#    Updated: 2022/06/04 19:31:12 by khirsig          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#	PROJECT SPECIFIC														   #
# **************************************************************************** #

NAME			:= chess

CC				:= g++
CFLAGS			:= -std=c++0x -O3

SRCS			:= 		main.cpp									\

SDIR			:= src
ODIR			:= obj
OBJS			:= $(addprefix $(ODIR)/, $(SRCS:.cpp=.o))

BREW = $(shell brew --prefix)
LDFLAGS			:= -L $(BREW)/lib -lraylib -lcurl
CFLAGS			+= -I $(BREW)/include

# **************************************************************************** #
#	RULES																	   #
# **************************************************************************** #

all:
	@make $(NAME) -j8

$(NAME): $(ODIR) $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)
	@echo "\033[1;32mChess compiled and linked.\033[0m"

$(ODIR)/%.o: $(SDIR)/%.cpp $(SDIR)/*.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR):
	mkdir -p $(ODIR)

clean:
	$(RM) -r $(ODIR)

fclean: clean
	$(RM) -r *.dSYM $(SDIR)/*.dSYM
	$(RM) $(NAME)

re: fclean all
