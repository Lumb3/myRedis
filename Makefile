CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread -MMD -MP -O2

SRC_DIR = src
BUILD_DIR = build

# Server files (exclude client)
SRCS_SERVER := $(filter-out $(SRC_DIR)/myredis_cli.cpp, $(wildcard $(SRC_DIR)/*.cpp))
OBJS_SERVER := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS_SERVER))

# Client files
SRCS_CLIENT := $(SRC_DIR)/myredis_cli.cpp
OBJS_CLIENT := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS_CLIENT))

TARGET_SERVER = myRedis
TARGET_CLIENT = myRedis_cli

all: $(TARGET_SERVER) $(TARGET_CLIENT)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile server objects
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build server
$(TARGET_SERVER): $(OBJS_SERVER)
	$(CXX) $(CXXFLAGS) $(OBJS_SERVER) -o $(TARGET_SERVER)

# Build client
$(TARGET_CLIENT): $(OBJS_CLIENT)
	$(CXX) $(CXXFLAGS) $(OBJS_CLIENT) -o $(TARGET_CLIENT)

clean:
	rm -rf $(BUILD_DIR) $(TARGET_SERVER) $(TARGET_CLIENT)

rebuild: clean all
