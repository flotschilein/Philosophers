# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fbraune <fbraune@student.42heilbronn.de>   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/29 22:48:41 by fbraune           #+#    #+#              #
#    Updated: 2025/08/17 15:53:52 by fbraune          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME	= philo
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -O3 -pthread
RM		= rm -rf

SRCS	= main.c
OBJS 	= $(SRCS:.c=.o)

all: $(NAME)
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re