import numpy as np
v1 = np.random.randint(0,999999,1000000)
v2 = np.random.randint(0,999999,1000000)
w = np.random.randint(0,100,1000000)
for i in range(1000000):
	print(str(v1[i])+" "+str(v2[i])+" "+str(w[i]))
for i in range(999999):
	print(str(i)+" "+str(i+1)+" 500")