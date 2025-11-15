# 회의록

- [회의록](#회의록)
	- [2025-10-27 (수업 시간)](#2025-10-27-수업-시간)
	- [2025-10-31 (온라인)](#2025-10-31-온라인)
		- [회의 중 기록된 노트](#회의-중-기록된-노트)
	- [2025-11-05 (오후 4시 강의실)](#2025-11-05-오후-4시-강의실)

## 2025-10-27 (수업 시간)
- 주요 논의사항
	- 주장은 이강석, 팀명은 UCI
	- GUI 기능을 추가하기로 결정
	- GitHub로 협업하기로 결정
	- 리눅스, gtk, C언어를 사용하기로 결정
- 할 일
	- [x] 권기영: github repo 생성

## 2025-10-31 (온라인)
- 주요 논의사항
	- 보고서 작성 방식 공유하며 통일
	- 문제 해결 범위: 어셈블러, 인터프리터, GUI 기능 구현 결정
	- 요구사항 내용 결정: 입출력, 제한 요소, use case 정의
	- 설계 내용 결정
		- 큰 모듈 구조 설계: assembler, runner, gui, io_utils
		- assembler: 
			- 명령어 형식: 컴퓨터 구조에서 배운대로 명령어는 10진수 명령어 + operand 로 결정
			- 레이블 처리:
				- 레이블은 사전 처리 방식 채택 (공간은 더 들지만 코드 간결성 확보)
				- 레이블 구조체 설계:
					- 필드: 레이블 이름, 호출된 위치 배열, 명령어 위치 주소
					- 구조체 배열로 관리
					- 레이블만 먼저 수집 -> 호출 시 주소 없으면 배열에 저장 -> 레이블 발견 시 처리
		- runner: ALU(이항, 단항), CU(흐름 제어, 함수, 데이터 이동), system(시스템 함수)
			- 조건문을 줄이고 기능을 분리하기 위해 배열에 범위별 함수 포인터를 넣어 호출하기
		- gui: layout(배치), widgets(위젯), callbacks(이벤트 콜백)
			- 기능 목록
				- 실행(한 줄씩, 함수별, 전체) + 실행 중인 라인 표시
				- 파일 입출력
				- 통계 자료 출력 (명령어 사용/실행 횟수, 메모리 접근 횟수, register, cpu stack/stack, label 위치)
				- 어셈블러 결과, 실행 결과 출력
		- 자료 구조 설계: code, data, stack 영역 배열은 한 덩어리로 cpu stack은 별개의 배열로
			- cs(프로그램이 하나지만 유연성을 위해), ds, ss, bp, sp 레지스터 사용
	- 역할 분담
		- 이강석: ALU 제외한 runner, .lst 파일 생성
		- 권기영: Assembler, ALU, .uco 파일 입력, GUI
	- branch 규정
		- main: 안정화 된 버전만, push 금지!
		- develop: PR을 통해서만 merge (코드 리뷰 필수)
		- feature: 자유롭게 (ex: feature/assembler) 작성 후 develop으로 pr 부탁
	- 코드 규칙
		- 파일명, 변수명: snake_case
		- 함수명: camelCase
	- 기타 사항: 참고문헌 및 AI 사용 기록은 사전에 문서화 필수
	- 할 일
		- [x] 각자 맡은 역할에 대한 문서 초안 작성 (설계는 flowchart로 use case 필수)
		- [x] 이강석: 문제 개요, 전체 구조 문서
		- [x] 권기영: 문제 요구사항

### 회의 중 기록된 노트

<div align="center">
	<img src="https://github.com/user-attachments/assets/9889f7d9-eeb3-4e91-9c01-cf256284ebf4" width="600"/>
	<p><em>자료 구조 설계</em></p>
</div>

<div align="center">
	<img src="https://github.com/user-attachments/assets/aaaa23d5-a161-4e02-af7b-4cd5b8f68d6b" width="650"/>
	<p><em>어셈블러 단계 설계</em></p>
</div>

<div align="center">
	<img src="https://github.com/user-attachments/assets/b6329d42-d3b5-40b8-a649-4e59739b6ac8" width="1200"/>
	<p><em>실행 단계 설계</em></p>
</div>

<div align="center">
	<img src="https://github.com/user-attachments/assets/18a95c38-d0ba-416f-adce-9b41661fcd52" width="1200"/>
	<p><em>전체 모듈 구조 설계 및 역할 분담</em></p>
</div>

## 2025-11-05 (오후 4시 강의실)
- 회의 전 결정사항(카톡)
  - C 코드 포맷팅: Clang-Format
  - 명령어 형식 재정의: opcode 6비트, 나머지 비트는 operand → opcode 그룹화
  - GUI 참조: Python Tutor 유사, 실행 흐름 시각화
  - GUI 실행 흐름 논리 논의
    - snapshot vs 변경점만 기록
    - 절충안으로 택함 N단계마다 snapshot 저장 + 나머지는 변경점 기록
- 주요 논의사항
  - 설계서 쓸 부분 역할 분담
    - 이강석: 첫 페이지, 서론 작성, 대체 시나리오(시퀀스/Use Case별), 설계
		- 권기영: 성공 시나리오(시퀀스/Use Case별), 요구사항 분석, 설계
  - 추진 전략 결정
    - 역할 분담 및 개발 도구 확정
    - 개발 절차: 문제 분석 → 설계 → 구현
    - 정보 수집: PDF, AI, 구글링, 공식 문서(GTK)
    - 개발 방법론: 폭포수 모델(순차적 진행)
  - 수행 일정 결정
    - 기획 및 상세 설계: 1~2주 (11/09)
    - 어셈블러 구현: 2주
    - 실행기 구현: 2~3주
    - 테스트 코드 구현: 2~3주
    - CLI 임시 구현: 2~3주
    - GUI 구현: 3주
    - 결과 보고서 작성: 4주
  - ppt 발표 부분 및 목차 결정
    - 서론
    - 시나리오
    - (이강석 발표)
    - 요구사항 분석
    - 전체 모듈 설계
    - 자료 구조 설계
    - 알고리즘 설계
      - (어셈블러 --> 권기영)
      - (실행기 --> 이강석)
    - GUI 설계
    - 일정표
    - (권기영 발표)
