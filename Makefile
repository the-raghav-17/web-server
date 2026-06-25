CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -pedantic -MMD
INCL     := -Iinclude

SRC_DIR   := src
BUILD_DIR := build

MODULES := $(SRC_DIR) $(SRC_DIR)/socket $(SRC_DIR)/parser

SRCS   := $(shell find $(MODULES) -maxdepth 1 -name '*.cpp')
OBJS   := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS   := $(OBJS:%.o=%.d)
TARGET := server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCL) -c $< -o $@

-include $(DEPS)

clean:
	rm -fr build $(TARGET)
