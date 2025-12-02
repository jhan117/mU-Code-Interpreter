# mU-code-interpreter

Software Project 수업의 step3 2인 개발 과제입니다.

## 프로젝트 개요
<img width="600" alt="Image" src="https://github.com/user-attachments/assets/792eca85-81a9-4a50-8470-c52b08fa7ea1" />

- mUcode 언어를 어셈블 → 실행 → 결과 확인 할수 있는 GUI/CLI 통합 인터프리터 개발
- 34개 명령어셋 지원
- 단계별 레지스터/메모리 시각화
- AppImage 배포
- 예제 프로그램/벤치마크 지원

## 디렉터리 구조

- `src/`: 소스 코드
- `include/`: 헤더 파일
- `assets/`: CSS 등 리소스 파일
- `test/`: 테스트 코드
- `docs/`: 회의록, 보고서 등 문서 파일
- `benchmark/`: 벤치마크를 위한 C,Java,Python,U-Code와 쉘스크립트

## 빌드 및 실행 방법

```sh
# 빌드 및 빌드 후 gui 실행
make build
make run

# 실행
.build/ucode_interpreter [uco 파일 경로] [lst 파일 경로] # cli 실행
.build/ucode_interpreter # gui 실행

# 벤치마크 (개발자용)
bash benchmark/run_benchmarks.sh # 모든 벤치마크 실행
.build/ucode_interpreter --benchmark [uco 파일 경로] # 단일 벤치마크 실행, 결과 출력 x, lst 파일 생성 안함
```

GTK+3 개발 환경이 필요합니다.
(ex: Ubuntu에서는 `sudo apt-get install libgtk-3-dev`)  
한글 폰트 설치가 필요합니다.
(ex: Ubuntu에서는 `sudo apt install fonts-nanum`)  
벤치마크에는 자바 설치가 필요합니다. (ex: Ubuntu에서는 `sudo apt install default-jdk`)  

```sh
# 배포 버전 실행 (리눅스) => releases 확인

# GUI 실행
./mUCodeInterpreter-x86_64.AppImage

# CLI 실행
./mUCodeInterpreter-x86_64.AppImage [uco 파일] [lst 파일]
```

## 개발 일지

- **2025-10-27 ~ 10-31**
  - README 및 회의록 정리
  - 초기 문서/설계 기록

- **2025-11-01 ~ 11-11**
  - 디자인 보고서/문서 추가
  - 초기 개발 방향 정리
  - 초기 자료 구조 코드 구현

- **2025-11-11 ~ 11-19**
  - 어셈블러, runner 구현
  - 11-19 설계 PPT 발표

- **2025-11-20 ~ 11-27**
  - GUI, CLI 구현
  - I/O 처리 및 LST 출력 개선
  - `dup` 명령어 추가
  - snapshot/output_buffer 구조 통합 및 리팩토링

- **2025-11-28**
  - 예제 UCO 테스트 추가: bubble sort, fibonacci, GCD
  - gui 경고 다이얼로그 추가, 실행 라인 표시 버그 수정
  - AppImage 배포
  - 전체 안정화 및 v1.0.1 릴리즈

- **2025-11-30**
  - 벤치마크 모드 추가: `benchRunner`, IO disable 함수 구현
  - 테스트 파일 및 벤치마크 쉘 스크립트 추가

- **2025-12-01**
  - uco 빈줄 처리 및 파일 선택 제한 버그 수정

- **2025-12-02**
  - v1.0.2 릴리즈

- **2025-12-03**
  - 결과 ppt 발표

## 참고자료

- mU-Code의 문제 SWP 교수님 PDF 자료
- 컴퓨터 구조 교수님 PDF 자료
- 리눅스 활용 및 프로그래밍 교수님 PDF 자료