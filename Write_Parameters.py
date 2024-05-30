filename = "Params_h_corrected.dat"
description = "Corrected h_init_function test"
u_step = 0.1
u_final = 10
r_step = 0.01
r_final = 15
InitData = "h_corrected.dat"
OutputFile = "h_corrected.dat"

with open("Parameters/" + filename, "w") as file:
    file.write("# Description: " + description + "\n")
    file.write("#\n")
    file.write("u_step: " + str(u_step) + "\n")
    file.write("u_final: " + str(u_final) + "\n")
    file.write("r_step: " + str(r_step) + "\n")
    file.write("r_final: " + str(r_final) + "\n")
    file.write("InitData: InitialData/" + InitData + "\n")
    file.write("OutputFile: Output/" + OutputFile + "\n")