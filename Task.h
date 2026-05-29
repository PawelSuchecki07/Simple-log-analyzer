#pragma once
#include <string>

enum class Priority { low, medium, high };

std::string priorityToString(Priority p);
Priority stringToPriority(const std::string& s);

class Task {
public:
    int id;
    std::string title;
    Priority priority;
    bool done;

    Task(int id, const std::string& title, Priority priority);

    void markDone();
    std::string toCSV() const;
    static Task fromCSV(const std::string& line);
    void print() const;
};