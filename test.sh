#!/bin/bash
 
PASS=0
FAIL=0
 
check() {
    local test_name=$1
    local input=$2
    local expected=$3
 
    echo "$input" > /tmp/test_input.log
    actual=$(./log_analyzer /tmp/test_input.log)
 
    if echo "$actual" | grep -q "$expected"; then
        echo "PASS: $test_name"
        PASS=$((PASS + 1))
    else
        echo "FAIL: $test_name"
        echo "  expected to find: $expected"
        FAIL=$((FAIL + 1))
    fi
}
 
check "valid ERROR detected" \
    "2024-01-15 08:25:30 [ERROR] (192.168.1.10) something broke" \
    "Errors: 1"
 
check "valid WARNING detected" \
    "2024-01-15 08:25:30 [WARNING] (10.0.0.1) high cpu usage" \
    "Warnings: 1"
 
check "INFO line is ignored" \
    "2024-01-15 08:25:30 [INFO] (10.0.0.1) all good" \
    "Errors: 0"
 
check "invalid IP flagged as malformed" \
    "2024-01-15 08:25:30 [ERROR] (999.0.0.1) bad ip" \
    "Malformed: 1"
 
check "invalid timestamp flagged as malformed" \
    "2024-01-15 99:99:99 [ERROR] (10.0.0.1) bad time" \
    "Malformed: 1"
 
check "entry without IP still works" \
    "2024-01-15 08:25:30 [WARNING] no ip in this line" \
    "Warnings: 1"
 
check "empty file gives zero counts" \
    "" \
    "Errors: 0"
 
echo ""
echo "Results: $PASS passed, $FAIL failed"
