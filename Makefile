# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/07/06 10:49:48 by hthomas           #+#    #+#              #
#    Updated: 2021/07/24 21:40:59 by edal--ce         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SUFFIXES:
.SUFFIXES: .cpp .o

NAME		=	webserv

CXX			=	clang++
RM			=	rm -f

CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98
LDFLAGS		=	#-fsanitize	=thread -g3

SRCSDIR		=	srcs/
SRCS		=	main.cpp

OBJSDIR		=	build/
OBJS		=	$(addprefix $(OBJSDIR), $(SRCS:%.cpp=%.o))

HEADER		=	$(SRCSDIR)$(NAME).hpp

DEBUG		=	0

all:	$(NAME)

$(NAME):	$(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $@

$(OBJSDIR)%.o:	$(SRCSDIR)%.cpp $(HEADER)
	mkdir -p build
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -D DEBUG_ACTIVE=$(DEBUG) -c $< -o $@

clean:
	$(RM) $(OBJS)


fclean:	clean
	$(RM) $(NAME)

re:		fclean
	make all

test:	$(NAME)
	./$^

.PHONY:	all clean fclean re

# .SILENT:
