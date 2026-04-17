import numpy as np
from scipy.integrate import quad

isqrtx = lambda x: 1/np.sqrt(x)
f4x = lambda x: np.log(x) / np.sqrt(x)
if1 = lambda x: x**3 * np.exp(-x**2)
if2 = lambda x: np.sin(x) * if1(x)

q = quad(f4x, 0,1, epsabs = 1e-3, epsrel = 1e-3, full_output = 1)
q = quad(isqrtx, 0,1, epsabs = 1e-3, epsrel = 1e-3, full_output = 1)
q = quad(if1, 0, np.inf, epsabs = 1e-3, epsrel = 1e-3, full_output = 1)
q = quad(if1, -np.inf, 0, epsabs = 1e-3, epsrel = 1e-3, full_output = 1)
q = quad(if2, 0, np.inf, epsabs = 1e-3, epsrel = 1e-3, full_output = 1)
q = quad(if2, -np.inf, np.inf, epsabs = 1e-3, epsrel = 1e-3, full_output = 1)
print("integral is ", q[0])
print("#evaluations is ", q[2]['neval'])
