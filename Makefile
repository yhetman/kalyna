#* ************************************************************************** *#
#*                                                                            *#
#*                                                                            *#
#*   Makefile                                                                 *#
#*                                                                            *#
#*   By: yhetman <yhetman@student.unit.ua>                                    *#
#*                                                                            *#
#*   Created: 2021/10/05 12:45:04 by yhetman                                  *#
#*   Updated: 2021/10/05 12:45:13 by yhetman                                  *#
#*                                                                            *#
#* ************************************************************************** *#

SRCS =  main.c  cipher.c get_options.c \
		key_scheduler.c tables.c
#decipher.c  bitwise_operations.c
SRCS_DIR = src/

OBJ_DIR = obj/

OBJ = $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))

OBJ_DIR = obj/

LIB_NAME = libkalyna.a

KALYNA = kalyna

FLAGS = -I includes -Wall -Wextra -Werror

DEBUG_FLAGS = -g3 -fsanitize=address

KALYNA_MAIN = src/main.c

all: make_obj_dir $(KALYNA)

$(OBJ_DIR)%.o: $(SRCS_DIR)%.c
	gcc $(FLAGS) -c $< -o $@

$(LIB_NAME): $(OBJ)
	ar -rv $(LIB_NAME) $^
	ranlib $(LIB_NAME)

$(KALYNA): $(LIB_NAME) $(KALYNA_MAIN)
	gcc $(FLAGS) $(KALYNA_MAIN) $(LIB_NAME) -o $(KALYNA)

make_obj_dir:
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(KALYNA)
	rm -f $(LIB_NAME)

re: fclean all

.PHONY: all clean flcean re debug