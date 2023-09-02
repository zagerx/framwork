#include "analog_i2c.h"
int i2c_bus_xfer_read(i2c_dev_t *i2c_dev,i2c_dev_mesg_t *msgs)
{
	return i2c_bitops_bus_xfer_read((i2c_bus_t*)(i2c_dev->i2c_phy),msgs);
}
int i2c_bus_xfer_write(i2c_dev_t *i2c_dev,i2c_dev_mesg_t *msgs)
{
	return i2c_bitops_bus_xfer_write((i2c_bus_t*)(i2c_dev->i2c_phy),msgs);
}
