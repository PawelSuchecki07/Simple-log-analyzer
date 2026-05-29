# Task Manager CLI

A simple command-line task manager written in C++17. Tasks are stored locally in a CSV file. No external libraries required.

## Features

- Add tasks with a priority level (low / medium / high)
- List all tasks
- Mark a task as done
- Delete a task
- Filter tasks by priority

## Build

```bash
g++ -std=c++17 -o tasks main.cpp Task.cpp TaskManager.cpp Storage.cpp
```

## Usage

```bash
./tasks add "Buy milk" --priority low
./tasks add "Write tests"              # default priority: medium
./tasks list
./tasks done 1
./tasks filter --priority low
./tasks delete 2
```

## Example output

```
[ ] #1 [high] Write unit tests
[x] #2 [low]  Buy milk
[ ] #3 [medium] Review PR
```

## Project structure

```
.
├── main.cpp          # CLI interface, parses commands and flags
├── Task.h / .cpp     # Data model, CSV serialization
├── Storage.h / .cpp  # File read/write
├── TaskManager.h / .cpp  # Business logic (add, done, delete, filter)
├── sample.log        # Example input
└── test.sh           # Shell-based test suite
```

## Tests

```bash
chmod +x test.sh && ./test.sh
```

## Tech

- C++17
- STL only — `std::vector`, `std::fstream`, `std::stringstream`
- No external dependencies
