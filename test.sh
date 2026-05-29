#!/bin/bash

PASS=0
FAIL=0

check() {
    local description=$1
    local expected=$2
    local actual=$3

    if echo "$actual" | grep -q "$expected"; then
        echo "[PASS] $description"
        PASS=$((PASS + 1))
    else
        echo "[FAIL] $description"
        echo "       expected: $expected"
        echo "       got:      $actual"
        FAIL=$((FAIL + 1))
    fi
}

rm -f tasks.csv

# Test 1 — add task
actual=$(./tasks add "Buy milk" --priority low 2>&1)
check "add task" "Added task #1" "$actual"

# Test 2 — mark task as done
./tasks done 1 > /dev/null 2>&1
check "mark done" "\[x\]" "$(./tasks list 2>&1)"

# Test 3 — delete task
./tasks delete 1 > /dev/null 2>&1
check "delete task" "No tasks" "$(./tasks list 2>&1)"

rm -f tasks.csv

echo ""
echo "Results: $PASS passed, $FAIL failed"