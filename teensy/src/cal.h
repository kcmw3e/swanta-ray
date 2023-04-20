// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //
//
// cal.h
// For scheduling
//

#ifndef SWANTA_CAL_H
#define SWANTA_CAL_H

#include <vector>

#include <Arduino.h>

using namespace std;

typedef void task_fn();

class Task {
  private:
    task_fn* _fn;
    uint32_t _dt; // milliseconds
    uint32_t _t;
  public:
    Task(task_fn* fn, size_t dt);
    void operator()(uint32_t);
};

class Cal {
  private:
    uint32_t _t;
    vector<Task> _tasks;
  public:
    Cal();
    bool setup();
    void tick();
    void add(task_fn* task, uint32_t dt);
};

#endif // SWANTA_CAL_H
