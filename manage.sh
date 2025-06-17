#!/usr/bin/env bash
set -euo pipefail

# Usage function
usage() {
  cat <<EOF
Usage: $0 <action> [options]

actions:
  build        Configure and build the project
  run          Run the built executable

Options for build action:
  --debug        Build in debug mode (Asan, -O0)
  --profile      Build in profile mode (-O2)
  --release      Build in release mode (-O3)
  --test         Build in test mode (Asan, pthread)
  --benchmark    Build in benchmark mode (pthread)
  --reconfigure  Reconfigure Meson (pass --reconfigure to setup)
EOF
  exit 1
}

# Globals
NATIVE_FILE="--native-file=clang-native.ini"
MESON_SETUP_CMD=(meson setup build)
NINJA_CMD=(ninja -C build -j"$(nproc)")

# Lower-level build invoker
env_build() {
  local mode="$1"; shift
  local reconfig_flag="" extra_opts=()

  # Collect extra flags
  for arg in "$@"; do
    if [[ "$arg" == "--reconfigure" ]]; then
      reconfig_flag="--reconfigure"
    else
      extra_opts+=("$arg")
    fi
  done

  # Run Meson setup
  "${MESON_SETUP_CMD[@]}" "$reconfig_flag" \
    -Dmode="$mode" "${extra_opts[@]}" "$NATIVE_FILE"

  # Build
  "${NINJA_CMD[@]}"
}

# Build wrapper
build() {
  # Default empty
  local mode="" opts=()

  # Parse build-specific options
  for arg in "$@"; do
    case "$arg" in
      --debug)      mode=debug;     opts+=("-Db_sanitize=address" "-Db_lundef=false" "-Doptimization=0") ;;
      --profile)    mode=profile;   opts+=("-Doptimization=2") ;;
      --release)    mode=release;   opts+=("-Doptimization=3") ;;
      --test)       mode=test;      opts+=("-Db_sanitize=address" "-Db_lundef=false" "-Doptimization=0") ;;
      --benchmark)  mode=benchmark; opts+=("-Doptimization=3") ;;
      --reconfigure)                opts+=("--reconfigure") ;;
      *) echo "[ERROR] Unkown build option: $arg"; usage ;;
    esac
  done

  # Require a mode
  if [[ -z "$mode" ]]; then
    echo "[ERROR] Please specify a build mode."
    usage
  fi

  # Invoke build
  env_build "$mode" "${opts[@]}"
}

# Run wrapper
run() {
  local exe="build/src/main/chess"

  if [[ ! -x "$exe" ]]; then
    echo "[ERROR] Executable not found: $exe" >&2
    exit 1
  fi

  # Launch
  "$exe"
}

# Main dispatch
if [[ $# -lt 1 ]]; then
  usage
fi

action="$1"; shift

case "$action" in
  build)
    build "$@" ;;
  run)
    if [[ $# -ne 0 ]]; then
      echo "[ERROR] run action does not take any arg" >&2
      usage
    fi
    run ;;
  *)
    echo "[ERROR] Unkown action: $action" >&2
    usage ;;
esac
