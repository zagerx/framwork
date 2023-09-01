#include "pid.h"

typedef struct pid
{
  float kp;
  float ki;
  float inger;
}pid_t;

void pid_init(pid_t *pid,float kp,float ki)
{
  pid->kp = kp;
  pid->ki = ki;
}

float pid_outcalc(pid_t *pid,float targe,float real)
{
  float err = 0.0f;
  float pid_out = 0.0f;
  err = targe-real;
  pid_out = pid->kp*err + pid->ki;
}







