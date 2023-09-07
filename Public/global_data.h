#ifndef __GLOBAL_DATA__H
#define __GLOBAL_DATA__H


enum{
  E_KEY_EVENT_IDLE,
  E_KEY_EVENT_HOLD,
};
void set_keysensor_data(unsigned char data);
unsigned char get_keysensor_data(void);
#endif
