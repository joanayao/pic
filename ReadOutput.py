import matplotlib.pyplot as plt
import numpy as np

def ReadOutput(filename):
    data = []
    with open(filename, 'r') as file:
        lines = file.readlines()
        num_columns = len(lines[0].split())
        lines.pop(0)
        columns = [[] for i in range(num_columns)]
        
        i = 0
        while i < len(lines):

            if lines[i] == '\n' and i < len(lines) - 1:
                data.append(columns)
                columns = [[] for i in range(num_columns)]
                data.append(lines[i+1])
                i += 2
                continue
            else:
                values = lines[i].split()
                for j, value in enumerate(values):
                    columns[j].append(float(value))

            i += 1
                
    return data

def WriteMunninFile(x,y,data,outfilename):
    # r -> 0
    # h -> 1
    # h_bar -> 2
    # q -> 3
    # g -> 4
    # g_bar -> 5

    if x == 'r':
        ix = 0
    elif x == 'h':
        ix = 1
    elif x == 'h_bar':
        ix = 2
    elif x == 'q':
        ix = 3
    elif x == 'g':
        ix = 4
    elif x == 'g_bar':
        ix = 5
    else:
        print("Error: invalid x variable")
        return
    
    if y == 'r':
        iy = 0
    elif y == 'h':
        iy = 1
    elif y == 'h_bar':
        iy = 2
    elif y == 'q':
        iy = 3
    elif y == 'g':
        iy = 4
    elif y == 'g_bar':
        iy = 5
    else:
        print("Error: invalid y variable")
        return

    file = open(outfilename, "w")

    for i in range(1, len(data)-1, 2):
        file.write(data[i])
        datalines = [str(data[i+1][ix][j]) + " " + str(data[i+1][iy][j]) + "\n" for j in range(len(data[i+1][ix]))]
        file.writelines(datalines)
        file.write("\n")


data = ReadOutput("test.dat")
WriteMunninFile('r','h_bar',data,"MuninnTest.dat")

