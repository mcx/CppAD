# ifndef CPPAD_LOCAL_PLAY_RANDOM_ITERATOR_HPP
# define CPPAD_LOCAL_PLAY_RANDOM_ITERATOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// ----------------------------------------------------------------------------

// BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE
namespace CppAD { namespace local { namespace play {

/*!
\file random_iterator.hpp
*/

/*!
Constant random iterator for a player object.

\tparam Addr
An integer type capable of representing the largest value in the vectors
var_arg, op2arg_index, op2var_index, var2op_index.
*/
template <class Addr>
class const_random_iterator {
private:
   /// vector of operators on the tape
   const pod_vector<opcode_t>* var_op_;

   /// vector of arguments for all the operators
   /// (note that this is same type as used in recorder; i.e., addr_t)
   const pod_vector<addr_t>* var_arg_;

   /// mapping from operator index to index of first argument in var_arg_
   const pod_vector<Addr>* op2arg_index_;

   /// mapping from operator index to index of primary (last) result
   const pod_vector<Addr>* op2var_index_;

   /// mapping from primary variable index to operator index
   /// (only specified for primary variables)
   const pod_vector<Addr>* var2op_index_;

public:
   /// index_t
   typedef Addr index_t;
   //
   /// default constructor
   const_random_iterator(void) :
   var_op_(nullptr)     ,
   var_arg_(nullptr)    ,
   op2arg_index_(nullptr) ,
   op2var_index_(nullptr) ,
   var2op_index_(nullptr)
   { }
   /// default assignment operator
   void operator=(const const_random_iterator& rhs)
   {
      var_op_          = rhs.var_op_;
      op2arg_index_    = rhs.op2arg_index_;
      op2var_index_    = rhs.op2var_index_;
      var2op_index_    = rhs.var2op_index_;
      return;
   }
   /*!
   Create a random iterator

   \par var2op_index
   This variable is not needed and can be null if the var2op member
   function is not used.
   */
   const_random_iterator(
      const pod_vector<opcode_t>&           var_op     , ///< var_op_
      const pod_vector<addr_t>&             var_arg    , ///< var_arg_
      const pod_vector<Addr>*               op2arg_index , ///< op2ar_vec_
      const pod_vector<Addr>*               op2var_index , ///< op2var_index_
      const pod_vector<Addr>*               var2op_index ) ///< var2op_index_
   :
   var_op_          ( &var_op    )   ,
   var_arg_         ( &var_arg   )   ,
   op2arg_index_    ( op2arg_index )   ,
   op2var_index_    ( op2var_index )   ,
   var2op_index_    ( var2op_index )
   { }
   /*!
   \brief
   fetch the information corresponding to an operator

   \param op_index
   index for this operator [in]

   \param op [out]
   op code for this operator.

   \param op_arg [out]
   pointer to the first arguement to this operator.

   \param var_index [out]
   index of the last variable (primary variable) for this operator.
   If there is no primary variable for this operator, i_var not sepcified
   and could have any value.
   */
   void op_info(
      size_t         op_index   ,
      op_code_var&   op         ,
      const addr_t*& op_arg     ,
      size_t&        var_index  ) const
   {  op        = op_code_var( (*var_op_)[op_index] );
      op_arg    = (*op2arg_index_)[op_index] + var_arg_->data();
      var_index = size_t( (*op2var_index_)[op_index] );
      return;
   }
   /*!
   \brief
   map variable index to operator index.

   \param var_index
   must be the index of a primary variable.

   \return
   is the index of the operator corresponding to this primary variable.
   */
   size_t var2op(size_t var_index) const
   {  // check that var2op_index was not null in constructor
      CPPAD_ASSERT_UNKNOWN( var2op_index_ != nullptr );
      //
      // operator index
      size_t op_index = size_t( (*var2op_index_)[var_index] );
      //
      // check that var_index is a primary variable index (see random_setup)
      CPPAD_ASSERT_UNKNOWN( op_index < var_op_->size() );
      //
      return op_index;
   }
   /// get operator corresponding to operator index
   op_code_var get_op(size_t op_index) const
   {  return op_code_var( (*var_op_)[op_index] );
   }
   /// number of operators
   size_t num_op(void) const
   {  return var_op_->size(); }
   //
   /// number of variables
   size_t num_var(void) const
   {  return var2op_index_->size(); }
};

} } } // BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE

# endif
