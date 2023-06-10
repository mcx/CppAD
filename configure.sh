# #! /bin/bash -e
# # SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# # SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# # SPDX-FileContributor: 2003-23 Bradley M. Bell
# # ----------------------------------------------------------------------------
# {xrst_begin configure app}
# {xrst_spell
#    adouble
#    ansi
#    badiff
#    boostvector
#    cl
#    cxx
#    cygdrive
#    dist
#    eigenvector
#    italic
#    linux
#    msvc
#    optionally
#    postfix
#    stdvector
#    dir
#    ublas
#    usr
#    Fortran
#    Ralphs
#    nmake
# }
# {xrst_comment_ch #}
#
# Configure Test and Installation Script
# ######################################
#
# Deprecated 2012-12-26
# *********************
# This install procedure is deprecated.
# You should use the :ref:`cmake-name` instructions to
# configure and install CppAD.
#
# Distribution Directory
# **********************
# You must first obtain a copy of the CppAD distribution directory
# using the :ref:`download-name` instructions.
# We refer to the corresponding
# :ref:`download@Distribution Directory` as
# *dist_dir* .
#
# Build Directory
# ***************
# Create the directory *dist_dir* / ``build`` ,
# which will be referred to as the build directory below.
#
# Configure
# *********
# Execute the following command in the build directory:
#
# | ../ ``configure`` \\
# | |tab| ``--help``                                       \\
# | |tab| ``--prefix`` = *prefix_dir*                      \\
# | |tab| ``--with-`` *test* ``vector``                    \\
# | |tab| ``--enable-msvc``                                \\
# | |tab| ``--with-clang``                                 \\
# | |tab| ``MAX_NUM_THREADS`` = *max_num_threads*          \\
# | |tab| ``CXX_FLAGS`` = *cxx_flags*                      \\
# | |tab| ``POSTFIX_DIR`` = *postfix_dir*                  \\
# | |tab| ``ADOLC_DIR`` = *adolc_dir*                      \\
# | |tab| ``BOOST_DIR`` = *boost_dir*                      \\
# | |tab| ``EIGEN_DIR`` = *eigen_dir*                      \\
# | |tab| ``FADBAD_DIR`` = *fadbad_dir*                    \\
# | |tab| ``SADADO_DIR`` = *sacado_dir*                    \\
# | |tab| ``IPOPT_DIR`` = *ipopt_dir*                      \\
# | |tab| ``TAPE_ADDR_TYPE`` = *tape_addr_type*            \\
# | |tab| ``TAPE_ID_TYPE`` = *tape_id_type*
#
# where only the ``configure`` line need appear; i.e.,
# the entries in all of the other lines are optional.
# The text in italic above is replaced values that you choose
# (see discussion below).
#
# make
# ****
# The following command, in the build directory,
# creates the file ``include/cppad/configure.hpp`` in
# the source directory and then builds
# some object libraries that are used by the tests
# ::
#
#    ``make``
#
# Examples and Tests
# ==================
# The following command will build and run all the correctness and speed tests.
# ::
#
#    make test
#
# prefix_dir
# **********
# The default value for prefix directory is ``$HOME``
# i.e., by default the CppAD include files
# will :ref:`install<configure@make install>` below ``$HOME`` .
# If you want to install elsewhere, you will have to use this option.
# As an example of using the ``--prefix`` = *prefix_dir* option,
# if you specify
# ::
#
#    ./configure --prefix=/usr/local
#
# the CppAD include files will be installed in the directory
#
#    / ``usr/local/include/cppad``
#
# --with-testvector
# *****************
# The :ref:`CPPAD_TESTVECTOR<testvector-name>`
# template class is used for many of the CppAD examples and tests.
# The default for this template class is ``CppAD::vector<`` *Scalar* > .
# If one, and only one, of the following command line arguments is specified:
# ::
#
#    --with-stdvector
#    --with-boostvector
#    --with-eigenvector
#
# the corresponding of the following template classes is used
#
# | |tab| ``std::vector<`` *Scalar* >
# | |tab| ``boost::numeric::ublas::vector<`` *Scalar* >
# | |tab| ``Eigen::matrix<`` *Scalar* , ``Eigen::Dynamic`` , 1>
#
# See also,
# :ref:`configure@boost_dir`
# and
# :ref:`configure@eigen_dir` .
#
# --enable-msvc
# *************
# If this flag is present, the Microsoft ``cl`` compiler will be
# placed at the front of a list of compilers to search for.
# In addition, nmake_ make files will be generated.
# The resulting C++, C, and Fortran compiler orders work Coin-OR specific
# and chosen by Ted Ralphs.
#
# .. _nmake: https://learn.microsoft.com/en-us/cpp/build/reference/nmake-reference
#
# --with-clang
# ************
# If this flag is present, ``clang++`` ( ``clang`` )
# will be used for the C++ (C) compiler.
# This option cannot appear when the --enable-msvc option appears.
#
# max_num_threads
# ***************
# this specifies the value for the default value for the preprocessor symbol
# :ref:`multi_thread@CPPAD_MAX_NUM_THREADS` .
# It must be greater than or equal to four; i.e.,
# *max_num_threads*  >= 4 .
# The default value for this option is 64.
#
# cxx_flags
# *********
# If the command line argument *CompilerFlags* is present,
# it specifies compiler flags.
# For example,
#
#    ``CXX_FLAGS`` = ``"-Wall -ansi"``
#
# would specify that warning flags ``-Wall``
# and ``-ansi`` should be included
# in all the C++ compile commands.
# The error and warning flags chosen must be valid options
# for the C++ compiler.
# The default value for *CompilerFlags* is the
# empty string.
#
# postfix_dir
# ***********
# The default value for this directory is empty; i.e., there
# is no postfix directory.
# As an example of using the ``POSTFIX_DIR`` = *postfix_dir* option,
# if you specify
# ::
#
#    ./configure --prefix=/usr/local POSTFIX_DIR=coin
#
# the CppAD include files will be
# :ref:`installed<configure@make install>` in the directory
#
#    / ``usr/local/include/coin-or/cppad``
#
# adolc_dir
# *********
# The default value for this directory is empty; i.e., there
# is no adolc directory.
# If you have
# `ADOL-C <https://projects.coin-or.org/ADOL-C>`_
# installed on your system, you can
# specify a value for *adolc_dir* in the
# :ref:`configure@Configure` command line.
# The value of *adolc_dir* must be such that
#
#    *adolc_dir* / ``include/adolc/adouble.h``
#
# is a valid way to reference ``adouble.h`` .
# In this case, you can run the Adolc speed correctness tests
# by executing the following commands starting in the build directory
#
#    make check_speed_adolc
#
# Note that these speed tests assume Adolc has been configure with
# its sparse matrix computations enabled using
#
#    ``--with-colpack`` = *colpack_dir*
#
# where *colpack_dir* is the same as *adolc_dir* .
#
# Linux
# =====
# If you are using Linux,
# you may have to add *adolc_dir* / ``lib`` to ``LD_LIBRARY_PATH`` .
# For example, if you use the ``bash`` shell to run your programs,
# you could include
#
# | |tab| ``LD_LIBRARY_PATH`` = *adolc_dir* / ``lib:$`` { ``LD_LIBRARY_PATH`` }
# | |tab| ``export LD_LIBRARY_PATH``
#
# in your ``$HOME/.bash_profile`` file.
#
# Cygwin
# ======
# If you are using Cygwin,
# you will have to add to following lines to the file
# ``.bash_profile`` in your home directory:
#
# | |tab| ``PATH`` = *adolc_dir* / ``bin:$`` { ``PATH`` }
# | |tab| ``export PATH``
#
# in order for Adolc to run properly.
# If *adolc_dir* begins with a disk specification,
# you must use the Cygwin format for the disk specification.
# For example,
# if ``d:/adolc_base`` is the proper directory,
# ``/cygdrive/d/adolc_base`` should be used for *adolc_dir* .
#
# boost_dir
# *********
# The default value for this directory is empty; i.e., there
# is no boost directory.
# If the command line argument
#
#    ``BOOST_DIR`` = *boost_dir*
#
# is present, it must be such that files
#
# | |tab| *boost_dir* / ``include/boost/numeric/ublas/vector.hpp``
# | |tab| *boost_dir* / ``include/boost/thread.hpp``
#
# are present.
# In this case, these files will be used by CppAD.
# See also, :ref:`--with-boostvector<configure@--with-testvector>`
#
# eigen_dir
# *********
# The default value for this directory is empty; i.e., there
# is no eigen directory.
# If you have
# `Eigen <http://eigen.tuxfamily.org>`_
# installed on your system, you can
# specify a value for *eigen_dir* .
# It must be such that
#
#    *eigen_dir* / ``include/Eigen/Core``
#
# is a valid include file.
# In this case CppAD will compile and test the Eigen examples; e.g.,
# :ref:`eigen_array.cpp-name` .
# See also, :ref:`--with-eigenvector<configure@--with-testvector>`
#
# fadbad_dir
# **********
# The default value for this directory is empty; i.e., there
# is no fadbad directory.
# If you have
# `Fadbad 2.1 <http://www.fadbad.com/>`_
# installed on your system, you can
# specify a value for *fadbad_dir* .
# It must be such that
#
#    *fadbad_dir* / ``include/FADBAD`` ++/ ``badiff.h``
#
# is a valid reference to ``badiff.h`` .
# In this case, you can run the Fadbad speed correctness tests
# by executing the following commands starting in the build directory:
#
#    make check_speed_fadbad
#
#
# ipopt_dir
# *********
# The default value for this directory is empty; i.e., there
# is no ipopt directory.
# If you have
# `Ipopt <http://www.coin-or.org/projects/Ipopt.xml>`_
# installed on your system, you can
# specify a value for *ipopt_dir* .
# It must be such that
#
#    *ipopt_dir* / ``include/coin-or/IpIpoptApplication.hpp``
#
# is a valid reference to ``IpIpoptApplication.hpp`` .
# In this case, the CppAD interface to Ipopt
# examples can be built and tested
# by executing the following commands starting in the build directory:
#
#    make check_cppad_ipopt
#
# Once this has been done, you can execute the program
# ``cppad_ipopt_speed`` in the ``build/cppad_ipopt/speed`` directory;
# see ``ipopt_ode_speed.cpp`` .
#
# sacado_dir
# **********
# The default value for this directory is empty; i.e., there
# is no sacado directory.
# If you have
# `Sacado <http://trilinos.sandia.gov/packages/sacado/>`_
# installed on your system, you can
# specify a value for *sacado_dir* .
# It must be such that
#
#    *sacado_dir* / ``include/Sacado.hpp``
#
# is a valid reference to ``Sacado.hpp`` .
# In this case, you can run the Sacado speed correctness tests
# by executing the following commands starting in the build directory:
# ::
#
#    make check_speed_sacado
#
# tape_addr_type
# **************
# The default value for this type is ``size_t`` .
# If the command line argument *tape_addr_type* is present,
# it specifies the type used for address in the AD recordings (tapes).
# The valid values for this argument are
# ``unsigned short int`` ,
# ``unsigned int`` ,
# ``size_t`` .
# The smaller the value of *sizeof* ( ``tape_addr_type`` ) ,
# the less memory is used.
# On the other hand, the value
#
#    ``std::numeric_limits<`` *tape_addr_type* >:: ``max`` ()
#
# must be larger than any of the following:
# :ref:`fun_property@size_op` ,
# :ref:`fun_property@size_op_arg` ,
# :ref:`size_par<fun_property@size_text>` ,
# :ref:`fun_property@size_par` ,
# :ref:`size_par<fun_property@size_VecAD>` .
#
# tape_id_type
# ************
# The default value for this type is ``size_t`` .
# If the command line argument *tape_id_type* is present,
# it specifies the type used for identifying tapes.
# The valid values for this argument are
# ``unsigned short int`` ,
# ``unsigned int`` ,
# ``size_t`` .
# The smaller the value of *sizeof* ( ``tape_id_type`` ) ,
# the less memory is used.
# On the other hand, the value
#
#    ``std::numeric_limits<`` *tape_id_type* >:: ``max`` ()
#
# must be larger than the maximum number of tapes per thread
# times :ref:`configure@max_num_threads` .
#
# make install
# ************
# Once you are satisfied that the tests are giving correct results,
# you can install CppAD into easy to use directories by executing the command
# ::
#
#    make install
#
# This will install CppAD in the location specified by
# :ref:`configure@prefix_dir` .
# You must have permission to write in the *prefix_dir*
# directory to execute this command.
# You may optionally specify a destination directory for the install; i.e.,
#
#    ``make install DESTDIR`` = *DestinationDirectory*
#
# {xrst_end configure}
# ----------------------------------------------------------------------------
#
# --help
if echo " $* " | grep ' --help ' > /dev/null
then
   cat << EOF
