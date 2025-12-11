import sys
if sys.prefix == '/usr':
    sys.real_prefix = sys.prefix
    sys.prefix = sys.exec_prefix = '/home/student/Desktop/paradowski-sosnowski-arno-25l-grupa103/LAB3/install/lab3_pd'
