import math as math
import matplotlib.pyplot as plt

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


def subtract(x1, y1, x2, y2):
    x, y = [], []
    for pos1, value in enumerate(x1):
        if value in x2:
            pos2 = x2.index(value)
            dif = y1[pos1] - y2[pos2]
            x.append(value)
            y.append(dif)

    return [x,y]  

def RKsubtract(file1,file2, u_step1, u_step2,v_step1,v_step2):
    output1 = ReadOutput(file1)
    output2 = ReadOutput(file2)

    conv_data = []
    x, y = [],[]

    for pos1, value1 in enumerate(output1):
        if type(value1) != list and value1 in output2: # if u value is in both files
            pos2 = output2.index(value1)
            x.append(int((pos2-1)/2)*u_step2)
            conv_data.append(value1)
            conv_data.append(subtract(output1[pos1+1][6],output1[pos1+1][2],output2[pos2+1][6],output2[pos2+1][2]))

    # with open(munninfile,"w") as file:
    #     for i in range(1, len(conv_data)-1, 2):
    #         file.write(conv_data[i])
    #         datalines = [str(conv_data[i+1][0][j]) + " " + str(conv_data[i+1][1][j]) + "\n" for j in range(len(conv_data[i+1][0]))]
    #         file.writelines(datalines)
    #         file.write("\n")

    y = [math.sqrt(sum(x*x for x in conv_data[i][1])*v_step2) for i in range(1, len(conv_data), 2)]

    return [[x,y],conv_data]




# SubHigh = RKsubtract("Output/WaveEq_NoFit_test1.dat","Output/WaveEq_NoFit_test2.dat",0.05,0.1,0.01,0.02)[0]
# SubLow = RKsubtract("Output/WaveEq_NoFit_test2.dat","Output/WaveEq_NoFit_test3.dat",0.1,0.2,0.02,0.04)[0]

# SubHigh[1] = [e*16 for e in SubHigh[1]]

# plt.plot(SubHigh[0],SubHigh[1],'x', label = r"Mid-High $\times 2^4$")
# plt.plot(SubLow[0],SubLow[1],'o', label = "Low-Mid")
# plt.legend()
# plt.xlabel("u")
# plt.ylabel("low - high for hbar")
# plt.savefig("RK_convergence_pointwise_nofit_1.png")

# SubHigh[0].pop(0)
# SubHigh[1].pop(0)
# SubLow[0].pop(0)
# SubLow[1].pop(0)

# x_h,y_h = [],[]
# for i, value in enumerate(SubLow[0]):
#     if value in SubHigh[0]:
#         j = SubHigh[0].index(value)
#         x_h.append(value)
#         y_h.append(math.log2(SubLow[1][i]/SubHigh[1][j]))


# plt.plot(x_h,y_h,'o')
# plt.ylim(0,5)
# plt.xlabel("u")
# plt.ylabel("log2(low/high) for hbar")
# plt.title("RK4 Convergence")
# plt.savefig("RK_convergence_nofit_1.png")

conv_data_High = RKsubtract("Output/WaveEq_NoFit_test1.dat","Output/WaveEq_NoFit_test2.dat",0.05,0.1,0.01,0.02)[1]
conv_data_Low = RKsubtract("Output/WaveEq_NoFit_test2.dat","Output/WaveEq_NoFit_test3.dat",0.1,0.2,0.02,0.04)[1]

conv_data_High[7][1] = [e*16 for e in conv_data_High[7][1]]

plt.plot(conv_data_Low[7][0][::2],conv_data_Low[7][1][::2], 'o', label = "Mid-Low ", color = "xkcd:light indigo")
plt.plot(conv_data_High[7][0][::2],conv_data_High[7][1][::2], linewidth = 0.9,label="High-Mid", color = "xkcd:warm pink")

plt.xlabel("v")
#plt.xlim(14,27)
#plt.ylim(-1e-4,3e-3)
plt.legend()
plt.ylabel("Low-High hbar")
plt.title(conv_data_High[6])

plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
plt.savefig("RK_hbar_errors_NoFit_u0.png")

plt.plot()