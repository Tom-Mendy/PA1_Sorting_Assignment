CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++17 -O0 -Isrc

SRC_DIR = src
OBJ_DIR = obj
NAME = sort_benchmark

ifeq ($(OS),Windows_NT)
    RM = del /Q
    RMDIR = rmdir /S /Q
    MKDIR = mkdir
else
    RM = rm -f
    RMDIR = rm -rf
    MKDIR = mkdir -p
endif

SRCS := $(wildcard src/*.cpp)


OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
ifeq ($(OS),Windows_NT)
	@if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
else
	@mkdir -p $(OBJ_DIR)
endif
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
ifeq ($(OS),Windows_NT)
	-$(RMDIR) $(OBJ_DIR)
else
	$(RMDIR) $(OBJ_DIR)
endif

fclean: clean
ifeq ($(OS),Windows_NT)
	-$(RM) $(NAME).exe
else
	$(RM) $(NAME)
endif

re: fclean all

.PHONY: all clean fclean re
