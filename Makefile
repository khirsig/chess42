# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/03/10 09:02:38 by tjensen           #+#    #+#              #
#    Updated: 2022/06/23 14:54:08 by khirsig          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#	PROJECT SPECIFIC														   #
# **************************************************************************** #

NAME			:= chess42

CC				:= c++
CFLAGS			:= -std=c++0x -O3

SRCS			:= main.cpp									\

OS				:= $(shell uname -s)
NUMPROC			:= 8

ifeq ($(OS),Linux)
		NUMPROC := $(shell grep -c ^processor /proc/cpuinfo)
else ifeq ($(OS),Darwin)
		NUMPROC := $(shell sysctl -n hw.ncpu)
endif

SDIR			:= src
HDIR			:= include
ODIR			:= obj
OBJS			:= $(addprefix $(ODIR)/, $(SRCS:.cpp=.o))

BREW			:= $(shell brew --prefix)
LDFLAGS			:= -L $(BREW)/lib -lraylib -lcurl
CFLAGS			+= -I $(BREW)/include

# **************************************************************************** #
#	RULES																	   #
# **************************************************************************** #

all:
	@make $(NAME) -j$(NUMPROC)

$(NAME): $(ODIR) $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)
	@echo "\033[1;32m$(NAME) compiled and linked.\033[0m"

$(ODIR)/%.o: $(SDIR)/%.cpp $(HDIR)/*.hpp
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR):
	mkdir -p $(ODIR)

clean:
	$(RM) -r $(ODIR)

fclean: clean
	$(RM) -r *.dSYM $(SDIR)/*.dSYM
	$(RM) $(NAME)

re: fclean all
