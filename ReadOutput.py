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

        data.append(columns)
                
    return data

def WriteMunninFile(x,y,data,outfilename):

    if x == 'r':
        ix = 0
    elif x == 'h':
        ix = 1
    elif x == 'hbar':
        ix = 2
    elif x == 'q':
        ix = 3
    elif x == 'g':
        ix = 4
    elif x == 'gbar':
        ix = 5
    elif x == 'v':
        ix = 6
    elif y == 'h_fit':
        ix = 7
    else:
        print("Error: invalid x variable")
        return
    
    if y == 'r':
        iy = 0
    elif y == 'h':
        iy = 1
    elif y == 'hbar':
        iy = 2
    elif y == 'q':
        iy = 3
    elif y == 'g':
        iy = 4
    elif y == 'gbar':
        iy = 5
    elif y == 'v':
        iy = 6
    elif y == 'hfit':
        iy = 7
    else:
        print("Error: invalid y variable")
        return

    file = open(outfilename, "w")

    for i in range(1, len(data)-1, 2):
        file.write(data[i])
        datalines = [str(data[i+1][ix][j]) + " " + str(data[i+1][iy][j]) + "\n" for j in range(len(data[i+1][ix]))]
        file.writelines(datalines)
        file.write("\n")


data = ReadOutput("Output/GRPulse_origin_h0_Parab_stag_small.dat")
WriteMunninFile('v','q',data,"MuninnPlot/GRPulse_origin_h0_Parab_stag_small_q_v.dat")
