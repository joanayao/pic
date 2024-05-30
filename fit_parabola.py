import math as math
import matplotlib.pyplot as plt
import numpy as np

def read_file(filename):

    columns = []
    with open(filename, 'r') as file:
        lines = file.readlines()
        lines.pop(0)
        num_columns = len(lines[0].split())
        for i in range(num_columns):
            columns.append([])
        
        for line in lines:
            values = line.split()
            for i, value in enumerate(values):
                columns[i].append(float(value))
                
    return columns

files = [
    "Parabola01.dat",
    "Parabola_fit01.dat"
]

datas, fits_params = [],[]

for i in range(0,len(files),2):
    datas.append(read_file(files[i]))
    fits_params.append(read_file(files[i+1]))

x = np.array(datas[0][0])
parabola = fits_params[0][0][0]*x*x + fits_params[0][1][0]*x + fits_params[0][2][0]*x

plt.plot(x[30::],datas[0][1][:len(datas[0][1])-30], "o", label = "data points")
plt.plot(x,parabola, label = "parabola fit")
plt.legend()
plt.savefig("parabola_test.png")