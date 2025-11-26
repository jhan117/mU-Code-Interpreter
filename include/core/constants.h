#pragma once

// 명령어 형식
#define LINE_BUFFER_LEN 256 // 널문자 포함 한 줄당 길이
#define MAX_LABEL_LEN 11    // 널문자 포함
#define MAX_OP_LEN 5        // 널문자 포함
#define MAX_OPERANDS 3
#define MAX_ARGS 10

// 동적 배열
#define INIT_LIST_CAPACITY 16
#define INIT_LINE_CAPACITY 100
#define INIT_SNAPSHOT_LIST_CAPACITY 50;
#define INIT_OUTPUT_BUF_CAPACITY 256

// 실행 관련 상수들
#define ERR_INVALID_ADDR 0x1
#define ERR_INVALID_PC 0x2
#define ERR_INVALID_BP 0x4
#define ERR_STACK_OVERFLOW 0x8
#define ERR_STACK_UNDERFLOW 0x10
#define ERR_CPU_STACK_OVERFLOW 0x20
#define ERR_CPU_STACK_UNDERFLOW 0x40

#define HD_NUM_MEMORY 0
#define HD_NUM_CPU_STACK 1
#define HD_NUM_CS 2
#define HD_NUM_PC 3
#define HD_NUM_DS 4
#define HD_NUM_SS 5
#define HD_NUM_SP 6
#define HD_NUM_BP 7

#define OPCODE_MAX 48
#define TOTAL_OPCODE_GROUPS 5
#define INIT_CPU_STACK_CAPACITY 100
#define INIT_MEMORY_SIZE 65536
#define INIT_SNAPSHOT_LIST_CAPACITY 10
#define SNAPSHOT_INTERVAL 1000

#define CLI 0
#define GUI 1

#define GLOBAL_BLOCK 1
