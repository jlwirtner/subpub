CXX			:= g++
CXXFLAGS	:= -std=c++11
LDFLAGS		:= 
BUILD 		:= ./build
OBJ_DIR		:= $(BUILD)/objects
APP_DIR		:= $(BUILD)/bin
TARGETS		:= ./targets
TARGET		:= sp_sched
INCLUDE		:= -Iinc/
SRC			:= $(wildcard src/*.cpp) \
			   $(TARGETS)/sched_main.cpp


OBJECTS		:= $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET) $^ $(LDFLAGS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*