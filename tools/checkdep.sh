#!/bin/sh

# This script checks some needed basic dependencies.

# output error messages to stderr
alias errcho='>&2 echo'

CHECKREADLINE=`locate readline.h | wc -l`

if [ $CHECKREADLINE = 0 ]; then
	errcho "Dependency-Check: Error!"
	errcho "'readline.h' is not available. Please install the readline development libraries."
	errcho "On Debian based platforms (Ubuntu) use"
	errcho " sudo apt-get install libreadline-dev"
	errcho "On SUSE platforms use"
	errcho " yum install readline-devel"
	exit 1
fi


CHECKGCC=`g++ --version 2> /dev/null | wc -l`

if [ $CHECKGCC = 0 ]; then
	errcho "Dependency-Check: Error!"
	errcho "Please install a c++ compiler (e. g. g++). You can do so by typing"
	errcho " sudo apt-get install g++"
	exit 2
fi

CHECKMFOUR=`m4 --version 2> /dev/null | wc -l`

if [ $CHECKMFOUR = 0 ]; then
	errcho "Dependency-Check: Error!"
	errcho "The program 'm4' is currently not installed. Please install it by typing"
	errcho " sudo apt-get install m4"
	exit 3
fi

exit 0

