CXX = g++
CXXFLAGS = -std=gnu++11 -Wall -I include/ -I tests/ -DDOCTEST_CONFIG_NO_MULTITHREADING
LDFLAGS =

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
TEST_FILES = $(wildcard $(TEST_DIR)/*.cpp)

OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))
TEST_OBJ_FILES = $(patsubst $(TEST_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(TEST_FILES))

TEST_EXEC = test_runner

.PHONY: all clean run test dirs

all: dirs $(TEST_EXEC)

$(TEST_EXEC): $(OBJ_FILES) $(TEST_OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

dirs:
	-@mkdir -p $(BUILD_DIR)

run: all
	./$(TEST_EXEC)
test: all
	./$(TEST_EXEC)

clean:
	rm -rf $(BUILD_DIR)/*.o $(TEST_EXEC)
