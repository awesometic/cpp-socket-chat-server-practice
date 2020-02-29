TARGET = $(TARGET_DIR)/client

SRC_DIR = $(shell pwd)
SRC_EXT = cpp
SOURCES = $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))
OBJECTS = $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SOURCES:.$(SRC_EXT)=.o))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking client file..."
	$(Q) $(CXX) $(CXXFLAGS) -o $(TARGET) $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.$(SRC_EXT)
	@echo "Compiling $<..."
	$(Q) $(CXX) $(CXXFLAGS) -c -o $@ $^

.PHONY = all
