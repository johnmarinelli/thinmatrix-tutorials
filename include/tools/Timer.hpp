#ifndef Timer_hpp
#define Timer_hpp

#include <stdio.h>
#include <GLFW/glfw3.h>

class Timer {
public:

  Timer();
  void update();
  double getDelta();
  
  const double FPS = 60.0;
  const double DT = 1.0 / FPS;
  
  double mCurrentTime;
  double mLastTime;
  double mElapsedTime;
  
};

#endif 
