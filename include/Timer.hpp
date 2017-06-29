#ifndef Timer_hpp
#define Timer_hpp

#include <stdio.h>
#include <GLFW/glfw3.h>

class Timer {
public:
  const double FPS = 60.0;
  const double DT = 1.0 / FPS;
  
  double mCurrentTime;
  double mLastTime;
  double mElapsedTime;
  
  Timer() :
    mCurrentTime(glfwGetTime() / 1000.0),
    mLastTime(0.0) {
  }
  
  void update() {
    auto t = glfwGetTime() / 1000.0;
    mLastTime = mCurrentTime;
    mCurrentTime = t;
    mElapsedTime += t;
  }
  
  double getDelta() {
    return mCurrentTime - mLastTime;
  }
};

#endif 