usage: <path_to_configure>/configure \\
   [--help] \\
   [--prefix=<prefix_dir>] \\
   [--with-bostvector] \\
   [--with-eigenvector] \\
   [--with-stdvector] \\
   [--enable-msvc] \\
   [MAX_NUM_THREADS=max_num_threads] \\
   [CXX_FLAGS=cxx_flags] \\
   [POSTFIX_DIR=postfix_dir] \\
   [ADOLC_DIR=adolc_dir] \\
   [BOOST_DIR=boost_dir] \\
   [EIGEN_DIR=eigne_dir] \\
   [FADBAD_DIR=fadbad_dir] \\
   [SACADO_DIR=sacado_dir] \\
   [IPOPT_DIR=ipopt_dir] \\
   [TAPE_ADDR_TYPE=tape_addr_type] \\
   [TAPE_ID_TYPE=tape_id_type]

The --help option just prints this message and exits.
EOF
   echo 'configure: OK'
   exit 0
fi
# ----------------------------------------------------------------------------
first_executable() {
   first_executable_result=''
   while [ "$first_executable_result" == '' ] && [ "$1" != '' ]
   do
      if which $1 >& /dev/null
      then
         first_executable_result="$(which $1)"
      fi
      shift
   done
}
# ----------------------------------------------------------------------------
echo $0 $*
#
prefix="$HOME"
testvector=''
enable_msvc=''
with_clang=''
max_num_threads='64'
cxx_flags=''
postfix_dir='NOTFOUND'
adolc_dir='NOTFOUND'
boost_dir='NOTFOUND'
eigen_dir='NOTFOUND'
fadbad_dir='NOTFOUND'
sacado_dir='NOTFOUND'
ipopt_dir='NOTFOUND'
tape_addr_type='size_t'
tape_id_type='size_t'
while [ "$1" != '' ]
do
   case "$1" in

      --prefix=*)
      prefix=$( echo $1 | sed -e 's|^--prefix=||' )
      ;;

      --with-boostvector | --with-eigenvector | --with-stdvector)
      if [ "$testvector" != '' ]
      then
         echo 'configure: Cannot specify more than one test vector'
         exit 1
      fi
      testvector=$( echo $1 | sed -e 's|^--with-||' -e 's|vector$||')
      ;;

      --enable-msvc)
      enable_msvc='yes'
      ;;

      --with-clang)
      with_clang='yes'
      ;;

      CXX_FLAGS=*)
      cxx_flags=$( echo $1 | sed -e 's|CXX_FLAGS=||' )
      ;;

      MAX_NUM_THREADS=*)
      max_num_threads=$( echo $1 | sed -e 's|MAX_NUM_THREADS=||' )
      ;;

      POSTFIX_DIR=*)
      postfix_dir=$( echo $1 | sed -e 's|POSTFIX_DIR=||' )
      ;;

      ADOLC_DIR=*)
      adolc_dir=$( echo $1 | sed -e 's|ADOLC_DIR=||' )
      ;;

      BOOST_DIR=*)
      boost_dir=$( echo $1 | sed -e 's|BOOST_DIR=||' )
      ;;

      EIGEN_DIR=*)
      eigen_dir=$( echo $1 | sed -e 's|EIGEN_DIR=||' )
      ;;

      FADBAD_DIR=*)
      fadbad_dir=$( echo $1 | sed -e 's|FADBAD_DIR=||' )
      ;;

      SACADO_DIR=*)
      sacado_dir=$( echo $1 | sed -e 's|SACADO_DIR=||' )
      ;;

      IPOPT_DIR=*)
      ipopt_dir=$( echo $1 | sed -e 's|IPOPT_DIR=||' )
      ;;

      TAPE_ADDR_TYPE=*)
      tape_addr_type=$( echo $1 | sed -e 's|TAPE_ADDR_TYPE=||' )
      ;;

      TAPE_ID_TYPE=*)
      tape_id_type=$( echo $1 | sed -e 's|TAPE_ID_TYPE=||' )
      ;;

      *)
      echo "configure: '$1' is not a valid option. Use following to get help:"
      echo "$0 --help"
      exit 1
   esac
   shift
