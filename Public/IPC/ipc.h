#ifndef __IPC__H
#define __IPC__H

extern unsigned int g_protocol_event;

enum{
    PROTOCOL_CMD_01 = 3,
};
#define GET_IPC_EVENT(event)      (g_protocol_event >> event) & 1
                                    
#define SET_IPC_EVENT(event)      g_protocol_event |= (1<<event) 
                                        
#define CLEAR_IPC_EVENT(event)    g_protocol_event &= ~(1<<event)
#endif