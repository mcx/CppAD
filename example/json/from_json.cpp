// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin from_json.cpp}

Convert Jason Graph to an ADFun Object: Example and Test
########################################################

Source Code
***********
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end from_json.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool from_json(void)
{  bool ok = true;
   using CppAD::vector;
   //
   // An AD graph example
   // node_1 : p[0]
   // node_2 : x[0]
   // node_3 : x[1]
   // node_4 : -2.0
   // node_5 : p[0] + x[0] + x[1]
   // node_6 : (p[0] + x[0] + x[1]) * (p[0] + x[0] + x[1])
   // y[0]   = (p[0] + x[0] + x[1]) * (p[0] + x[0] + x[1])
   // use single quote to avoid having to escape double quote
   std::string json =
      "{\n"
      "   'function_name'  : 'from_json example',\n"
      "   'op_define_vec'  : [ 3, [\n"
      "       { 'op_code':1, 'name':'add', 'n_arg':2 } ,\n"
      "       { 'op_code':2, 'name':'mul', 'n_arg':2 } ,\n"
      "       { 'op_code':3, 'name':'sum'            } ]\n"
      "   ],\n"
      "   'n_dynamic_ind'  : 1,\n"
      "   'n_variable_ind' : 2,\n"
      "   'constant_vec'   : [ 1, [ -2.0 ] ],\n"
      "   'op_usage_vec'   : [ 2, [\n"
      "       [ 3, 1, 3, [1, 2, 3 ] ] ,\n"
      "       [ 2, 5, 5             ] ] \n"
      "   ],\n"
      "   'dependent_vec'   : [ 1, [6] ] \n"
      "}\n";
   // Convert the single quote to double quote
   for(size_t i = 0; i < json.size(); ++i)
      if( json[i] == '\'' ) json[i] = '"';
   //
   CppAD::ADFun<double> fun;
   fun.from_json(json);
   //
   // Compute function value
   vector<double> p(1), x(2);
   p[0] = 1.0;
   x[0] = 2.0;
   x[1] = 3.0;
   fun.new_dynamic(p);
   vector<double> y = fun.Forward(0, x);
   ok  &= y[0] ==  (p[0] + x[0] + x[1]) * (p[0] + x[0] + x[1]);
   //
   // Conpute derivative value
   vector<double> jac = fun.Jacobian(x);
   ok &= jac[0] == 2.0 * (p[0] + x[0] + x[1]);
   ok &= jac[1] == 2.0 * (p[0] + x[0] + x[1]);
   //
   return ok;
}
// END C++
