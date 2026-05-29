# Log Analyzer

A simple C++ CLI tool that reads a `.log` file, filters ERROR and WARNING entries, validates timestamps and IP addresses, and prints a summary.

## Build & run

```bash
g++ -o log_analyzer main.cpp
./log_analyzer sample.log
```

## Log format

```
YYYY-MM-DD HH:MM:SS [LEVEL] (IP) message
```

IP is optional. Level can be `INFO`, `WARNING`, or `ERROR`.


## Tests
```bash
chmod +x test.sh && ./test.sh
```