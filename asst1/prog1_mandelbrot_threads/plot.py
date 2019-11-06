#!/usr/bin/env python

import re
import matplotlib.pyplot as plt
decimal = re.compile(r'(\d*\.{0,1}\d+)')

speed = []
threads = []

while True:
	try:
		s = input()
		val = list(map(float,re.findall(decimal,s)))
		print("get number: {}".format(val))
		assert len(val)==2,"regex error"
		speed.append(val[0])
		threads.append(val[1])
	except EOFError as io:
		break

plt.plot(threads,speed)
plt.show()