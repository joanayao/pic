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

def subtract(x1, y1, x2, y2):
    x, y = [], []
    for pos1, value in enumerate(x1):
        if value in x2:
            pos2 = x2.index(value)
            dif = y1[pos1] - y2[pos2]
            x.append(value)
            y.append(dif)

    return [x,y]      

def Convergence(x1, y1, x2, y2, x3, y3, type):

    if type == 'analytical':
        dif1 = subtract(x3,y3,x1,y1)
        dif2 = subtract(x2,y2,x1,y1)
    elif type == 'numeric':
        dif1 = subtract(x3,y3,x2,y2)
        dif2 = subtract(x2,y2,x1,y1)
    else:
        print("Error: type not defined")
        return

    x,y = [], []
    for pos1, value in enumerate(dif1[0]):
        if value in dif2[0]:
            pos2 = dif2[0].index(value)
            x.append(value)
            y.append(math.log2(dif1[1][pos1]/dif2[1][pos2]))
    return [x,y]

files = ["integral1.txt", # index 0
         "integral05.txt", # index 1
         "integral025.txt", # index 2
         "integral0125.txt", # index 3
         "integral09odd.txt", # index 4
         "integral09even.txt", # index 5
         "integral03odd.txt", # index 6
         "integral03even.txt", # index 7
         "integral01odd.txt", # index 8
         "integral01even.txt", # index 9
         "simpson_i0_l.dat",
         "simpson_i0_m.dat",
         "simpson_i0_h.dat",
         "simpson_offset_i0_l.dat",
         "simpson_offset_i0_m.dat",
         "simpson_offset_i0_h.dat",
         "trap_l.dat",
         "trap_m.dat",
         "trap_h.dat",
]

xyData = []
for file in files:
    data =  read_file(file)
    x = data[0]
    y = data[1]
    xyData.append([x,y])

# integral01 = [[],[]]
# integral01[0] = xyData[8][0]+xyData[9][0]
# integral01[1] = xyData[8][1]+xyData[9][1]

# print(integral01[0])

# print(xyData[8][0])

# plt.plot(xyData[18][0],xyData[18][1],'o', color = 'hotpink')
# plt.xlabel('x')
# plt.ylabel('Integral')
# plt.title('Integral of a Gaussian Distribution')
# plt.savefig("Trap_integral01.png")

################## Plot differences for odd and even separately ##################

# Plot for odd number of slices

# SubOdd1 = subtract(xyData[4][0],xyData[4][1],xyData[6][0],xyData[6][1])
# SubOdd2 = subtract(xyData[6][0],xyData[6][1],xyData[8][0],xyData[8][1])

# SubOdd2[1] = [e*3*3*3*3 for e in SubOdd2[1]]


# plt.plot(SubOdd2[0],SubOdd2[1], linewidth = 0.9, label = r"Difference Mid-High $\times$ $3^4$",color = 'xkcd:soft pink')
# plt.plot(SubOdd1[0],SubOdd1[1], 'o', label = "Difference Low-Mid", color = "hotpink")

# plt.ylim(-2.1e-6,2.1e-6)
# plt.legend(loc = "upper left")
# plt.xlabel("x")
# plt.ylabel("Relative Errors")
# #plt.title("Odd Slices")
# plt.savefig("Simp_subtract_odd1_final_NoAbs.png")


# Plot for even number of slices

# SubEven1 = subtract(xyData[5][0],xyData[5][1],xyData[7][0],xyData[7][1])
# SubEven2 = subtract(xyData[7][0],xyData[7][1],xyData[9][0],xyData[9][1])

# SubEven2[1] = [e*3*3*3*3 for e in SubEven2[1]]


# plt.plot(SubEven2[0],SubEven2[1], linewidth = 0.8,label = r"Difference Mid-High $\times$ $3^4$", color = 'xkcd:soft pink')
# plt.plot(SubEven1[0],SubEven1[1], 'o',label = "Difference Low-Mid", color = "hotpink")
# plt.ylim(-2.5e-7,2.5e-7)
# plt.legend(loc = "upper left")
# plt.xlabel("x")
# plt.ylabel("Relative Errors")
# #plt.title("Even Slices")
# plt.savefig("Simp_subtract_even_final_NoAbs.png")


################## Plot differences without separate files ##################

Sub1 = subtract(xyData[16][0],xyData[16][1],xyData[17][0],xyData[17][1])
Sub2 = subtract(xyData[17][0],xyData[17][1],xyData[18][0],xyData[18][1])

Sub2[1] = [sub*4 for sub in Sub2[1]]
#Sub1[0] = [x-0.5*(Sub1[0][1]-Sub1[0][0]) for x in Sub1[0]]

plt.ticklabel_format(style='sci', axis='y', scilimits=(0,0))
plt.ylabel("Relative errors")
plt.xlabel("x")
plt.plot(Sub2[0],Sub2[1], linewidth = 1, color = 'xkcd:soft pink', label = r"High-Mid $\times$ $2^2$")
plt.plot(Sub1[0],Sub1[1], 'o', color = 'hotpink', label = "Mid-Low")
plt.legend(loc = "upper left")
plt.savefig("Trap_subtract.png")



################## Plot Divisions ###########################################

# Conv1 = Convergence(xyData[2][0],xyData[2][1],xyData[1][0],xyData[1][1],xyData[0][0],xyData[0][1],'numeric')
# Conv2 = Convergence(xyData[3][0],xyData[3][1],xyData[2][0],xyData[2][1],xyData[1][0],xyData[1][1],'numeric')

#plt.scatter(Conv1[0],Conv1[1],label = 'Conv1', color = 'pink')
#plt.scatter(Conv2[0],Conv2[1],label = 'Conv2', color = 'red')
#plt.plot(Conv1[0],Conv1[1],label = 'Conv1', color = 'pink',linestyle = 'solid')
#plt.title("Simpson Rule Convergence")
#plt.xlim(-5,5)
#plt.ylim(0,8)
#plt.savefig("Simp_convergence3.png")


##################### First Slice of RK ################################

# files = ["Output/WaveEq_test1.dat","Output/WaveEq_test2.dat","Output/WaveEq_test3.dat"]

# for i in range(len(files)):
#     Data = ReadOutput(files[i])
#     x = Data[2][6]
#     y = Data[2][2]
#     xyData.append([x,y])

# plt.plot(xyData[10][0],xyData[10][1], label = "High")
# plt.plot(xyData[11][0],xyData[11][1], 'x', label = "Mid")
# plt.plot(xyData[12][0],xyData[12][1], 'x', label = "Low")
# plt.xlim(20.15,20.17)
# plt.ylim(0.1,0.102)
# plt.legend()
# plt.savefig("RK_firstslice.png")

##################### First Slice of RK  with Simpson_test.cpp ################################

files = ["RK_FirstSlice01_hbar.dat","RK_FirstSlice02_hbar.dat","RK_FirstSlice04_hbar.dat"]

xyData = []
for file in files:
    data =  read_file(file)
    x = data[0]
    y = data[1]
    xyData.append([x,y])

Sub1 = subtract(xyData[0][0],xyData[0][1],xyData[1][0],xyData[1][1])
Sub2 = subtract(xyData[1][0],xyData[1][1],xyData[2][0],xyData[2][1])

Sub1[1] = [sub*16 for sub in Sub1[1]]

plt.plot(Sub1[0],Sub1[1],'o')
plt.plot(Sub2[0],Sub2[1], linewidth = 0.8)
plt.savefig("RK_firstslice_hbar_errors.png")