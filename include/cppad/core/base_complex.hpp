# ifndef CPPAD_CORE_BASE_COMPLEX_HPP
# define CPPAD_CORE_BASE_COMPLEX_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/configure.hpp>
# include <limits>
# include <complex>

/*
{xrst_begin base_complex.hpp}
{xrst_spell
   azmul
   complier
   isnan
}

Enable use of AD<Base> where Base is std::complex<double>
#########################################################
{xrst_toc_hidden
   example/general/complex_poly.cpp
}
Example
*******
The file :ref:`complex_poly.cpp-name` contains an example use of
``std::complex<double>`` type for a CppAD *Base* type.

Include Order
*************
This file is included before ``<cppad/cppad.hpp>``
so it is necessary to define the error handler
in addition to including
:ref:`base_require.hpp<base_require@Include Order>`
{xrst_spell_off}
{xrst_code cpp} */
# include <limits>
# include <complex>
# include <cppad/base_require.hpp>
# include <cppad/core/cppad_assert.hpp>

/* {xrst_code}
{xrst_spell_on}

CondExpOp
*********
The type ``std::complex<double>`` does not support the
``<`` , ``<=`` , ``>=`` , and ``>`` operators; see
:ref:`base_cond_exp@CondExpTemplate@Not Ordered` .
Hence these operators and ``CondExpOp`` function are defined by
{xrst_spell_off}
{xrst_code cpp} */
# define CPPAD_TEMP(op)                                          \
   inline bool operator op(                                      \
      const std::complex<double>& left   ,                       \
      const std::complex<double>& right  )                       \
   {  CppAD::ErrorHandler::Call(                                 \
         true     , __LINE__ , __FILE__ ,                        \
         "std::complex<double> " #op " std::complex<double>" ,   \
         "Error: std::complex is not an ordered type"            \
      );                                                         \
      return false;                                              \
   }
