# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/09/08 14:55:13 by edal--ce          #+#    #+#              #
#    Updated: 2021/09/08 14:55:54 by edal--ce         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	webserv

DIRSRC		=	srcs
OBJD		=	obj
INCLUDE		=	incl

INCLUDEF	=	$(INCLUDE)/includes.hpp 	\
				$(INCLUDE)/request.hpp		\
				$(INCLUDE)/server.hpp		\
				$(INCLUDE)/webserv.hpp		

SRC			=	main.cpp		\
				server.cpp		\
				request.cpp	

OBJ			=	$(SRC:.cpp=.o)
OBJS		=	$(OBJ:%=$(OBJD)/%)

CFLAGS		= 	-Wall -Wextra -std=c++98 -g -fsanitize=address #-Werror 

CC			=	clang++

RM			=	rm -rf

$(NAME)		:	$(OBJD) $(OBJS) $(INCLUDEF)
				$(CC) -I ./$(INCLUDE) $(CFLAGS) $(OBJS) -o $(NAME) 

$(OBJD)		:
				@mkdir $(OBJD)

$(OBJD)/%.o	:	$(DIRSRC)/%.cpp
				$(CC) -I ./$(INCLUDE) -I ./$(INCLUDE) $(CFLAGS) -o $@ -c $<

all			:	$(NAME)

clean		:
				$(RM) $(OBJS)

fclean		:	clean
				
				$(RM) $(NAME) $(LIB)

run 		: re
			./webserv

bonus		:	all

re			:	fclean all

.PHONY		:	all clean re fclean