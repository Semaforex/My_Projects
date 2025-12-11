import numpy

def inverse_kinematics(x, y, z, l1, l2, l3, l4, lb):
    dx = numpy.sqrt(x**2 + y**2)
    alpha = numpy.arctan((l4+z-lb)/dx)
    d = numpy.sqrt(dx**2 + (l4+z-lb-l1)**2)
    gamma = numpy.arccos((l2**2 + d**2 - l3**2)/(2*l2*d))
    theta2 = numpy.pi/2 - alpha - gamma
    theta3 = numpy.pi/2-numpy.arccos((l2**2 + l3**2 - d**2)/(2*l2*l3))
    theta4=numpy.pi-theta2-theta3
    theta1 = numpy.arctan2(y, x)


    # Remove duplicates (within tol)
    return [theta1, theta2, theta3, theta4, 0.0]



if __name__ == "__main__":
    base_height = 0.05
    link1_length = 0.088
    link2_length = 0.135
    link3_length = 0.147
    link4_length = 0.05
    link5_length = 0.02
    base_limit_lower = -1.57
    base_limit_upper = 1.57
    link1_limit_lower = 0.0
    link1_limit_upper = 1.48
    link2_limit_lower = -0.17
    link2_limit_upper = 1.57
    link3_limit_lower = -1.57
    link3_limit_upper = 1.57
    link4_limit_lower = -1.57
    link4_limit_upper = 1.57
    x = 0.16289270158448518
    y = -0.1881485719406504
    z = 0.1176315477708855
    d = numpy.sqrt(x**2 + y**2)
    print(d)
    print(inverse_kinematics(x, y, z, link1_length, link2_length, link3_length, link4_length, base_height))