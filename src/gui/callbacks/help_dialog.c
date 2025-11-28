#include "gui/gui_callbacks.h"

void onHelpUsage(GtkWidget *widget, gpointer user_data) {
  GuiContext *ctx = getGuiContext();

  GtkWidget *dialog = gtk_dialog_new_with_buttons(
      "GUI 사용법", ctx->main_window, GTK_DIALOG_MODAL, "_Close",
      GTK_RESPONSE_CLOSE, NULL);
  gtk_widget_set_size_request(dialog, 600, 400);

  GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

  GtkWidget *label = gtk_label_new(
      "=== 메뉴 ===\n"
      "▶ Open/Save: .uco 파일 로드/저장\n"
      "▶ Save .lst: .lst 파일 저장\n"
      "▶ Run: 코드를 실행합니다.\n"
      "▶ View 메뉴: 각 패널을 보이기/숨기기 합니다.\n"
      "▶ Help: gui 사용법과 명령어 사용법, 그리고 팀 소개를 "
      "확인할 수 있습니다.\n"
      "\n=== view 확인 ===\n"
      "▶ ucode: 원본 코드를 확인할 수 있습니다.\n"
      "주석은 무시되며 편집할 수 있습니다. 양식을 덜 신경써보세요!\n"
      "insert로 행을 쉽게 추가하고 delete로 쉽게 삭제하세요!\n"
      "▶ assemble: 어셈블 코드를 확인할 수 있습니다.\n"
      "▶ 실행 라인 강조: 어셈블, 원본 코드에서 현재 어디 줄에서\n실행 "
      "중인지 확인 할 수 있습니다.\n"
      "\n=== 특이 기능 ===\n"
      "▶ 파일 입력 버튼: 버튼을 눌러 편하게 uco 파일을 불러오세요!\n"
      "▶ 슬라이더와 이동 버튼: 원하는 변경점으로 이동합니다.\n"
      "▶ Registers: 레지스터 상태를 확인합니다.\n"
      "▶ Cpu stack: cpu stack에 저장된 상태를 확인합니다.\n"
      "▶ Memory: memory의 stack부분의 상태를 확인합니다.\n"
      "▶ Labels: 라벨 할당을 확인합니다.\n"
      "▶ Symbols: 심볼(변수) 할당을 확인합니다.\n"
      "\n=== 통계 ===\n"
      "▶ 입출력: 실행 중 발생하는 입력과 출력을 확인합니다.\n"
      ">> 가 보이면 꼭 입력해주세요! 출력은 << 로 나옵니다.\n"
      "▶ 결과 통계: 실행 완료 후 통계를 확인합니다.\n"
      "▶ lst 출력: 실행 완료 후 생성된 lst를 확인합니다. 저장되지 않습니다.\n");

  gtk_label_set_xalign(GTK_LABEL(label), 0.0);
  gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
  gtk_container_set_border_width(GTK_CONTAINER(box), 15);
  gtk_box_pack_start(GTK_BOX(box), label, TRUE, TRUE, 0);

  gtk_container_add(GTK_CONTAINER(content), box);

  gtk_widget_show_all(dialog);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

static void add_header_row(GtkGrid *grid, int *row, const char *text) {
  GtkWidget *label = gtk_label_new(text);
  gtk_widget_set_halign(label, GTK_ALIGN_START);
  gtk_widget_set_margin_top(label, 10);
  gtk_widget_set_margin_bottom(label, 4);
  // 전체 5열 span
  gtk_grid_attach(grid, label, 0, *row, 5, 1);
  (*row)++;
}

static void add_row(GtkGrid *grid, int *row, const char *op,
                    const char *meaning, const char *action) {
  GtkWidget *l_op = gtk_label_new(op);
  GtkWidget *l_meaning = gtk_label_new(meaning);
  GtkWidget *l_action = gtk_label_new(action);

  gtk_widget_set_halign(l_op, GTK_ALIGN_START);
  gtk_widget_set_halign(l_meaning, GTK_ALIGN_START);
  gtk_widget_set_halign(l_action, GTK_ALIGN_START);

  gtk_label_set_line_wrap(GTK_LABEL(l_op), TRUE);
  gtk_label_set_line_wrap(GTK_LABEL(l_meaning), TRUE);
  gtk_label_set_line_wrap(GTK_LABEL(l_action), TRUE);

  gtk_grid_attach(grid, l_op, 0, *row, 1, 1);
  gtk_grid_attach(grid, l_meaning, 2, *row, 1, 1);
  gtk_grid_attach(grid, l_action, 4, *row, 1, 1);

  // op가 비어있으면 separator 안 넣기
  if (op[0] != '\0') {
    GtkWidget *sep1 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_grid_attach(grid, sep1, 1, *row, 1, 1);
    GtkWidget *sep2 = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_grid_attach(grid, sep2, 3, *row, 1, 1);
    GtkWidget *hsep = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
    gtk_grid_attach(grid, hsep, 0, *row + 1, 5, 1);
    (*row) += 2;
  } else {
    (*row)++;
  }
}

void onHelpInstruction(GtkWidget *widget, gpointer user_data) {
  GuiContext *ctx = getGuiContext();

  GtkWidget *dialog = gtk_dialog_new_with_buttons(
      "명령어 설명", ctx->main_window, GTK_DIALOG_MODAL, "_Close",
      GTK_RESPONSE_CLOSE, NULL);

  GtkWidget *content = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

  GtkWidget *scroll = gtk_scrolled_window_new(NULL, NULL);
  gtk_widget_set_size_request(scroll, 650, 450);
  gtk_container_add(GTK_CONTAINER(content), scroll);

  GtkWidget *grid = gtk_grid_new();
  gtk_grid_set_row_spacing(GTK_GRID(grid), 4);
  gtk_grid_set_column_spacing(GTK_GRID(grid), 20);
  gtk_container_add(GTK_CONTAINER(scroll), grid);

  int row = 0;

  add_header_row(GTK_GRID(grid), &row, "[[ 프로그램 구성 명령 ]]");
  add_row(GTK_GRID(grid), &row, "명령어", "의미", "동작");
  add_row(GTK_GRID(grid), &row, "nop", "no operation",
          "아무 작업도 수행하지 않으며 주로 레이블 위치에 사용됨.");
  add_row(GTK_GRID(grid), &row, "bgn n", "begin",
          "프로그램의 시작점. n은 전역 변수의 총량");
  add_row(GTK_GRID(grid), &row, "sym b n s", "symbol",
          "변수(심볼)가 속한 블록(b)과 블록 내에서의 오프셋(n) 및 크기(s) "
          "표현\n  - 전역 변수의 블록 번호는 1, 지역 변수의 블록 번호는 2~ "
          "(함수마다 별도 번호)\n  - 오프셋은 0부터 시작- 일반 정수형은 크기가 "
          "1\n  - 배열형은 배열의 크기만큼 변수 선언부를 어셈블리어로 표현한 "
          "것으로, 실제로는 사용되지 않고 인간의 이해를 돕는 코드");
  add_row(GTK_GRID(grid), &row, "end", "end",
          "프로그램의 끝. 어셈블을 종료하는 역할. global frame의 ret 취급.");

  add_header_row(GTK_GRID(grid), &row, "[[ 입출력 처리 ]]");
  add_row(GTK_GRID(grid), &row, "시스템 함수", "사용 예", "동작");
  add_row(GTK_GRID(grid), &row, "read(i)", "ldp\nlda 2 0\npush\ncall read",
          "외부 입력값을 읽어 스택 꼭대기에 저장된 주소로 저장한다.");
  add_row(
      GTK_GRID(grid), &row, "write(i)", "ldp\nlod 2 0\npush\ncall write",
      "스택 꼭대기의 값을 출력한다. 출력된 값의 뒤에 공백을 추가로 출력한다.");

  add_header_row(GTK_GRID(grid), &row, "[[ 함수 관련 명령 ]]");
  add_row(GTK_GRID(grid), &row, "명령어", "의미", "동작");
  add_row(GTK_GRID(grid), &row, "proc n", "procedure",
          "함수의 시작을 나타내며, n은 매개 변수를 포함하여 함수가 사용하는 "
          "모든 지역 변수의 총량");
  add_row(GTK_GRID(grid), &row, "ret", "return",
          "함수를 종료하고 복귀. 반환값이 있을 때는 ret 전에 CPU 스택에 "
          "저장해야 한다.");
  add_row(GTK_GRID(grid), &row, "ldp", "load parameters",
          "함수의 실인자들을 스택에 저장. 함수를 호출하기 전에 함수가 사용할 "
          "메모리 영역을 설정하고 매개변수들을 전달할 준비를 한다.");
  add_row(GTK_GRID(grid), &row, "push", "push parameters",
          "CPU 스택에 올려져 있는 실인자 값을 메모리 스택에 저장한다.");
  add_row(GTK_GRID(grid), &row, "call label", "call",
          "label로 지정된 함수를 호출");

  add_header_row(GTK_GRID(grid), &row, "[[ 흐름 제어 ]]");
  add_row(GTK_GRID(grid), &row, "명령어", "의미", "동작");
  add_row(GTK_GRID(grid), &row, "ujp label", "unconditional jump",
          "지정한 label로 무조건 이동");
  add_row(GTK_GRID(grid), &row, "tjp label", "jump on true",
          "stack[top]의 값이 참이면 label로 이동");
  add_row(GTK_GRID(grid), &row, "fjp label", "jump on false",
          "stack[top]의 값이 거짓이면 label로 이동");

  add_header_row(GTK_GRID(grid), &row, "[[ 데이터 이동 연산자 ]]");
  add_row(GTK_GRID(grid), &row, "명령어", "의미", "동작");
  add_row(GTK_GRID(grid), &row, "lob b n", "load",
          "b 블록 n 오프셋의 데이터를 스택에 넣는다. 즉, b와 n으로 계산되는 "
          "주소에 있는 변수의 값이 스택에 저장된다.");
  add_row(GTK_GRID(grid), &row, "lda b n", "load address",
          "b 블록 n 오프셋의 실제 메모리 번지를 스택에 넣는다. 즉, b와 n으로 "
          "계산되는 주소 자체가 스택에 저장되며, 배열 참조를 위해 활용된다.");
  add_row(GTK_GRID(grid), &row, "ldc c", "load contant",
          "상수값 c가 스택에 저장된다.");
  add_row(GTK_GRID(grid), &row, "str b n", "store",
          "스택 꼭대기의 값을 pop하여 b와 n으로 계산되는 주소의 메모리에 "
          "저장한다.");
  add_row(GTK_GRID(grid), &row, "ldi", "load indirect",
          "간접 주소법을 이용해 메모리의 값을 스택에 가져 온다. 스택 꼭대기의 "
          "값을 pop하여 주소값으로 사용하고, 데이터를 스택에 저장한다.");
  add_row(GTK_GRID(grid), &row, "sti", "store indirect",
          "간접 주소법을 이용해 스택 꼭대기의 값을 메모리에 저장한다. 저장할 "
          "변수의 주소와 저장할 값, 두개가 스택에서 pop된다.");
  add_row(GTK_GRID(grid), &row, "dup", "duplicate",
          "스택 꼭대기의 값을 복사해 다시 스택 꼭대기에 저장한다.");

  add_header_row(GTK_GRID(grid), &row, "[[ 이항 연산자 ]]");
  add_row(GTK_GRID(grid), &row, "명령어", "의미", "동작");
  add_row(GTK_GRID(grid), &row, "add", "add",
          "stack[top-1] = stack[top-1] + stack[top]");
  add_row(GTK_GRID(grid), &row, "sub", "subtract",
          "stack[top-1] = stack[top-1] - stack[top]");
  add_row(GTK_GRID(grid), &row, "mult", "multiply",
          "stack[top-1] = stack[top-1] * stack[top]");
  add_row(GTK_GRID(grid), &row, "div", "divide",
          "stack[top-1] = stack[top-1] / stack[top]");
  add_row(GTK_GRID(grid), &row, "mod", "modulo",
          "stack[top-1] = stack[top-1] % stack[top]");
  add_row(GTK_GRID(grid), &row, "gt", "greater than",
          "stack[top-1] = stack[top-1] > stack[top]");
  add_row(GTK_GRID(grid), &row, "lt", "less than",
          "stack[top-1] = stack[top-1] < stack[top]");
  add_row(GTK_GRID(grid), &row, "ge", "gt or equal",
          "stack[top-1] = stack[top-1] >= stack[top]");
  add_row(GTK_GRID(grid), &row, "le", "lt or equal",
          "stack[top-1] = stack[top-1] <= stack[top]");
  add_row(GTK_GRID(grid), &row, "eq", "equal",
          "stack[top-1] = stack[top-1] == stack[top]");
  add_row(GTK_GRID(grid), &row, "ne", "not equal",
          "stack[top-1] = stack[top-1] != stack[top]");
  add_row(GTK_GRID(grid), &row, "and", "and",
          "stack[top-1] = stack[top-1] && stack[top]");
  add_row(GTK_GRID(grid), &row, "or", "or",
          "stack[top-1] = stack[top-1] || stack[top]");

  add_header_row(GTK_GRID(grid), &row, "[[ 이항 연산자 ]]");
  add_row(GTK_GRID(grid), &row, "명령어", "의미", "동작");
  add_row(GTK_GRID(grid), &row, "not", "not", "stack[top-1] = !stack[top]");
  add_row(GTK_GRID(grid), &row, "neg", "negation",
          "stack[top-1] = -stack[top]");

  gtk_widget_show_all(dialog);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void onHelpAbout(GtkWidget *widget, gpointer user_data) {
  GuiContext *ctx = getGuiContext();

  const char *authors[] = {"이강석", "권기영", NULL};

  GtkWidget *about = gtk_about_dialog_new();
  gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(about),
                                    "mUCode Interpreter");
  gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(about),
                               "학부 소프트웨어 프로젝트");
  gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(about),
                                "컴퓨터공학과 학부 과제용으로 제작된\n"
                                "어셈블리/가상머신 GUI 시뮬레이터입니다.");
  gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(about), authors);

  gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(about),
                                 "© 2025 mUCode Interpreter Project");

  gtk_about_dialog_set_website_label(
      GTK_ABOUT_DIALOG(about),
      "https://github.com/jhan117/mU-Code-Interpreter");

  gtk_dialog_run(GTK_DIALOG(about));
  gtk_widget_destroy(about);
}
