# ============================
# 기본 설정
# ============================
CC := gcc
CFLAGS := -Wall -Wextra -g -Iinclude

SRC_DIR := src
BUILD_DIR := build
TARGET := $(BUILD_DIR)/ucode_interpreter

# GTK 플래그
GTK_CFLAGS := `pkg-config --cflags gtk+-3.0`
GTK_LIBS := `pkg-config --libs gtk+-3.0`

# ============================
# 소스 수집
# ============================

ALL_SRC := $(shell find $(SRC_DIR) -name "*.c")
ALL_OBJ := $(ALL_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/src/%.o)

# ============================
# 빌드 타깃
# ============================

all: build

build: $(TARGET)

$(TARGET): $(ALL_OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) $^ -o $@ $(GTK_LIBS)

# ============================
# 오브젝트 공통 규칙
# ============================

$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -c $< -o $@

# ============================
# 실행
# ============================

run: $(TARGET)
	./$(TARGET)

# ============================
# 테스트
# ============================

test-build:
	$(MAKE) -C test test-build

test-run:
	$(MAKE) -C test test-run

test: test-build
	$(MAKE) -C test test

test-clean:
	$(MAKE) -C test clean

# ============================
# 클린
# ============================

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
	$(MAKE) -C test clean

.PHONY: all build run test-build test-run test test-clean clean