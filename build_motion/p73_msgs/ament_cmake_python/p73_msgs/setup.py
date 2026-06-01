from setuptools import find_packages
from setuptools import setup

setup(
    name='p73_msgs',
    version='0.0.0',
    packages=find_packages(
        include=('p73_msgs', 'p73_msgs.*')),
)
