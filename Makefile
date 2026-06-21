CXX      := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -pedantic -MMD
INCL     := -Iinclude

SRC_DIR   := src
BUILD_DIR := build

MODULES := $(SRC_DIR) $(SRC_DIR)/socket

SRCS := $(shell find $(MODULES) -name '*.cpp')
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:%.o=%.d)
OUT  := server

all: $(OBJS)
	$(CXX) $^ -o $(OUT)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCL) -c $^ -o $@

-include $(DEPS)

clean:
	rm -fr build $(OUT)
