CC := gcc
CFLAGS := -Wall -Wextra -I../include -I.
BUILD_DIR := ../build
TARGET := $(BUILD_DIR)/run_tests

SRC := \
    ../src/core/vm_context.c

TEST_SRC := \
    core/test_vm_context.c \
    test_main.c

OBJ := $(SRC:../src/%.c=$(BUILD_DIR)/%.o) \
       $(TEST_SRC:%.c=$(BUILD_DIR)/%.o)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: ../src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	@$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: test clean
