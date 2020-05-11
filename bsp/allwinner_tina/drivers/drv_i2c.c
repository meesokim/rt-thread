#include <rtthread.h>
#include "drv_i2c.h"

enum {
	I2C_ADDR			= 0x000,
	I2C_XADDR			= 0x004,
	I2C_DATA 			= 0x008,
	I2C_CNTR			= 0x00c,
	I2C_STAT			= 0x010,
	I2C_CCR				= 0x014,
	I2C_SRST			= 0x018,
	I2C_EFR				= 0x01c,
	I2C_LCR				= 0x020,
};

enum {
	I2C_STAT_BUS_ERROR	= 0x00,
	I2C_STAT_TX_START	= 0x08,
	I2C_STAT_TX_RSTART	= 0x10,
	I2C_STAT_TX_AW_ACK	= 0x18,
	I2C_STAT_TX_AW_NAK	= 0x20,
	I2C_STAT_TXD_ACK	= 0x28,
	I2C_STAT_TXD_NAK	= 0x30,
	I2C_STAT_LOST_ARB	= 0x38,
	I2C_STAT_TX_AR_ACK	= 0x40,
	I2C_STAT_TX_AR_NAK	= 0x48,
	I2C_STAT_RXD_ACK	= 0x50,
	I2C_STAT_RXD_NAK	= 0x58,
	I2C_STAT_IDLE		= 0xf8,
};

#ifdef RT_USING_I2C


struct f1c100s_i2c_bus
{
	struct rt_i2c_bus_device parent;
	rt_uint32_t addr;
	rt_uint32_t pclk;
	char *device_name;
};

static rt_size_t f1c100s_master_xfer(struct rt_i2c_bus_device *bus, struct rt_i2c_msg msgs[], rt_uint32_t num);

const struct rt_i2c_bus_device_ops i2c_ops =
{
    f1c100s_master_xfer,
    RT_NULL,
    RT_NULL,
};

struct i2c_f1c100s {
	rt_uint32_t reg;
};

static int f1c100s_i2c_start(rt_uint32_t addr)
{
	return 0;
}

static void f1c100s_i2c_stop(rt_uint32_t addr)
{

}

static int f1c100s_i2c_read(rt_uint32_t addr, struct rt_i2c_msg *msg)
{
	return 0;
}

static int f1c100s_i2c_write(rt_uint32_t addr, struct rt_i2c_msg *msg)
{
	return 0;
}

static rt_size_t f1c100s_master_xfer(struct rt_i2c_bus_device *bus, struct rt_i2c_msg msgs[], rt_uint32_t num)
{
	struct rt_i2c_msg *msg;
	rt_uint32_t i;
	rt_err_t ret = RT_ERROR;
	rt_size_t res;
	struct f1c100s_i2c_bus *i2c = (struct f1c100s_i2c_bus *)bus;

	if (!msgs || num <= 0)
		return 0;
	if (f1c100s_i2c_start(i2c->addr) != I2C_STAT_TX_START)
		return 0;

	for (i = 0; i < num; i++)
	{
		msg = &msg[i];
		if (i != 0)
		{
			if (f1c100s_i2c_start(i2c->addr) != I2C_STAT_TX_START)
				break;
		}
		if (msg->flags & RT_I2C_RD)
			res = f1c100s_i2c_read(i2c->addr, msg);
		else
		{
			res = f1c100s_i2c_write(i2c->addr, msg);
		}
		
	}
}
static void f1c100s_i2c_set_rate(struct f1c100s_i2c_bus * bus, rt_uint64_t rate)
{
	rt_uint64_t pclk = bus->pclk;
	rt_int64_t freq, delta, best = 0x7fffffffffffffffLL;
	int tm = 5, tn = 0;
	int m, n;

	for(n = 0; n <= 7; n++)
	{
		for(m = 0; m <= 15; m++)
		{
			freq = pclk / (10 * (m + 1) * (1 << n));
			delta = rate - freq;
			if(delta >= 0 && delta < best)
			{
				tm = m;
				tn = n;
				best = delta;
			}
			if(best == 0)
				break;
		}
	}
	writel(bus->addr + I2C_CCR, ((tm & 0xf) << 3) | ((tn & 0x7) << 0));
}

int rt_hw_i2c_init(void)
{
#ifdef RT_USING_I2C0
	static struct f1c100s_i2c_bus f1c100s_i2c0;
	rt_memset((void *)&f1c100s_i2c0, 0, sizeof(struct f1c100s_i2c_bus));
	f1c100s_i2c0.parent.ops = &f1c100s_i2c0;
	f1c100s_i2c0.addr = 0x01c27000;
	f1c100s_i2c0.name = "f1c100s_i2c0";
	gpio_set_func(GPIO_PORT_A, GPIO_PIN_3, 3);
	gpio_set_func(GPIO_PORT_B, GPIO_PIN_3, 3);
	gpio_set_pull_mode(GPIO_PORTA, GPIO_PIN_3, PULL_UP);
	rt_i2c_bus_device_register(&f1c100s_i2c0.parent, "i2c0");
#endif
#ifdef RT_USING_I2C1
	static struct f1c100s_i2c_bus f1c100s_i2c1;
	rt_memset((void *)&f1c100s_i2c1, 0, sizeof(struct f1c100s_i2c_bus));
	f1c100s_i2c0.parent.ops = &f1c100s_i2c1;
	f1c100s_i2c0.addr = 0x01c27000;
	f1c100s_i2c0.name = "f1c100s_i2c1";
	gpio_set_func(GPIO_PORT_A, GPIO_PIN_3, 3);
	gpio_set_func(GPIO_PORT_B, GPIO_PIN_3, 3);
	rt_i2c_bus_device_register(&f1c100s_i2c0.parent, "i2c1");
#endif    
	return 0;
};
#endif // RT_USING_I2C