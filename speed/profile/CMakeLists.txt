# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2003-25 Bradley M. Bell
# ----------------------------------------------------------------------------
# Build the speed/profile directory tests
# Inherit build type from ../CMakeList.txt

# Adds flags to the compiler command line for sources in the current directory
# and below. This command can be used to add any flags, but it was originally
# intended to add preprocessor definitions.
ADD_DEFINITIONS("-DCPPAD_PROFILE_SPEED ${cppad_profile_flag}")

# Extra flags used by the linker when creating an executable.
SET(CMAKE_EXE_LINKER_FLAGS ${cppad_profile_flag} ${cppad_link_flags})

# Local include directories to search (not in package_prefix/includdir)
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}/../src )

# add_executable(<name> [WIN32] [MACOSX_BUNDLE] [EXCLUDE_FROM_ALL]
#                 source1 source2 ... sourceN
# )
SET(source_list ../main.cpp
   ../cppad/det_lu.cpp
   ../cppad/det_minor.cpp
   ../cppad/mat_mul.cpp
   ../cppad/ode.cpp
   ../cppad/poly.cpp
   ../cppad/sparse_hessian.cpp
   ../cppad/sparse_jacobian.cpp
)
set_compile_flags( speed_profile "${cppad_debug_which}" "${source_list}" )
#
ADD_EXECUTABLE( speed_profile EXCLUDE_FROM_ALL ${source_list} )
#
# List of libraries to be linked into the specified target
TARGET_LINK_LIBRARIES(speed_profile
   ${cppad_lib}
   ${colpack_libs}
)
# profile builds it own copy of src/speed library (see ADD_EXECUTABLE above)
# TARGET_LINK_LIBRARIES(speed_profile speed_src )
TARGET_LINK_LIBRARIES(speed_profile
   speed_src
   ${cppad_lib}
   ${colpack_libs}
)

# check_speed_profile
add_check_executable(check_speed profile "correct 54321")

# Sed script to make gprof output more readable, use:
#  ./speed_profile speed 54321
#  gprof speed_profile gmon.out | sed -f 'gprof.sed' > gprof.out
#
# configure_file(InputFile OutputFile [COPYONLY] [ESCAPE_QUOTES] [@ONLY])
CONFIGURE_FILE(
   ${CMAKE_CURRENT_SOURCE_DIR}/gprof.sed.in
   ${CMAKE_CURRENT_BINARY_DIR}/gprof.sed
   COPYONLY
)
