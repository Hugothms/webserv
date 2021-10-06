#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: edal--ce <edal--ce@sudent.42.fr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/04 14:27:31 by edal--ce          #+#    #+#              #
#    Updated: 2020/08/18 21:53:32 by edal--ce         ###   ########.fr        #
#                                                                              #
#******************************************************************************#
NAME		=	webserv

DIRSRC		=	srcs
OBJD		=	obj
INCLUDE		=	incl

INCLUDEF	=	$(INCLUDE)/includes.hpp 	\
				$(INCLUDE)/server.hpp		\
				$(INCLUDE)/request.hpp						
# $(INCLUDE)/webserv.hpp		

SRC			=	main.cpp		\
				server.cpp		\
				request.cpp	

OBJ			=	$(SRC:.cpp=.o)
OBJS		=	$(OBJ:%=$(OBJD)/%)

CFLAGS		= 	-Wall -Wextra -std=c++98 -g -fsanitize=address #-Werror 

CC			=	clang++

RM			=	rm -f

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

run 		: all
			./webserv

bonus		:	all

re			:	fclean all

.PHONY		:	all clean re fclean