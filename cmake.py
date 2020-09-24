#! /usr/bin/python3
import os
import argparse
import sys
import subprocess
import shutil

# System specific settings

if(os.name == 'nt'):
    MAKE_ALL = ['msbuild','-verbosity:m','/property:Configuration=Release','ALL_BUILD.vcxproj']
    MAKE_INSTALL = ['msbuild','-verbosity:m','/property:Configuration=Release','INSTALL.vcxproj']
else:
    MAKE_ALL = ['make','all']
    MAKE_INSTALL = ['make','install']    

# Argument parsing
argument_parser = argparse.ArgumentParser()
argument_parser.add_argument('-c','--common',action='store_true')
argument_parser.add_argument('-r','--rebuild',action='store_true')
argument_parser.add_argument('-e','--examples',action='store_true')
argument_parser.add_argument('-t','--tests',action='store_true')

def clean_path(path : str):
    if(os.path.exists(path)):
        shutil.rmtree(path)

COMMON_BUILD = os.path.join('common','build')

# build targets
def build_common(rebuild : bool):
    subprocess.run(['git','submodule','update','--init','--recursive'])
    if(rebuild):
        clean_path(COMMON_BUILD) 
    if(not(os.path.exists(COMMON_BUILD))):
        os.mkdir(COMMON_BUILD)
    os.chdir(COMMON_BUILD)
    subprocess.run(['cmake','..'])
    subprocess.run(MAKE_INSTALL)
    os.chdir('..')
    os.chdir('..')

def build_library(rebuild : bool, examples : bool, tests : bool):
    if(rebuild):
        clean_path('build')
    if(not(os.path.exists('build'))):
        os.mkdir('build')
    os.chdir('build')
    EXAMPLES = "-DBUILD_EXAMPLES=OFF"
    TESTS = "-DBUILD_TESTS=OFF"
    if(examples):
        EXAMPLES = "-DBUILD_EXAMPLES=ON"
    if(tests):
        EXAMPLES = "-DBUILD_TESTS=ON"    
    subprocess.run(['cmake','..',EXAMPLES,TESTS])
    subprocess.run(MAKE_ALL)
    os.chdir('..')


def build_main():
    args = argument_parser.parse_args()
    if args.common:
        build_common(args.rebuild)
    else:
        build_library(
            args.rebuild,
            args.examples,
            args.tests)

    

if __name__ == '__main__':
    try:
        build_main()
    except:
        print(sys.exc_info())

