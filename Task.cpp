#include "TaskManager.h"
#include <iostream>

TaskManager::TaskManager(const std::string& filename) : storage(filename) {
    tasks = storage.load();
}

int getNextId(const std::vector<Task>& tasks) {
    int maxId = 0;
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].id > maxId)
            maxId = tasks[i].id;
    }
    return maxId + 1;
}

void TaskManager::add(const std::string& title, Priority priority) {
    int newId = getNextId(tasks);
    Task t(newId, title, priority);
    tasks.push_back(t);
    storage.save(tasks);
    std::cout << "Added task #" << newId << "\n";
}

void TaskManager::markDone(int id) {
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].id == id) {
            tasks[i].markDone();
            storage.save(tasks);
            std::cout << "Task #" << id << " marked as done\n";
            return;
        }
    }
    std::cout << "Error: task #" << id << " not found\n";
}

void TaskManager::remove(int id) {
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].id == id) {
            tasks.erase(tasks.begin() + i);
            storage.save(tasks);
            std::cout << "Task #" << id << " removed\n";
            return;
        }
    }
    std::cout << "Error: task #" << id << " not found\n";
}

void TaskManager::listAll() {
    if (tasks.size() == 0) {
        std::cout << "No tasks\n";
        return;
    }
    for (int i = 0; i < tasks.size(); i++) {
        tasks[i].print();
    }
}

void TaskManager::listByPriority(Priority priority) {
    bool found = false;
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].priority == priority) {
            tasks[i].print();
            found = true;
        }
    }
    if (!found) std::cout << "No tasks with that priority\n";
}