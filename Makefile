CXX = g++
CXXFLAGS = -std=gnu++11 -Wall -I include/ -I tests/ -DDOCTEST_CONFIG_NO_MULTITHREADING
LDFLAGS = 

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

APP_MAIN = $(SRC_DIR)/main.cpp
SRC_LIB_FILES = $(filter-out $(APP_MAIN), $(SRC_FILES))

APP_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))
TEST_LIB_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_LIB_FILES))
TEST_OBJ = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_FILES))

APP_EXEC = semestria
TEST_EXEC = test_runner

.PHONY: all clean run test dirs

all: dirs $(APP_EXEC) $(TEST_EXEC)

$(APP_EXEC): $(APP_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST_EXEC): $(TEST_LIB_OBJ) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

dirs:
	-@mkdir -p $(BUILD_DIR)

run: dirs $(APP_EXEC)
	./$(APP_EXEC)

test: dirs $(TEST_EXEC)
	./$(TEST_EXEC)

clean:
	rm -rf $(BUILD_DIR)/*.o $(APP_EXEC) $(TEST_EXEC) $(APP_EXEC).exe $(TEST_EXEC).exe