import subprocess

ParametersFile = "Parameters/Params_Pulse_Origin.dat"

subprocess.run(["g++", "main.cpp", "Simpson.cpp", "Solver.cpp", "Parameters.cpp", "-o", "main.exe"])
subprocess.run(["./main.exe", ParametersFile])

