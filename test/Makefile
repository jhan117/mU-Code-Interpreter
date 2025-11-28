# ============================
# 기본 설정
# ============================
CC := gcc
CFLAGS := -Wall -Wextra -g -I../include -I.

SRC_DIR := ../src
TEST_DIR := .
BUILD_DIR := ../build-test

TARGET := $(BUILD_DIR)/run_tests

# ============================
# 소스 수집
# ============================

# 실제 프로젝트 코드 (src)
# - GUI 제외
# - main.c 제외
SRC_FILES := $(shell find $(SRC_DIR) -name "*.c" \
                ! -path "*/gui/*" \
                ! -name "main.c")

# 테스트 코드
TEST_FILES := $(shell find $(TEST_DIR) -name "*.c" ! -path "*/build-test/*")

# 오브젝트
OBJ_SRC := $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/src/%.o)
OBJ_TEST := $(TEST_FILES:$(TEST_DIR)/%.c=$(BUILD_DIR)/test/%.o)

OBJ := $(OBJ_SRC) $(OBJ_TEST)

# ============================
# 빌드 타깃
# ============================
test-build: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

# ============================
# 실행
# ============================
test-run: $(TARGET)
	./$(TARGET)

# 빌드 후 실행
test: test-build
	./$(TARGET)

# ============================
# 클린
# ============================
clean:
	rm -rf $(BUILD_DIR)

# ============================
# 오브젝트 빌드 규칙
# ============================
$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: test-build test-run test clean
