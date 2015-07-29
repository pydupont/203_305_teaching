import numpy as np
import matplotlib.pyplot as plt

# evenly sampled time at 200ms intervals
t = np.arange(0., 5., 0.2) #Simple data data. Same as range() function

# red dashes, blue squares and green triangles
plt.plot(t, t, 'r--')    #Red (r) dashes (--)
plt.plot(t, t**2, 'bs')  #Blue (b) squares (s)
plt.plot(t, t**3, 'g^')  #Green (g) triangles (^)
plt.plot(t, t**4, 'b+-') #Blue (b) crosses (+) linked by a line (-)
plt.show()
