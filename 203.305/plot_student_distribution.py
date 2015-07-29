import numpy as np
from scipy.stats import t as student_t
import plotly.plotly as py
from plotly.graph_objs import *

mu = 0
x = np.linspace(-10, 10, 1000)
dist = student_t(1e10, 0)
y = dist.pdf(x)
data = Data([
    Scatter(
        x = x,
        y = y
    )
])
layout = Layout(title="Student's t-distribution")
fig = Figure(data=data,layout=layout)
py.plot(fig,file="Student Distribution")