done
if [ "$testvector" == '' ]
then
   testvector='cppad'
fi
if [ "$with_clang" == 'yes' ] && [ "$enable_msvc" == 'yes' ]
then
   echo 'configure: cannot specify both --enable-mcvc and --with-clang'
   exit 1
fi
# ----------------------------------------------------------------------------
# type_makefile
if [ "$enable_msvc" == 'yes' ]
then
   type_makefile='NMake Makefiles'
else
   if [[ $(uname -s) =~ ^MSYS_.* ]]
   then
      type_makefile='MSYS Makefiles'
   else
      type_makefile='Unix Makefiles'
   fi
fi
#
# cmake_cxx_compiler
cmake_cxx_compiler=''
if [ "$enable_msvc" == 'yes' ]
then
   first_executable icl cl g++
else
   first_executable g++ clang++ CC pgCC icpc gpp cxx cc++
fi
if [ "$with_clang" == 'yes' ]
then
   cmake_cxx_compiler='-D CMAKE_CXX_COMPILER=clang++'
elif [ "$first_executable_result" != '' ]
then
   cmake_cxx_compiler="-D CMAKE_CXX_COMPILER=$first_executable_result"
fi
#
# cmake_c_compiler
cmake_c_compiler=''
if [ "$enable_msvc" == 'yes' ]
then
   first_executable icl cl gcc
