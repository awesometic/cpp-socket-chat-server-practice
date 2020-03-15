CXX = clang++
CXXFLAGS = \
	-Wall \
	-std=c++17
MAKE = make

ifneq ($V,1)
Q ?= @
endif

ifneq ($DEBUG,1)
DEBUG ?= 0
endif

CUR_DIR = $(shell pwd)
PROJ_SRC_DIR = $(CUR_DIR)/src
BUILD_DIR_BASE = $(CUR_DIR)/build
SERVER_BUILD_DIR = $(BUILD_DIR_BASE)/server
CLIENT_BUILD_DIR = $(BUILD_DIR_BASE)/client
TARGET_DIR = $(CUR_DIR)/bin

export CXX CXXFLAGS MAKE Q DEBUG
export SERVER_BUILD_DIR CLIENT_BUILD_DIR TARGET_DIR

default_to_src := \
	$(Q) $(shell mkdir -p $(SERVER_BUILD_DIR) $(CLIENT_BUILD_DIR) $(TARGET_DIR)) \
	$(Q) $(MAKE) -C $(PROJ_SRC_DIR) $@ $(1)

.PHONY = all
all:
	$(call default_to_src,all)

.PHONY = server
server:
	$(call default_to_src,server)

.PHONY = client
client:
	$(call default_to_src,client)

.PHONY = clean
clean:
	rm -rf $(BUILD_DIR_BASE)/*
	rm -rf $(TARGET_DIR)/*
