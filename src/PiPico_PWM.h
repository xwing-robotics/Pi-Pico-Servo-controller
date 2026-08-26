/*
  PWM servo control, max 16 servos, for RP2040 and RP23050
  Resolution 200 ns with 10 ms (100 Hz) frame rate.
  Please refer to the RP240 and RP2350 PWM slice table for pin assignments.
  Frame rate any value between 25 and 200 Hz.

  Max resolution servo pulse:
  25  Hz  0.5 us
  50  Hz  0.4 us
  100 Hz  0.3 us
  150 Hz  0.1 us
  200 Hz  0.1 us

  Warning:
  Uses pico-sdk features, use only with Earle philhower core-

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

class PWM {
public:
  PWM(){};
  bool begin(int pin, int freq, float min_us, float max_us) {
    if (freq > 200) freq = 200;
    if (freq < 25) freq = 25;
    this->pin = pin;
    this->req_freq = freq;
    this->min_us = min_us;
    this->max_us = max_us;

    //find divider so that full 16bit count results in freq
    this->max_duty = (1 << 16) - 1;
    float divider = clock_get_hz(clk_sys) / (max_duty + 1) / freq;
    this->act_freq = freq;
    this->inv_duty_resolution_us = 1.0e-6 * act_freq * (max_duty + 1);

    // get slice number
    this->slicenum = pwm_gpio_to_slice_num(pin);

    // assign GPIO to pwm functionality
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // set the clkdiv mode (this might not actually do anything)
    pwm_set_clkdiv_mode(slicenum, PWM_DIV_FREE_RUNNING);

    // disable phase correct (if enabled, frequency is halved and duty cycle is doubled)
    pwm_set_phase_correct(slicenum, false);

    // set clkdiv to system clock in Mhz (125Mhz clock = clkdiv of 125)
    // makes it so that our on/off threshold on each pwm channel is equal to the on time per cycle in microseconds
    pwm_set_clkdiv(slicenum, divider);

    // set wrap to full scale for maximum resolution
    pwm_set_wrap(slicenum, max_duty);

    // set count
    pwm_set_gpio_level(pin, 0);

    // enable PWM slice
    pwm_set_enabled(slicenum, true);

    return true;
  };

  void writeFactor(float f) {
    writeMicroseconds(min_us + f * (max_us - min_us));
  }

  void writeMicroseconds(float us) {
    if (us < min_us) us = min_us;
    if (us > max_us) us = max_us;
    int duty = us * inv_duty_resolution_us;
    if (duty < 0) duty = 0;
    if (duty > max_duty) duty = max_duty;
    pwm_set_gpio_level(pin, us * inv_duty_resolution_us);
  };

  int get_slicenum() {
    return slicenum;
  }
  float get_min_us() {
    return min_us;
  }
  float get_max_us() {
    return max_us;
  }
  float get_duty_resolution_us() {
    return 1 / inv_duty_resolution_us;
  }
  int get_req_freq() {
    return req_freq;
  }
  int get_act_freq() {
    return act_freq;
  }

private:
  int pin;
  int slicenum;
  int bits;
  int max_duty;
  float min_us;
  float max_us;
  int req_freq;  //requested frequency
  int act_freq;  //actual frequency
  float inv_duty_resolution_us;
};