else
   first_executable gcc clang cc pgcc icc
fi
if [ "$with_clang" == 'yes' ]
then
   cmake_c_compiler='-D CMAKE_C_COMPLIER=clang'
elif [ "$first_executable_result" != '' ]
then
   cmake_c_compiler="-D CMAKE_C_COMPILER=$first_executable_result"
fi
#
# CMAKE_Fortran_COMPILER
cmake_fortran_compiler=''
if [ "$enable_msvc" == 'yes' ]
then
   first_executable ifort gfortran
else
   first_executable gfortran ifort g95 fort77 f77 g77 pgf90 pgf77 ifc frt af77
fi
if [ "$first_executable_result" != '' ]
then
   cmake_fortran_compiler="-D CMAKE_Fortan_COMPILER=$first_executable_result"
fi
#
# source_dir
source_dir=$( echo $0 | sed -e 's|/configure.sh$||' )
if [ "$source_dir" == "$0" ] || [ "$source_dir" == '.' ]
then
   echo 'configure: cannot run configure.sh in the current directory'
   exit 1
fi
#
# include_adolc
include_adolc='false'
if [ "$adolc_dir" != 'NOTFOUND' ]
then
   include_adolc='true'
fi
#
# include_ipopt
include_ipopt='false'
if [ "$ipopt_dir" != 'NOTFOUND' ]
then
   include_ipopt='true'
