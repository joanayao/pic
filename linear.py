import math as math
import matplotlib.pyplot as plt
import numpy as np

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

datas = [read_file("LinearReg_NoZero_Exp01.dat"),read_file("LinearReg_NoZero_Exp02.dat"),read_file("LinearReg_NoZero_Exp04.dat")]
fits_params = [read_file("LinearReg_NoZero_Exp_fit01.dat"),read_file("LinearReg_NoZero_Exp_fit02.dat"),read_file("LinearReg_NoZero_Exp_fit04.dat")]

x01 = np.array(datas[0][0])
x02 =np.array(datas[1][0])
x04 =np.array(datas[2][0])
fitted_line01 = fits_params[0][0][0] + fits_params[0][1][0]*x01
fitted_line02 = fits_params[1][0][0] + fits_params[1][1][0]*x02
fitted_line04 = fits_params[2][0][0] + fits_params[2][1][0]*x04

# plt.plot(x,fitted_line01, label = "h = 0.01")
# plt.plot(x,fitted_line02, label = "h = 0.02")
# plt.plot(x,fitted_line04, label = "h = 0.04")

# plt.scatter(datas[0][0],datas[0][1], label = "exp(x) without point in x=0", color = "xkcd:warm pink")
# #plt.plot(x,y, label = "Linear regression of 4 points after origin",color = "xkcd:light indigo")
# plt.title("Linear Regression")
# plt.legend()
# plt.ylabel("y")
# plt.xlabel("x")
# plt.savefig("linear_exp_NoZero.png")


SubHigh = subtract(x01.tolist(),fitted_line01.tolist(),x02.tolist(),fitted_line02.tolist())
SubLow = subtract(x02.tolist(),fitted_line02.tolist(),x04.tolist(),fitted_line04.tolist())

# Conv = Convergence(x01.tolist(),fitted_line01.tolist(),x02.tolist(),fitted_line02.tolist(),x04.tolist(),fitted_line04.tolist(),"numeric")

# plt.plot(Conv[0],Conv[1])
# plt.ylabel("log2(low/high)")
# plt.title("Linear regression convergence (witouth point in x=0)")
# plt.xlabel("x")
# #plt.ylim(0,2)
# plt.savefig("linear_exp_conv_NoZero_2.png")


SubHigh[1] = [e*2 for e in SubHigh[1]]
plt.plot(SubHigh[0],SubHigh[1])
plt.plot(SubLow[0],SubLow[1],"o")
plt.ylabel("low - high * 2")
plt.xlabel("x")
plt.title("Linear regression Errors (without point in x=0)")
plt.savefig("linear_exp_errors_NoZero.png")


# b_convergence = math.log2(abs(fits_params[2][0][0]-fits_params[1][0][0])/abs(fits_params[1][0][0]-fits_params[0][0][0]))
# print(b_convergence)

# fit_convergence = math.log2(abs(fits_params[2][1][0]-fits_params[1][1][0])/abs(fits_params[1][1][0]-fits_params[0][1][0]))
# print(fit_convergence)
