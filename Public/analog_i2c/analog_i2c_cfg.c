
#include "analog_i2c.h"
#include "analog_i2c_com.h"

static i2c_bus_info_t* i2c_bus_infoarry[I2C_BUS_MAX] = {
                                            {"i2c_bus_01","A.15","B.10"}     \
                                        };
static i2c_bus_t* i2c_bus_arry[] = {0};

i2c_bus_t* i2c_bus_init(i2c_bus_info_t* info)
{
    /*���߱������ܴ�����*/
    // for (unsigned char i = 0; i < sizeof(i2c_bus_infoarry); i++)
    // {
    //     /* code */
    //     if (!strcmp(i2c_bus_arry[i]->name,info->i2c_name))//�Ѿ�����
    //     {
    //         /* code */
    //         return i2c_bus_arry[i];
    //     }
    // }    
    return i2c_bus_register(info);
}
