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

data = read_file("integral.txt") # change filename here

x = data[0]
y = data[1]

plt.scatter(x,y,color='pink')
plt.title("Integral with Simpon Rule of gaussian with sigma=1 & h=0.01")
plt.xlim(-0.2,0.2)
plt.savefig("gaussian_integral001.png")
