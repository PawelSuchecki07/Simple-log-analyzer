#include "Storage.h"
#include <fstream>
#include <iostream>

Storage::Storage(const std::string& filename) {
    this->filename = filename;
}

std::vector<Task> Storage::load() {
    std::vector<Task> tasks;
    std::ifstream file(filename);

    if (!file.is_open()) {
        return tasks;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        tasks.push_back(Task::fromCSV(line));
    }

    file.close();
    return tasks;
}

void Storage::save(const std::vector<Task>& tasks) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error: cannot open file " << filename << "\n";
        return;
    }

    for (int i = 0; i < tasks.size(); i++) {
        file << tasks[i].toCSV() << "\n";
    }

    file.close();
}