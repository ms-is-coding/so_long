# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/13 20:22:30 by smamalig          #+#    #+#              #
#    Updated: 2025/03/22 23:39:04 by smamalig         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME           = so_long
CC             = cc
CFLAGS         = -Wall -Wextra -Werror -g3 -fsanitize=address
SRCS           = src/main.c src/textures.c src/hitbox.c
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

# Colors
RED            = \033[0;31m
GREEN          = \033[0;32m
YELLOW         = \033[0;33m
BLUE           = \033[0;34m
MAGENTA        = \033[0;35m
CYAN           = \033[0;36m
RESET          = \033[0m

all: $(NAME)

bonus: all

$(NAME): $(LIBFT) $(LIBMLX) $(OBJS)
	@printf "$(BLUE)%12s$(RESET): $(YELLOW)Building $(NAME)...$(RESET)\n" $(NAME)
	@$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $(NAME)
	@printf "$(BLUE)%12s$(RESET): $(GREEN)Successfully built $(NAME)$(RESET)\n" $(NAME)

$(OBJ_DIR)/%.o: src/%.c $(HEADERS)
	@mkdir -p $(dir $@)
	@printf "$(BLUE)%12s$(RESET): $(MAGENTA)Compiling$(RESET) $<\n" $(NAME)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make -C $(LIBFT_DIR) --no-print-directory

$(LIBMLX):
	@make -C $(LIBMLX_DIR) --no-print-directory

clean:
	@printf "$(BLUE)%12s$(RESET): $(RED)Cleaning object files...$(RESET)\n" $(NAME)
	@rm -rf obj
	@make -C $(LIBFT_DIR) clean --no-print-directory
	@make -C $(LIBFT_GL_DIR) clean --no-print-directory
	@make -C $(LIBMLX_DIR) clean --no-print-directory

fclean: clean
	@printf "$(BLUE)%12s$(RESET): $(RED)Full clean...$(RESET)\n" $(NAME)
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean --no-print-directory
	@make -C $(LIBFT_GL_DIR) fclean --no-print-directory

re: fclean all

.PHONY: all clean fclean re libft bonus $(LIBFT) $(LIBFT_GL) $(LIBMLX)
