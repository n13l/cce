#!/usr/bin/env bats
load support/helper

@test "tester works as expected" {
  skip
  run ./obj/tools/tester
  assert_success
}
