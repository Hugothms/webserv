# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hthomas <hthomas@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/06 10:49:48 by hthomas           #+#    #+#              #
#    Updated: 2021/07/06 11:21:24 by hthomas          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SUFFIXES:
.SUFFIXES: .cpp .o

NAME	=		webserv

SHELL	=		/bin/sh
CXX		=		clang++
RM		=		rm -f

CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98
LDFLAGS	=		#-fsanitize=thread -g3
THREADFLAGS	=	#-pthread

SRCSDIR	=		srcs/
SRCS	=		main.cpp

OBJSDIR	=		build/
OBJS	=		$(addprefix $(OBJSDIR), $(SRCS:%.cpp=%.o))

HEADER	=		$(SRCSDIR)$(NAME).hpp


all:	$(NAME)

$(OBJSDIR)%.o:	$(SRCSDIR)%.cpp $(HEADER)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $< -o $@

$(NAME):	$(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(THREADFLAGS) $(OBJS) -o $@

clean:
	$(RM) $(OBJS)


fclean:	clean
	$(RM) $(NAME)

re:		fclean
	make all

test:	$(NAME)
	./$^

.PHONY:	all clean fclean re

.SILENT:
