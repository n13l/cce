fixtures() {
  FIXTURE_DIR="$BATS_TEST_DIRNAME/fixtures/$1"
  RELATIVE_FIXTURE_ROOT="$(bats_trim_filename "$FIXTURE_DIR")"
}

setup() {
  export TMP="$BATS_TEST_DIRNAME/tmp"
  DATA_DIR=`mktemp -d`
  DEC=./obj/tools/decrypt
}

filter_control_sequences() {
  "$@" | sed $'s,\x1b\\[[0-9;]*[a-zA-Z],,g'
}

teardown() {
  [ -d "$TMP" ] && rm -f "$TMP"/*
  rm -fr $DATA_DIR
}

flunk() {
  { if [ "$#" -eq 0 ]; then cat -
    else echo "$@"
    fi
  } | sed "s:${BATS_TEST_DIRNAME}:TEST_DIR:g" >&2
  return 1
}

assert_success() {
  if [ "$status" -ne 0 ]; then
    flunk "command failed with exit status $status"
  elif [ "$#" -gt 0 ]; then
    assert_output "$1"
  fi
}

assert_failure() {
  if [ "$status" -eq 0 ]; then
    flunk "expected failed exit status"
  elif [ "$#" -gt 0 ]; then
    assert_output "$1"
  fi
}

assert_equal() {
  if [ "$1" != "$2" ]; then
    { echo "expected: $1"
      echo "actual:   $2"
    } | flunk
  fi
}

assert_output() {
  local expected
  if [ $# -eq 0 ]; then expected="$(cat -)"
  else expected="$1"
  fi
  assert_equal "$expected" "$output"
}

make_modified_in_past() {
  local current_timestamp=$(date +%Y%m%d%H%M)
  local past_timestamp=$(expr $current_timestamp - 10)
  touch -t $past_timestamp "$1"
}
