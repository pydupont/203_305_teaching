import matplotlib.pyplot as plt
import numpy as np

#Fake data generation
e1 = np.random.normal(0, 1, size=(500,))
e2 = np.random.normal(0, 1, size=(500,))
e3 = np.random.normal(0, 2, size=(500,))
e4 = np.random.normal(0, 2, size=(500,))

data=[e1,e2,e3,e4]
plt.boxplot(data)
plt.show()
