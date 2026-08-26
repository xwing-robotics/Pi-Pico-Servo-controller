/*
  PWM servo control, max 16 servos, for RP2040 and RP23050.
  Resolution 310 ns with 20 ms (50 Hz) frame rate.
  Resolution 50 ns with 3.33 ms (300 Hz) frame rate.

  Please refer to the RP240 and RP2350 PWM slice table for pin assignments.
  Frame rate any value between 50 and 300 Hz.

  Max resolution servo pulse:
  50  Hz  0.31 us
  100 Hz  0.15 us
  150 Hz  0.10 us
  200 Hz  0.08 us
  300 Hz  0.05 us

  v 1.0.0 build 26/08/2026

  Warning:
  Uses pico-sdk features, use only with Earle philhower core.

  (c) 2026 by Xwing
  Private and educational use permitted.
  Commercial use prohibited. 
*/

/*
MIT License
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
//Maximum number of PWM outputs
#define PWM_MAX 16

#include <hardware/pwm.h>


// WIP, doming soon


