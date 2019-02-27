#!/usr/bin/env bash
script_dir=$(dirname ${BASH_SOURCE[0]})
export PATH=$script_dir/bin:$script_dir/libexec:$PATH
set -eEu -o pipefail
$script_dir/cce.bats "$@"
