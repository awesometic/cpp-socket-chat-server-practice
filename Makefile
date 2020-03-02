CXX = clang++
CXXFLAGS = \
	-Wall \
	-std=c++17
MAKE = make

ifneq ($V,1)
Q ?= @
endif

CUR_DIR = $(shell pwd)
PROJ_SRC_DIR = $(CUR_DIR)/src
BUILD_DIR = $(CUR_DIR)/build
TARGET_DIR = $(CUR_DIR)/bin

export CXX CXXFLAGS MAKE Q BUILD_DIR TARGET_DIR

default_to_src := \
	$(Q) $(shell mkdir -p $(BUILD_DIR)) \
	$(Q) $(shell mkdir -p $(TARGET_DIR)) \
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
	rm -f $(BUILD_DIR)/*
	rm -f $(TARGET_DIR)/*
