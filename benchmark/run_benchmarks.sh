#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
C_DIR="$SCRIPT_DIR/c"
PY_DIR="$SCRIPT_DIR/python"
JAVA_DIR="$SCRIPT_DIR/java"
BIN_DIR="$C_DIR/bin"
UCO_DIR="$SCRIPT_DIR/ucode"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="$REPO_ROOT/build"
INTERPRETER_BIN="$BUILD_DIR/ucode_interpreter"

REQUIRED_TOOLS=(gcc javac java python3)
for tool in "${REQUIRED_TOOLS[@]}"; do
  if ! command -v "$tool" >/dev/null 2>&1; then
    echo "[ERROR] Required tool '$tool' not found in PATH" >&2
    exit 1
  fi
done

compile_c_sources() {
  mkdir -p "$BIN_DIR"
  for src in "$C_DIR"/bench_*.c; do
    local name
    name="$(basename "${src%.c}")"
    gcc -O2 "$src" -o "$BIN_DIR/$name"
  done
}

compile_java_sources() {
  (cd "$JAVA_DIR" && javac bench_*.java)
}

ensure_interpreter() {
  if [ ! -x "$INTERPRETER_BIN" ]; then
    echo "[INFO] Building ucode_interpreter..."
    (cd "$REPO_ROOT" && make build >/dev/null)
  fi
}

measure_time() {
  local label=$1
  shift
  local start end elapsed
  start=$(date +%s%N)
  "$@" >/dev/null
  end=$(date +%s%N)
  elapsed=$(python3 - <<PY
start = $start
end = $end
print(f"{(end - start) / 1e9:.6f}")
PY
)
  printf "  %-24s %s s\n" "$label" "$elapsed"
}

compile_c_sources
compile_java_sources
ensure_interpreter

BENCHES=(bubble_sort binary_tree_dfs fibonacci)

for bench in "${BENCHES[@]}"; do
  echo "=== ${bench} ==="
  measure_time "C" "$BIN_DIR/bench_${bench}"
  measure_time "Python" python3 "$PY_DIR/bench_${bench}.py"
  measure_time "Java" java -cp "$JAVA_DIR" "bench_${bench}"
  measure_time "Interpreter" "$INTERPRETER_BIN" --benchmark "$UCO_DIR/bench_${bench}.uco"
  echo
done
