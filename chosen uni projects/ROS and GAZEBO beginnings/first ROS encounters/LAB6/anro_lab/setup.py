import os
from glob import glob
from setuptools import setup
from setuptools import find_packages

package_name = 'anro_lab6'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name), glob('launch/*.py')),
        (os.path.join('share', package_name), glob('urdf/*'))
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='michal',
    maintainer_email='michal.parad@pwuw.pl',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            "cube_move = anro_lab6.cube_move:main",
            "reverse_kin = anro_lab6.reverse_kin:main",
            "forward_kin = anro_lab6.forward_kin:main",
            "rvizToDobot = anro_lab6.rvizToDobot:main",
        ],
    },
)
