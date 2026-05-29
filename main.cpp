#include <iostream>
#include <string>
#include "TaskManager.h"

void printHelp() {
    std::cout << "Usage:\n";
    std::cout << "  ./tasks add <title> [--priority low|medium|high]\n";
    std::cout << "  ./tasks list\n";
    std::cout << "  ./tasks filter --priority low|medium|high\n";
    std::cout << "  ./tasks done <id>\n";
    std::cout << "  ./tasks delete <id>\n";
}

std::string getFlag(int argc, char* argv[], const std::string& flag) {
    for (int i = 1; i < argc - 1; i++) {
        if (std::string(argv[i]) == flag)
            return std::string(argv[i + 1]);
    }
    return "";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 1;
    }

    TaskManager manager("tasks.csv");
    std::string command = argv[1];

    if (command == "add") {
        if (argc < 3) {
            std::cout << "Error: missing task title\n";
            return 1;
        }
        std::string title = argv[2];
        std::string priorityStr = getFlag(argc, argv, "--priority");
        Priority priority = Priority::medium;
        if (!priorityStr.empty())
            priority = stringToPriority(priorityStr);
        manager.add(title, priority);

    } else if (command == "list") {
        manager.listAll();

    } else if (command == "filter") {
        std::string priorityStr = getFlag(argc, argv, "--priority");
        if (priorityStr.empty()) {
            std::cout << "Error: missing --priority flag\n";
            return 1;
        }
        manager.listByPriority(stringToPriority(priorityStr));

    } else if (command == "done") {
        if (argc < 3) {
            std::cout << "Error: missing task id\n";
            return 1;
        }
        manager.markDone(std::stoi(argv[2]));

    } else if (command == "delete") {
        if (argc < 3) {
            std::cout << "Error: missing task id\n";
            return 1;
        }
        manager.remove(std::stoi(argv[2]));

    } else {
        std::cout << "Unknown command: " << command << "\n";
        printHelp();
        return 1;
    }

    return 0;
}