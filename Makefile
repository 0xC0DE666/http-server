CC := g++
C_FLAGS := -g -Wall -Wextra

BIN_DIR := ./bin
SRC_DIR := ./src

all: clean app;

APP_SRCS = $(wildcard $(SRC_DIR)/*.cpp)
APP_HDRS = $(wildcard $(SRC_DIR)/*.h)
APP_OBJS := $(patsubst %.cpp, %.o, $(APP_SRCS))

$(APP_SRCS):
	$(CC) $(C_FLAGS) -c -o $(patsubst %.cpp, %.o, $@) $@;

app: $(APP_OBJS);
	$(CC) $(C_FLAGS) -o $(BIN_DIR)/$@ $(APP_OBJS);

clean:
	rm -f $(APP_OBJS) $(BIN_DIR)/*;