fi
#
# cppad_cxx_flags
cppad_cxx_flags=$( echo " $cxx_flags " | sed -e 's| -g ||' )
cppad_cxx_fags="$cppad_flags -D CPPAD_DEBUG_AND_RELEASE"
#
# cmake_install_libdirs
cmake_install_libdirs='lib;lib64'
if [ -d '/usr/lib64' ]
then
   cmake_install_libdirs='lib64;lib'
fi
#
# CMakeCache.txt
if [ -e CMakeCache.txt ]
then
   rm CMakeCache.txt
fi
echo
echo cmake \
   -U .+ \
   -S "$source_dir" \
   -B . \
   \
   -D CMAKE_VERBOSE_MAKEFILE=0 \
   -G "'$type_makefile'" \
   \
   $cmake_cxx_compiler \
   $cmake_c_compiler \
   $cmake_fortran_compiler \
   \
   -D cppad_prefix="$prefix" \
   -D cppad_postfix="$postfix_dir" \
   \
   -D cmake_install_includedirs=include \
   -D cmake_install_libdirs=$cmake_install_libdirs \
   \
   -D cmake_install_datadir=share \
   -D cmake_install_docdir=share/doc \
   \
   -D include_adolc=$include_adolc \
   -D include_ipopt=$include_adolc \
   -D include_cppadcg=false \
   \
   -D eigen_prefix="$eigen_dir" \
   -D colpack_prefix='NOTFOUND' \
   -D fadbad_prefix="$fadbad_dir" \
   -D sacado_prefix="$sacado_dir" \
   \
   -D cppad_cxx_flags="$cxx_flags" \
   -D cppad_profile_flag='' \
   -D cppad_testvector="$testvector"  \
   -D cppad_max_num_threads="$max_num_threads" \
   -D cppad_tape_id_type="$tape_id_type" \
   -D cppad_tape_addr_type="$tape_addr_type" \
   -D cppad_debug_which='debug_none'
