# 기본 컴파일러/플래그
CC := gcc
CFLAGS := -Wall -Wextra -Iinclude

# 프로젝트 디렉터리/타깃 정의
SRC_DIR := src
BUILD_DIR := build
APP := $(BUILD_DIR)/ucode_interpreter

# 전체 소스/오브젝트 목록 자동 수집
SRC := $(shell find $(SRC_DIR) -name "*.c")
OBJ := $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/src/%.o)

all: build

# 실행 파일 빌드
build: $(APP)

# 링크 단계
$(APP): $(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

# 각 소스 -> 오브젝트 컴파일
$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# 테스트 실행
test:
	$(MAKE) -C test test

# 빌드 산출물 정리
clean:
	rm -rf $(BUILD_DIR)
	$(MAKE) -C test clean

.PHONY: all build test clean
