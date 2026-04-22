# ============================================================================
# Codexion - Concurrent Programming Project
# ============================================================================

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread
LDFLAGS		= -pthread

NAME		= codexion
SRC_DIR		= coders
OBJ_DIR		= obj

# Source files
SRCS		= $(SRC_DIR)/main.c \
				$(SRC_DIR)/parse.c \
				$(SRC_DIR)/init.c \
				$(SRC_DIR)/sim.c \
				$(SRC_DIR)/coder_routine.c \
				$(SRC_DIR)/actions.c \
				$(SRC_DIR)/dongle.c \
				$(SRC_DIR)/log.c \
				$(SRC_DIR)/time.c \
				$(SRC_DIR)/monitor.c \
				$(SRC_DIR)/scheduler.c \
				$(SRC_DIR)/scheduler_heap.c \
				$(SRC_DIR)/cleanup.c \
				$(SRC_DIR)/utils.c

# Object files
OBJS		= $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# ============================================================================
# Rules
# ============================================================================

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LDFLAGS)
	@echo "✅ $(NAME) compiled successfully!"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	@echo "🧹 Object files removed"

fclean: clean
	rm -f $(NAME)
	@echo "🧹 Executable removed"

re: fclean all

.PHONY: all clean fclean re