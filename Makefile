
# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#              #
#    Updated: 2021/10/12 23:13:43 by edal--ce         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

DIRSRC		=	srcs
OBJD		=	obj
INCLUDE		=	incl

INCLUDEF	=	$(INCLUDE)/includes.hpp		\
				$(INCLUDE)/location.hpp		\
				$(INCLUDE)/request.hpp		\
				$(INCLUDE)/server.hpp		\
				$(INCLUDE)/client.hpp		\
				$(INCLUDE)/webserv.hpp

SRC			=	main.cpp			\
				location.cpp		\
				request.cpp			\
				server.cpp			\
				utils_parsing.cpp	\
				client.cpp			\
				webserv.cpp

OBJ			=	$(SRC:.cpp=.o)
OBJS		=	$(OBJ:%=$(OBJD)/%)

CFLAGS		=	-Wall -Wextra -std=c++98 -g -fsanitize=address #-Werror

CC			=	clang++

RM			=	rm -rf

DEBUG		=	1


$(NAME)		:	$(OBJD) $(OBJS) $(INCLUDEF)
				$(CC) -I ./$(INCLUDE) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJD)		:
				@mkdir $(OBJD)

$(OBJD)/%.o	:	$(DIRSRC)/%.cpp $(INCLUDEF)
				$(CC) -I ./$(INCLUDE) $(CFLAGS) -D DEBUG_ACTIVE=$(DEBUG) -o $@ -c $<

all			:	$(NAME)

clean		:
				$(RM) $(OBJS)

fclean		:	clean

				$(RM) $(NAME) $(LIB)

run			:	all
				./webserv

run_config_base:	all
				./webserv config/base.conf

bonus		:	all

re			:	fclean all

.PHONY		:	all clean re fclean
