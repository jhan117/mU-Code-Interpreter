CC := gcc
CFLAGS := -Wall -Wextra -I../include -I.

SRC_DIR := ../src
TEST_DIR := .
BUILD_TEST := ../build-test
TARGET := $(BUILD_TEST)/run_tests

# 메인 소스 자동 수집 (gui 폴더 제외)
SRC := $(shell find $(SRC_DIR) -name "*.c" ! -path "*/gui/*")
# 테스트 소스 자동 수집 (test 폴더만)
TEST_SRC := $(shell find $(TEST_DIR) -name "*.c")

# 오브젝트 분리 저장
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_TEST)/src/%.o) \
       $(TEST_SRC:$(TEST_DIR)/%.c=$(BUILD_TEST)/test/%.o)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

# 메인 소스 -> test 전용 오브젝트
$(BUILD_TEST)/src/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# 테스트 소스 -> test 전용 오브젝트
$(BUILD_TEST)/test/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	@$(TARGET)

test-clean:
	rm -rf $(BUILD_TEST)

.PHONY: test test-clean
