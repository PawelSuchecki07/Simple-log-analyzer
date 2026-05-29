#pragma once
#include <vector>
#include <string>
#include "Task.h"
#include "Storage.h"

class TaskManager {
public:
    std::vector<Task> tasks;
    Storage storage;

    TaskManager(const std::string& filename);

    void add(const std::string& title, Priority priority);
    void markDone(int id);
    void remove(int id);
    void listAll();
    void listByPriority(Priority priority);
};