#!/usr/bin/python3

k = 2
n = 17
m = 1
p = 0

freq = 24 * n * k / (m * (1 << p))
regval = 1 << 31 | p << 16 | (n-1) << 8 | (k-1) << 4 | (m-1)
print("CPU freq=%dMHz, PLL_CPU_CTRL_REG=0x%08x" % (freq, regval))

