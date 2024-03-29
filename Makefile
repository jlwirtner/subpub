#TODO: Clean up!
# - dangling rule for sched target
# - need to add rules for post and listen targets
CXX			:= g++
CXXFLAGS	:= -std=c++17
LDFLAGS		:= 
BUILD 		:= ./build
THIRD_PARTY	:= ./third_party
OBJ_DIR		:= $(BUILD)/objects
APP_DIR		:= $(BUILD)/bin
LIB_DIR		:= $(BUILD)/lib
TARGETS		:= ./targets
TARGET_SCHED		:= sp_sched
TARGET_POST		:= sp_post
TARGET_LISTEN		:= sp_listen
TARGET_LIB 	:= libsubpub.a
TARGET_TEST	:= sp_test
INCLUDE		:= -Iinc/
SRC			:= $(wildcard src/*.cpp) \
			   $(TARGETS)/sched_main.cpp
SCHED_SRC   := $(TARGETS)/sched_main.cpp
POST_SRC    := $(TARGETS)/post_main.cpp
LISTEN_SRC  := $(TARGETS)/listen_main.cpp
LIB_SRC		:= $(wildcard src/*.cpp)
TEST_SRC	:= $(TARGETS)/test.cpp
CATCH_INCLUDE := -Ithird_party/


OBJECTS		:= $(SRC:%.cpp=$(OBJ_DIR)/%.o)
LIB_OBJECTS	:= $(LIB_SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build lib test sched post listen

lib: build $(LIB_DIR)/$(TARGET_LIB)

test: build $(APP_DIR)/$(TARGET_TEST)

sched: build $(APP_DIR)/$(TARGET_SCHED)

post: build $(APP_DIR)/$(TARGET_POST)

listen: build $(APP_DIR)/$(TARGET_LISTEN)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(APP_DIR)/$(TARGET_TEST): $(TEST_SRC) $(LIB_DIR)/$(TARGET_LIB)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CATCH_INCLUDE) $(INCLUDE) $< -o $@ $(LDFLAGS) -L $(LIB_DIR) -lsubpub

$(LIB_DIR)/$(TARGET_LIB): $(LIB_OBJECTS)
	@mkdir -p $(@D)
	ar rcs $(LIB_DIR)/$(TARGET_LIB) $^

$(APP_DIR)/$(TARGET_SCHED): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET_SCHED) $^ $(LDFLAGS)

$(APP_DIR)/$(TARGET_SCHED): $(SCHED_SRC) $(LIB_DIR)/$(TARGET_LIB)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ $(LDFLAGS) -L $(LIB_DIR) -lsubpub

$(APP_DIR)/$(TARGET_POST): $(POST_SRC) $(LIB_DIR)/$(TARGET_LIB)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ $(LDFLAGS) -L $(LIB_DIR) -lsubpub

$(APP_DIR)/$(TARGET_LISTEN): $(LISTEN_SRC) $(LIB_DIR)/$(TARGET_LIB)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $< -o $@ $(LDFLAGS) -L $(LIB_DIR) -lsubpub


.PHONY: all lib build sched post listen clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*