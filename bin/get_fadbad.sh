#! /bin/bash -e
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell
#
# CppAD is distributed under the terms of the
#              Eclipse Public License Version 2.0.
#
# This Source Code may also be made available under the following
# Secondary License when the conditions for such availability set forth
# in the Eclipse Public License, Version 2.0 are satisfied:
#            GNU General Public License, Version 3.0.
# -----------------------------------------------------------------------------
# $begin get_fadbad.sh$$ $newlinech #$$
# $spell
#	Fadbad
#	CppAD
# $$
#
# $section Download and Install Fadbad in Build Directory$$
# $index fadbad, download and install$$
# $index download, install fadbad$$
# $index install, fadbad$$
#
# $head Syntax$$
# $code bin/get_fadbad.sh$$
#
# $head Purpose$$
# If you are using Unix, this command will download and install
# $href%http://www.fadbad.com%Fadbad%$$ in the
# CppAD $code build$$ directory.
#
# $head Distribution Directory$$
# This command must be executed in the
# $cref/distribution directory/download/Distribution Directory/$$.
#
# $head External Directory$$
# The Fadbad source code is downloaded into the sub-directory
# $code build/external$$ below the distribution directory.
#
# $head Prefix Directory$$
# The Fadbad include files are installed in the sub-directory
# $code build/prefix/include/FADBAD++$$ below the distribution directory.
#
# $end
# -----------------------------------------------------------------------------
if [ $0 != "bin/get_fadbad.sh" ]
then
	echo "bin/get_fadbad.sh: must be executed from its parent directory"
	exit 1
fi
# -----------------------------------------------------------------------------
# bash function that echos and executes a command
echo_eval() {
	echo $*
	eval $*
}
# -----------------------------------------------------------------------------
echo 'Download fadbad to build/external and install it to build/prefix'
version='2.1'
web_page='http://www.fadbad.com/download'
cppad_dir=`pwd`
prefix="$cppad_dir/build/prefix"
installed_flag="build/external/fadbad-${version}.installed"
if [ -e "$installed_flag" ]
then
	echo "$installed_flag exists: Skipping get_fadbad.sh"
	exit 0
fi
# -----------------------------------------------------------------------------
if [ ! -d build/external ]
then
	echo_eval mkdir -p build/external
fi
echo_eval cd build/external
# -----------------------------------------------------------------------------
if [ ! -e "FADBAD++-$version.tar.gz" ]
then
	echo_eval wget --no-check-certificate $web_page/FADBAD++-$version.tar.gz
fi
if [ -e "FADBAD++" ]
then
	echo_eval rm -r FADBAD++
fi
echo_eval tar -xzf FADBAD++-$version.tar.gz
if [ ! -e "$prefix/include" ]
then
	echo_eval mkdir -p "$prefix/include"
fi
if [ -e "$prefix/include/FADBAD++" ]
then
	echo_eval rm -r "$prefix/include/FADBAD++"
fi
echo_eval cp -r FADBAD++ "$prefix/include/FADBAD++"
# -----------------------------------------------------------------------------
echo_eval touch $cppad_dir/$installed_flag
echo "get_fadbad.sh: OK"