#
cmake \
   -U .+ \
   -S "$source_dir" \
   -B . \
   \
   -D CMAKE_VERBOSE_MAKEFILE=0 \
   -G "$type_makefile" \
   \
   -D cppad_prefix="$prefix" \
   -D cppad_postfix="$postfix_dir" \
   \
   -D cmake_install_includedirs=include \
   -D cmake_install_libdirs=$cmake_install_libdirs \
   \
   -D cmake_install_datadir=share \
   -D cmake_install_docdir=share/doc \
   \
   -D include_adolc=$include_adolc \
   -D include_ipopt=$include_adolc \
   -D include_cppadcg=false \
   \
   -D eigen_prefix="$eigen_prefix" \
   -D colpack_prefix='NOTFOUND' \
   -D fadbad_prefix="$fadbad_prefix" \
   -D sacado_prefix="$sacado_prefix" \
   \
   -D cppad_cxx_flags="$cxx_flags" \
   -D cppad_profile_flag='' \
   -D cppad_testvector="$testvector"  \
   -D cppad_max_num_threads="$max_num_threads" \
   -D cppad_tape_id_type="$tape_id_type" \
   -D cppad_tape_addr_type="$tape_addr_type" \
   -D cppad_debug_which='debug_none'
# ----------------------------------------------------------------------------
echo 'configure.sh: OK'
exit 0
