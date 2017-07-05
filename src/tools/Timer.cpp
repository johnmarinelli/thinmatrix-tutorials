#include "Timer.hpp"

Timer::Timer() :
  mCurrentTime(glfwGetTime() / 1000.0),
  mLastTime(0.0) {
}

void Timer::update() {
  auto t = glfwGetTime() / 1000.0;
  mLastTime = mCurrentTime;
  mCurrentTime = t;
  mElapsedTime += t;
}

double Timer::getDelta() {
  return mCurrentTime - mLastTime;
}
