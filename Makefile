CC := gcc
CFLAGS := -Wall -Wextra -g -Iinclude `pkg-config --cflags gtk+-3.0`
LDFLAGS := `pkg-config --libs gtk+-3.0`

SRC_DIR := src
BUILD_DIR := build
TARGET := $(BUILD_DIR)/gui_app

# src 폴더 안 모든 .c 파일 자동 수집
SRC := $(shell find $(SRC_DIR) -name "*.c")

# 같은 트리 구조 유지하며 .o 생성
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

$(TARGET): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# 기본 타겟: GUI 실행 파일 빌드
all: build/gui_app

# GUI 실행
run: build/gui_app
	./build/gui_app

test:
	$(MAKE) -C test test

# 테스트 clean
test-clean:
	$(MAKE) -C test clean

# 전체 clean
clean:
	rm -rf build
	$(MAKE) -C test clean

.PHONY: all run test test-clean clean
