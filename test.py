import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(0, 1, 100)
y = x / 2 / (1 + np.cos(x))

plt.plot(x, y)
plt.xlabel('x')
plt.ylabel('y')
plt.title('Graph of x / 2 / (1 + cos(x))')
plt.grid(True)
plt.show()