namespace CppAD {
   CPPAD_TEMP(<)
   CPPAD_TEMP(<=)
   CPPAD_TEMP(>=)
   CPPAD_TEMP(>)
   inline std::complex<double> CondExpOp(
      enum CppAD::CompareOp      cop        ,
      const std::complex<double> &left      ,
      const std::complex<double> &right     ,
      const std::complex<double> &trueCase  ,
      const std::complex<double> &falseCase )
   {  CppAD::ErrorHandler::Call(
         true     , __LINE__ , __FILE__ ,
         "std::complex<float> CondExpOp(...)",
         "Error: cannot use CondExp with a complex type"
      );
      return std::complex<double>(0);
   }
}
# undef CPPAD_TEMP
/* {xrst_code}
{xrst_spell_on}


CondExpRel
**********
The :ref:`CPPAD_COND_EXP_REL<base_cond_exp@CondExpRel>` macro invocation
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   CPPAD_COND_EXP_REL( std::complex<double> )
}
/* {xrst_code}
{xrst_spell_on}
used ``CondExpOp`` above to
define ``CondExp`` *Rel* for ``std::complex<double>`` arguments
and *Rel* equal to
``Lt`` , ``Le`` , ``Eq`` , ``Ge`` , and ``Gt`` .

EqualOpSeq
**********
Complex numbers do not carry operation sequence information.
Thus they are equal in this sense if and only if there values are equal.
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline bool EqualOpSeq(
      const std::complex<double> &x ,
      const std::complex<double> &y )
   {  return x == y;
   }
}
/* {xrst_code}
{xrst_spell_on}

Identical
*********
Complex numbers do not carry operation sequence information.
Thus they are all parameters so the identical functions just check values.
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline bool IdenticalCon(const std::complex<double> &x)
   {  return true; }
   inline bool IdenticalZero(const std::complex<double> &x)
   {  return (x == std::complex<double>(0., 0.) ); }
   inline bool IdenticalOne(const std::complex<double> &x)
   {  return (x == std::complex<double>(1., 0.) ); }
   inline bool IdenticalEqualCon(
      const std::complex<double> &x, const std::complex<double> &y)
   {  return (x == y); }
}
/* {xrst_code}
{xrst_spell_on}

Ordered
*******
Complex types do not support comparison operators,
{xrst_spell_off}
{xrst_code cpp} */
# undef  CPPAD_USER_MACRO
# define CPPAD_USER_MACRO(Fun)                                     \
inline bool Fun(const std::complex<double>& x)                     \
{      CppAD::ErrorHandler::Call(                                  \
               true     , __LINE__ , __FILE__ ,                    \
               #Fun"(x)",                                          \
               "Error: cannot use " #Fun " with x complex<double> " \
       );                                                          \
       return false;                                               \
}
namespace CppAD {
   CPPAD_USER_MACRO(LessThanZero)
   CPPAD_USER_MACRO(LessThanOrZero)
   CPPAD_USER_MACRO(GreaterThanOrZero)
   CPPAD_USER_MACRO(GreaterThanZero)
   inline bool abs_geq(
      const std::complex<double>& x ,
      const std::complex<double>& y )
   {  return std::abs(x) >= std::abs(y); }
}
/* {xrst_code}
{xrst_spell_on}

Integer
*******
The implementation of this function must agree
with the CppAD user specifications for complex arguments to the
:ref:`Integer<Integer@x@Complex Types>` function:
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline int Integer(const std::complex<double> &x)
   {  return static_cast<int>( x.real() ); }
}
/* {xrst_code}
{xrst_spell_on}

azmul
*****
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   CPPAD_AZMUL( std::complex<double> )
}
/* {xrst_code}
{xrst_spell_on}

isnan
*****
The gcc 4.1.1 complier defines the function

   ``int std::complex<double>::isnan`` ( ``std::complex<double>`` *z*  )

(which is not specified in the C++ 1998 standard ISO/IEC 14882).
This causes an ambiguity between the function above and the CppAD
:ref:`isnan<nan-name>` template function.
We avoid this ambiguity by defining a non-template version of
this function in the CppAD namespace.
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline bool isnan(const std::complex<double>& z)
   {  return (z != z);
   }
}
/* {xrst_code}
{xrst_spell_on}

Valid Unary Math
****************
The following macro invocations define the standard unary
math functions that are valid with complex arguments and are
required to use ``AD< std::complex<double> >`` .
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   CPPAD_STANDARD_MATH_UNARY(std::complex<double>, cos)
   CPPAD_STANDARD_MATH_UNARY(std::complex<double>, cosh)
   CPPAD_STANDARD_MATH_UNARY(std::complex<double>, exp)
   CPPAD_STANDARD_MATH_UNARY(std::complex<double>, log)
   CPPAD_STANDARD_MATH_UNARY(std::complex<double>, sin)
   CPPAD_STANDARD_MATH_UNARY(std::complex<double>, sinh)
   CPPAD_STANDARD_MATH_UNARY(std::complex<double>, sqrt)
}
/* {xrst_code}
{xrst_spell_on}

Invalid Unary Math
******************
The following macro definition and invocations define the standard unary
math functions that are invalid with complex arguments and are
required to use ``AD< std::complex<double> >`` .
{xrst_spell_off}
{xrst_code cpp} */
# undef  CPPAD_USER_MACRO
# define CPPAD_USER_MACRO(Fun)                                     \
inline std::complex<double> Fun(const std::complex<double>& x)     \
{      CppAD::ErrorHandler::Call(                                  \
               true     , __LINE__ , __FILE__ ,                    \
               #Fun"(x)",                                          \
               "Error: cannot use " #Fun " with x complex<double> " \
       );                                                          \
       return std::complex<double>(0);                             \
}
namespace CppAD {
   CPPAD_USER_MACRO(abs)
   CPPAD_USER_MACRO(fabs)
   CPPAD_USER_MACRO(acos)
   CPPAD_USER_MACRO(asin)
   CPPAD_USER_MACRO(atan)
   CPPAD_USER_MACRO(sign)
   CPPAD_USER_MACRO(asinh)
   CPPAD_USER_MACRO(acosh)
   CPPAD_USER_MACRO(atanh)
   CPPAD_USER_MACRO(erf)
   CPPAD_USER_MACRO(erfc)
   CPPAD_USER_MACRO(expm1)
   CPPAD_USER_MACRO(log1p)
}
/* {xrst_code}
{xrst_spell_on}

pow
***
The following defines a ``CppAD::pow`` function that
is required to use ``AD< std::complex<double> >`` :
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline std::complex<double> pow(
      const std::complex<double> &x ,
      const std::complex<double> &y )
   {  return std::pow(x, y); }
}
/* {xrst_code}
{xrst_spell_on}

numeric_limits
**************
The following defines the CppAD :ref:`numeric_limits-name`
for the type ``std::complex<double>`` :
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   CPPAD_NUMERIC_LIMITS(double, std::complex<double>)
}
/* {xrst_code}
{xrst_spell_on}

to_string
*********
The following defines the function CppAD :ref:`to_string-name`
for the type ``std::complex<double>`` :
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   CPPAD_TO_STRING(std::complex<double>)
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end base_complex.hpp}
*/
# undef  CPPAD_USER_MACRO_ONE
# define CPPAD_USER_MACRO_ONE(Fun)                                 \
inline bool Fun(const std::complex<float>& x)                      \
{      CppAD::ErrorHandler::Call(                                  \
               true     , __LINE__ , __FILE__ ,                    \
               #Fun"(x)",                                          \
               "Error: cannot use " #Fun " with x complex<float> " \
       );                                                          \
       return false;                                               \
}
# undef  CPPAD_USER_MACRO_TWO
# define CPPAD_USER_MACRO_TWO(Fun)                                 \
inline std::complex<float> Fun(const std::complex<float>& x)       \
{      CppAD::ErrorHandler::Call(                                  \
               true     , __LINE__ , __FILE__ ,                    \
               #Fun"(x)",                                          \
               "Error: cannot use " #Fun " with x complex<float> " \
       );                                                          \
       return std::complex<float>(0);                              \
}
namespace CppAD {
   // CondExpOp ------------------------------------------------------
   inline std::complex<float> CondExpOp(
      enum CppAD::CompareOp      cop       ,
      const std::complex<float> &left      ,
      const std::complex<float> &right     ,
      const std::complex<float> &trueCase  ,
      const std::complex<float> &falseCase )
   {  CppAD::ErrorHandler::Call(
         true     , __LINE__ , __FILE__ ,
         "std::complex<float> CondExpOp(...)",
         "Error: cannot use CondExp with a complex type"
      );
      return std::complex<float>(0);
   }
   // CondExpRel --------------------------------------------------------
   CPPAD_COND_EXP_REL( std::complex<float> )
   // EqualOpSeq -----------------------------------------------------
   inline bool EqualOpSeq(
      const std::complex<float> &x ,
      const std::complex<float> &y )
   {  return x == y;
   }
   // Identical ------------------------------------------------------
   inline bool IdenticalCon(const std::complex<float> &x)
   {  return true; }
   inline bool IdenticalZero(const std::complex<float> &x)
   {  return (x == std::complex<float>(0., 0.) ); }
   inline bool IdenticalOne(const std::complex<float> &x)
   {  return (x == std::complex<float>(1., 0.) ); }
   inline bool IdenticalEqualCon(
      const std::complex<float> &x, const std::complex<float> &y)
   {  return (x == y); }
   // Ordered --------------------------------------------------------
   CPPAD_USER_MACRO_ONE(LessThanZero)
   CPPAD_USER_MACRO_ONE(LessThanOrZero)
   CPPAD_USER_MACRO_ONE(GreaterThanOrZero)
   CPPAD_USER_MACRO_ONE(GreaterThanZero)
   inline bool abs_geq(
      const std::complex<float>& x ,
      const std::complex<float>& y )
   {  return std::abs(x) >= std::abs(y); }
   // Integer ------------------------------------------------------
   inline int Integer(const std::complex<float> &x)
   {  return static_cast<int>( x.real() ); }
   // isnan -------------------------------------------------------------
   inline bool isnan(const std::complex<float>& z)
   {  return (z != z);
   }
   // Valid standard math functions --------------------------------
   CPPAD_STANDARD_MATH_UNARY(std::complex<float>, cos)
   CPPAD_STANDARD_MATH_UNARY(std::complex<float>, cosh)
   CPPAD_STANDARD_MATH_UNARY(std::complex<float>, exp)
   CPPAD_STANDARD_MATH_UNARY(std::complex<float>, log)
   CPPAD_STANDARD_MATH_UNARY(std::complex<float>, sin)
   CPPAD_STANDARD_MATH_UNARY(std::complex<float>, sinh)
   CPPAD_STANDARD_MATH_UNARY(std::complex<float>, sqrt)
   // Invalid standrd math functions -------------------------------
   CPPAD_USER_MACRO_TWO(abs)
   CPPAD_USER_MACRO_TWO(acos)
   CPPAD_USER_MACRO_TWO(asin)
   CPPAD_USER_MACRO_TWO(atan)
   CPPAD_USER_MACRO_TWO(sign)
   // The pow function
   inline std::complex<float> pow(
      const std::complex<float> &x ,
      const std::complex<float> &y )
   {  return std::pow(x, y); }
   // numeric_limits -------------------------------------------------
   CPPAD_NUMERIC_LIMITS(float, std::complex<float>)
   // to_string -------------------------------------------------
   CPPAD_TO_STRING(std::complex<float>)
}

// undefine macros only used by this file
# undef CPPAD_USER_MACRO
# undef CPPAD_USER_MACRO_ONE
# undef CPPAD_USER_MACRO_TWO

# endif
