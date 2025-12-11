from setuptools import find_packages, setup

package_name = 'lab2_pd_pkg'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools', 'rclpy', 'turtlesim'],
    zip_safe=True,
    maintainer='mparadow',
    maintainer_email='michal.paradowski.stud@pw.edu.pl',
    description='custom turtle controlling',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'turtle_control_node = lab2_pd_pkg.turtle_control_node:main'
        ],
    },
)
