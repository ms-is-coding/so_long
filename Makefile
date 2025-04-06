# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/13 20:22:30 by smamalig          #+#    #+#              #
#    Updated: 2025/04/06 19:18:35 by smamalig         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME           = so_long
CC             = cc
CFLAGS         = -Wall -Wextra -Werror -O3
SRC_FILES      = main.c textures.c hitbox.c threads.c options.c
SRCS           = $(addprefix src/, $(SRC_FILES))
OBJ_DIR        = obj
OBJS           = $(patsubst src/%.c, $(OBJ_DIR)/%.o, $(SRCS))
INCLUDES       = -Iinclude -Ilibft/include -Ilibmlx -Ilibft_gl/include
HEADERS        = $(wildcard include/*.h)

LIBFT          = libft
LIBFT_FLAGS    = -Llibft -lft
LIBFT_DIR      = ./libft

LIBMLX         = mlx
LIBMLX_FLAGS   = -Llibmlx -lmlx -lXext -lX11
LIBMLX_DIR     = ./libmlx

LDFLAGS        = $(LIBFT_FLAGS) $(LIBMLX_FLAGS)

RED            = \e[31m
GREEN          = \e[32m
YELLOW         = \e[33m
BLUE           = \e[34m
MAGENTA        = \e[35m
CYAN           = \e[36m
RESET          = \e[m

all: $(NAME)

bonus: all

$(NAME): $(LIBFT) $(LIBMLX) $(OBJS)
	@printf "$(BLUE)%s$(RESET): $(YELLOW)Building $(NAME)...$(RESET)\n" $(NAME)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@printf "$(BLUE)%s$(RESET): $(GREEN)Successfully built $(NAME)$(RESET)\n" $(NAME)

$(OBJ_DIR)/%.o: src/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@printf "$(BLUE)%s$(RESET): $(MAGENTA)Compiling$(RESET) $<\n" $(NAME)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR) --no-print-directory

$(LIBMLX):
	@make -C $(LIBMLX_DIR) --no-print-directory

clean:
	@printf "$(BLUE)%s$(RESET): $(RED)Cleaning object files...$(RESET)\n" $(NAME)
	@rm -rf obj
	@make -C $(LIBFT_DIR) clean --no-print-directory
	@make -C $(LIBMLX_DIR) clean --no-print-directory

fclean: clean
	@printf "$(BLUE)%s$(RESET): $(RED)Full clean...$(RESET)\n" $(NAME)
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean --no-print-directory

re: fclean all

.PHONY: all clean fclean re libft bonus $(LIBFT) $(LIBMLX)
