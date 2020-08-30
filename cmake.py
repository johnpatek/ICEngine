#! /usr/bin/python3
import os
import argparse
import sys

# System specific settings

MAKE_COMMAND = 'make'
INSTALL_TARGET = 'install'
ALL_TARGET = 'all'
if(os.name == 'nt')
    MAKE_COMMAND = 'msbuild'
    INSTALL_TARGET = 'INSTALL.vcxproj'
    ALL_TARGET = 'ALL.vcxproj'


# Argument parsing

argument_parser = argparse.ArgumentParser()
argument_parser.add_argument('-c','--common',action='store_true')
argument_parser.add_argument('-e','--examples',action='store_true')
argument_parser.add_argument('-t','--tests',action='store_true')
argument_parser.add_argument('-r','--rebuild',action='store_true')

# build targets

def build_common():
    print('building common')

def build_engine(
    examples : bool, 
    tests : bool):
    print('building engine')

def build_main():
    args = argument_parser.parse_args()
    if args.common:
        build_common()
    build_engine()

if __name__ == '__main__':
    try:
        build_main()
    except:
        print(sys.exc_info())