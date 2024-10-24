CC := g++
C_FLAGS := -std=c++11 -g -Wall -Wextra -pthread

BIN_DIR := ./bin
SRC_DIR := ./src

all: clean app test;

clean:
	rm -f $(APP_OBJS) $(BIN_DIR)/*;

#------------------------------
# APP
#------------------------------

APP_DIR := $(SRC_DIR)/app
APP_SRCS = $(wildcard $(APP_DIR)/*.cpp)
APP_HDRS = $(wildcard $(APP_DIR)/*.hpp)
APP_OBJS := $(patsubst %.cpp, %.o, $(APP_SRCS))
APP_OBJS_NO_MAIN := $(patsubst $(APP_DIR)/main.o, , $(APP_OBJS))

$(APP_SRCS):
	$(CC) $(C_FLAGS) -c -o $(patsubst %.cpp, %.o, $@) $@;

app: $(APP_OBJS);
	$(CC) $(C_FLAGS) -o $(BIN_DIR)/$@ $(APP_OBJS);

#------------------------------
# TESTS
#------------------------------

TEST_DIR := $(SRC_DIR)/test
TEST_SRCS := $(wildcard $(TEST_DIR)/*.cpp)
TEST_HDRS = $(wildcard $(TEST_DIR)/*.hpp)
TEST_OBJS := $(patsubst %.c, %.o, $(TEST_SRCS))

$(TEST_SRCS):
	$(CC) $(C_FLAGS) -c -o $(patsubst %.c, %.o, $@) $@;

test: $(APP_OBJS_NO_MAIN) $(TEST_OBJS);
	$(CC) $(C_FLAGS) -lcriterion -o $(BIN_DIR)/$@ $(APP_OBJS_NO_MAIN) $(TEST_OBJS);
