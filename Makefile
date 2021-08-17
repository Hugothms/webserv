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
RM			=	rm -f


$(NAME)		:	$(LIB) $(OBJD) $(OBJS) $(INCLUDEF)
				$(CC) -I ./$(INCLUDE) -I ./$(MLX_OS) $(LIB) $(CFLAGS) $(CFRAME) $(OBJS) -o $(NAME) 

$(OBJD)		:
				@mkdir $(OBJD)

$(OBJD)/%.o	:	$(DIRSRC)/%.cpp
				$(CC) -I ./$(INCLUDE) -I ./$(INCLUDE) $(CFLAGS) -o $@ -c $<

$(LIB)			:
				$(MAKE) -C $(MLX_OS)
				cp $(MLX_OS)/$(LIB) ./.

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