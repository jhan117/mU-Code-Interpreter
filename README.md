# mU-code-interpreter

Software Project 수업의 step3 과제입니다.

## 프로젝트 개요

## 디렉터리 구조

- `src/`: 소스 코드
- `include/`: 헤더 파일
- `assets/`: CSS 등 리소스 파일
- `test/`: 테스트 코드
- `docs/`: 회의록, 보고서 등 문서 파일

## 빌드 및 실행 방법

```sh
# 빌드 및 빌드 후 gui 실행
make build
make run

# 실행
.build/ucode_interpreter [uco 파일 경로] [lst 파일 경로] # cli 실행
.build/ucode_interpreter # gui 실행
```

GTK+3 개발 환경이 필요합니다.
(ex: Ubuntu에서는 `sudo apt-get install libgtk-3-dev`)
한글 폰트 설치가 필요합니다.
(ex: Ubuntu에서는 `sudo apt install fonts-nanum`)

```sh
# 배포 버전 실행 (리눅스) => releases 확인

# GUI 실행
./mUCodeInterpreter-x86_64.AppImage

# CLI 실행
./mUCodeInterpreter-x86_64.AppImage [uco 파일] [lst 파일]
```

## 개발 일지

## 참고자료

- mU-Code의 문제 SWP 교수님 PDF 자료
- 컴퓨터 구조 교수님 PDF 자료
- 리눅스 활용 및 프로그래밍 교수님 PDF 자료