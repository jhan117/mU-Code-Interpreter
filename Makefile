# ============================
# 기본 설정
# ============================
CC := gcc
CFLAGS := -Wall -Wextra -g -Iinclude

SRC_DIR := src
BUILD_DIR := build
CLI_TARGET := $(BUILD_DIR)/ucode_interpreter
GUI_TARGET := $(BUILD_DIR)/gui_app

# GTK 플래그 (GUI 전용)
GTK_CFLAGS := `pkg-config --cflags gtk+-3.0`
GTK_LIBS := `pkg-config --libs gtk+-3.0`

# ============================
# 소스 수집
# ============================

# 전체 소스
ALL_SRC := $(shell find $(SRC_DIR) -name "*.c")

# CLI용: gui/ 제외
CLI_SRC := $(shell find $(SRC_DIR) -name "*.c" ! -path "*/gui/*")

# GUI용: 전체 포함
GUI_SRC := $(ALL_SRC)

CLI_OBJ := $(CLI_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/src/%.o)
GUI_OBJ := $(GUI_SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/src/%.o)

# ============================
# 빌드 타깃
# ============================
all: $(CLI_TARGET)

# CLI 빌드
cli-build: $(CLI_TARGET)

$(CLI_TARGET): $(CLI_OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

# GUI 빌드
gui-build: CFLAGS += $(GTK_CFLAGS)
gui-build: LDFLAGS := $(GTK_LIBS)
gui-build: $(GUI_TARGET)

$(GUI_TARGET): $(GUI_OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# ============================
# 오브젝트 공통 규칙
# ============================
$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ============================
# 실행
# ============================

# CLI 실행
cli-run: $(CLI_TARGET)
	./$(CLI_TARGET) test.uco result.lst

# GUI 실행
gui-run: $(GUI_TARGET)
	./$(GUI_TARGET)

# CLI 빌드 후 실행
cli: cli-build
	./$(CLI_TARGET)  test.uco result.lst

# GUI 빌드 후 실행
gui: gui-build
	./$(GUI_TARGET)

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
	$(MAKE) -C test clean

# ============================
# 오브젝트 공통 규칙
# ============================
$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: cli-build cli-run cli gui-build gui-run gui test-build test-run test test-clean clean