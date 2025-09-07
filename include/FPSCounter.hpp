#pragma once
#include <chrono>
#include <iomanip>
#include <iostream>

class FpsCounter
{
  using clock = std::chrono::high_resolution_clock;

  clock::time_point last_report_{ clock::now() };
  int               frames_{ 0 };
  double            last_fps_{ 0.0 };

public:
  // Вызывайте 1 раз на каждый отрисованный кадр
  inline void frame()
  {
    ++frames_;
    auto   now = clock::now();
    double sec = std::chrono::duration< double >( now - last_report_ ).count();
    if( sec >= 1 )
    {
      last_fps_ = frames_ / sec;
      std::cout << "FPS: " << std::fixed << std::setprecision( 1 ) << last_fps_ << '\n';
      frames_      = 0;
      last_report_ = now;
    }
  }

  // Последнее рассчитанное значение (можно вывести в оверлей)
  double last() const { return last_fps_; }

  // Сброс
  void reset()
  {
    frames_      = 0;
    last_fps_    = 0.0;
    last_report_ = clock::now();
  }
};
