#pragma once
#include <string>
#include <vector>
#include "Task.h"

class Storage {
public:
    std::string filename;

    Storage(const std::string& filename);

    std::vector<Task> load();
    void save(const std::vector<Task>& tasks);
};