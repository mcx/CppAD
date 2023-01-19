# ifndef TAPE_HPP
# define TAPE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "type.hpp"
# include "add_op.hpp"
# include "con_op.hpp"
# include "sub_op.hpp"
/*
{xrst_begin val_op_tape dev}
{xrst_spell
   dep
   initializes
}

The Value Operator Tape
#######################

Prototype
*********
{xrst_literal
   // BEGIN_TAPE_T
   // END_TAPE_T
}

Purpose
*******
This class is used to define a function using a sequence of operators.


n_val
*****
{xrst_literal
   // BEGIN_N_VAL
   // END_N_VAL
}
This is the number of elements in the value vector.

arg_vec
*******
{xrst_literal
   // BEGIN_ARG_VEC
   // END_ARG_VEC
}
The elements of this vector at non-negative integers that connect
with an operator to make an operator usage.

con_vec
*******
{xrst_literal
   // BEGIN_CON_VEC
   // END_CON_VEC
}
This is a vector of constants that are loaded into the value vector.
(Constants do not depend on the independent values.)

op_vec
******
{xrst_literal
   // BEGIN_OP_VEC
   // END_OP_VEC
}
Each element of this vector corresponds to an operator usage.
The order of the vector is the order of operations.

info
****
{xrst_literal
   // BEGIN_OP_INFO_T
   // END_OP_INFO_T
}
This is the information corresponding to one operator usage.

arg_index
=========
this specifies the offset in arg_vec for this operator usage.

rex_index
=========
this specifies the offset in the value vector
of the first result for this operator usage.

op_ptr
======
this is a pointer to the operator corresponding to this
operator usage.

dep_vec
*******
{xrst_literal
   // BEGIN_DEP_VEC
   // END_DEP_VEC
}
This is the vector of dependent indices in the values vector.
The function corresponding to a tape maps the independent values
to the dependent values.

swap
****
{xrst_literal
   // BEGIN_SWAP
   // END_SWAP
}
This swaps the contents of this tape with another tape.

set_ind
*******
{xrst_literal
   // BEGIN_SET_IND
   // END_SET_IND
}
This initializes the tape to be empty,
then sets the number of independent variables, and
then places the constant zero directly after the last independent variable.
This is the first step in a creating a recording.

set_dep
*******
{xrst_literal
   // BEGIN_SET_DEP
   // END_SET_DEP
}
This sets the dependent variables to the corresponding indices
in the value vector.

eval
****
{xrst_literal
   // BEGIN_EVAL
   // END_EVAL
}
Given the independent values, this routine execute the operators
in order to evaluate the rest of the value vector.

trace
=====
If this is try, the :ref:`val_op_base@print_op` function is used
to print each of the operators.

val_vec
=======
This vector has size equal to *n_val*.
The first *n_ind* elements are inputs.
The rest of the elements are outputs.


{xrst_end val_op_tape}
*/
// BEGIN_TAPE_T
template <class Value> class tape_t {
// END_TAPE_T
public :
   // BEGIN_OP_INFO_T
   typedef struct {
      addr_t      arg_index;    // starting index in arg_vec for an operator
      addr_t      res_index;    // starting result index in val_vec
      op_base_t<Value>* op_ptr; // pointer to this operator
   } op_info_t;
   // END_OP_INFO_T
private :
   size_t                n_ind_;     // number of independent values
   size_t                n_val_;     // index in val_vec of record result
   Vector<addr_t>        arg_vec_;   // index of operator arguments in val_vec
   Vector<Value>         con_vec_;   // constants used by the tape
   Vector<op_info_t>     op_vec_;    // operators that define this function
   Vector<addr_t>        dep_vec_;   // index in val_vec of dependent variables
   //
public :
   // ------------------------------------------------------------------------
   // BEGIN_N_VAL
   size_t n_val(void) const
   {  return n_val_; }
   // END_N_VAL
   //
   // BEGIN_ARG_VEC
   const Vector<addr_t>& arg_vec(void) const
   {  return arg_vec_; }
   // END_ARG_VEC
   //
   // BEGIN_CON_VEC
   const Vector<Value>& con_vec(void) const
   {  return con_vec_; }
   // END_CON_VEC
   //
   // BEGIN_OP_VEC
   const Vector<op_info_t>& op_vec(void) const
   {  return op_vec_; }
   // END_OP_VEC
   //
   // BEGIN_DEP_VEC
   const Vector<addr_t>& dep_vec(void) const
   {  return dep_vec_; }
   // END_DEP_VEC
   // ------------------------------------------------------------------------
   // BEGIN_SWAP
   void swap(tape_t& other)
   // END_SWAP
   {  std::swap( n_ind_, other.n_ind_ );
      std::swap( n_val_, other.n_val_);
      arg_vec_.swap( other.arg_vec_ );
      con_vec_.swap( other.con_vec_ );
      op_vec_.swap( other.op_vec_ );
      dep_vec_.swap( other.dep_vec_ );
   }
   //
   // BEGIN_SET_IND
   void set_ind(size_t n_ind)
   // END_SET_IND
   {  n_ind_ = n_ind;
      n_val_ = n_ind;
      dep_vec_.resize(0);
      op_vec_.resize(0);
      con_vec_.resize(0);
# ifndef NDEBUG
      size_t zero = size_t( record_con_op(Value(0.0)) );
      assert ( zero == n_ind_ );
# else
      record_con_op( Value(0.0) );
# endif
      assert( n_val_ == n_ind + 1 );
   }
   //
   // BEGIN_SET_DEP
   void set_dep(const Vector<addr_t>& dep_vec)
   {  dep_vec_ = dep_vec; }
   // END_SET_DEP
   //
   // record_op
   addr_t record_op(op_enum_t op_enum, const Vector<addr_t>& op_arg);
   // record_con_op
   addr_t record_con_op(const Value& constant);
   // record_fun_op
   addr_t record_fun_op(
      size_t function_id            ,
      size_t call_id                ,
      size_t n_res                  ,
      const Vector<addr_t>& fun_arg
   );
   // BEGIN_EVAL
   void eval(
      bool          trace  ,
      Vector<Value>& val_vec) const
   // END_EVAL
   {  assert( val_vec.size() == static_cast<size_t>(n_val_) );
      //
      // trace
      if( trace )
      {  // no operators for independent variables
         std::printf("independent vector\n");
         for(size_t res_index = 0; res_index < n_ind_; ++res_index)
         {  Value res = val_vec[res_index];
            std::printf(
               "%5ld  %10.3g\n", res_index, res
            );
         }
         std::printf("operators\n");
      }
      //
      // i_op
      size_t n_op = op_vec_.size();
      for(size_t i_op = 0; i_op < n_op; ++i_op)
      {  //
         // arg_index, res_index
         const op_info_t& op_info     = op_vec_[i_op];
         op_base_t<Value>* op_ptr     = op_info.op_ptr;
         addr_t            arg_index  = op_info.arg_index;
         addr_t            res_index  = op_info.res_index;
         //
         // op_base_t<Value>::eval
         op_ptr->eval(
            trace, arg_index, arg_vec_, con_vec_, res_index, val_vec
         );
      }
      // trace
      if( trace )
      {  // no operators for dependent variables
         std::printf("dependent vector\n");
         for(size_t i = 0; i < dep_vec_.size(); ++i)
         {  addr_t res_index = dep_vec_[i];
            Value res        = val_vec[res_index];
            std::printf(
               "%5d  %10.3g\n", res_index, res
            );
         }
         // space after end of this tape
         std::printf("\n");
      }
   }
   //
   // renumber
   void renumber(void);
   //
   // dead_code
   void dead_code(void);
};

# include "record_op.hpp"
# include "renumber.hpp"
# include "dead_code.hpp"
# include "fun_op.hpp"

# endif
