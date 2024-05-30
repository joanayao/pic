import math as math

# Set hbar_init as a gaussian

p = 1e-4
r0 = 0
sig = 1

r_step = 0.01
r_size = 1500

filename = "Pulse_origin" + ".dat"

# h_init_function = lambda r: (r*(p*math.exp(-(r-r0)*(r-r0)/(sig*sig))*(2*r-2*r*r*(r-r0)/(sig*sig)))
# +p*r*r*math.exp(-(r-r0)*(r-r0)/(sig*sig)))

h_init_function = lambda r: p*r*r*math.exp(-(r-r0)*(r-r0)/(sig*sig))*(-2*r*r+2*r*r0+3*sig*sig)/(sig*sig)

with open("InitialData/" + filename, "w") as file:
    file.writelines([str(i*r_step) + "\n" for i in range(0,r_size)])
    file.writelines([str(h_init_function(i*r_step)) + "\n" for i in range(0,r_size)])