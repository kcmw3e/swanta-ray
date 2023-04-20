// Casey Walker (2023)
// Team Swanta Ray Team
// 24-775 Bioinspired Robot Design and Experimentation
// Carnegie Mellon University
// -------------------------------------------------------------------------- //

#include <Arduino_DebugUtils.h>

#include "cal.h"


Task::Task(task_fn* fn, size_t dt) {
  _fn = fn;
  _dt = dt;
  _t = millis();
}

void Task::operator()(uint32_t t) {
  if (t - _t > _dt) {
    _t = t;
    _fn();
  }
}

Cal::Cal() {
  _t = millis();
}

bool Cal::setup() {
  return true;
}

void Cal::tick() {
  _t = millis();
  for (size_t i = 0; i < _tasks.size(); i++) {
    Task task = _tasks[i];
    task(_t);
  }
}

void Cal::add(task_fn* fn, uint32_t dt) {
  Task task(fn, dt);
  _tasks.push_back(task);
}
