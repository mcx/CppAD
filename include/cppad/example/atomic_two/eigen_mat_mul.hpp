# ifndef CPPAD_EXAMPLE_ATOMIC_TWO_EIGEN_MAT_MUL_HPP
# define CPPAD_EXAMPLE_ATOMIC_TWO_EIGEN_MAT_MUL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin atomic_two_eigen_mat_mul.hpp app}
{xrst_spell
  nr
  tr
}

atomic_two Eigen Matrix Multiply Class
######################################

See Also
********
:ref:`atomic_three_mat_mul.hpp-name`

Purpose
*******
Construct an atomic operation that computes the matrix product,
:math:`R = A \times \B{R}`
for any positive integers :math:`r`, :math:`m`, :math:`c`,
and any :math:`A \in \B{R}^{r \times m}`,
:math:`B \in \B{R}^{m \times c}`.

Matrix Dimensions
*****************
This example puts the matrix dimensions in the atomic function arguments,
instead of the :ref:`constructor<atomic_two_ctor-name>` , so that they can
be different for different calls to the atomic function.
These dimensions are:

.. list-table::
   :widths: auto

   * - *nr_left*
     - number of rows in the left matrix; i.e, :math:`r`
   * - *n_middle*
     - rows in the left matrix and columns in right; i.e, :math:`m`
   * - *nc_right*
     - number of columns in the right matrix; i.e., :math:`c`

Theory
******

Forward
=======
For :math:`k = 0 , \ldots`, the *k*-th order Taylor coefficient
:math:`R_k` is given by

.. math::

   R_k = \sum_{\ell = 0}^{k} A_\ell B_{k-\ell}

Product of Two Matrices
=======================
Suppose :math:`\bar{E}` is the derivative of the
scalar value function :math:`s(E)` with respect to :math:`E`; i.e.,

.. math::

   \bar{E}_{i,j} = \frac{ \partial s } { \partial E_{i,j} }

Also suppose that :math:`t` is a scalar valued argument and

.. math::

   E(t) = C(t) D(t)

It follows that

.. math::

   E'(t) = C'(t) D(t) +  C(t) D'(t)

.. math::

   (s \circ E)'(t)
   =
   \R{tr} [ \bar{E}^\R{T} E'(t) ]

.. math::

   =
   \R{tr} [ \bar{E}^\R{T} C'(t) D(t) ] +
   \R{tr} [ \bar{E}^\R{T} C(t) D'(t) ]

.. math::

   =
   \R{tr} [ D(t) \bar{E}^\R{T} C'(t) ] +
   \R{tr} [ \bar{E}^\R{T} C(t) D'(t) ]

.. math::

   \bar{C} = \bar{E} D^\R{T} \W{,}
   \bar{D} = C^\R{T} \bar{E}

Reverse
=======
Reverse mode eliminates :math:`R_k` as follows:
for :math:`\ell = 0, \ldots , k-1`,

.. math::

   \bar{A}_\ell     = \bar{A}_\ell     + \bar{R}_k B_{k-\ell}^\R{T}

.. math::

   \bar{B}_{k-\ell} =  \bar{B}_{k-\ell} + A_\ell^\R{T} \bar{R}_k

Start Class Definition
**********************
{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/cppad.hpp>
# include <Eigen/Core>

/* {xrst_code}
{xrst_spell_on}
Public
******

Types
=====
{xrst_spell_off}
{xrst_code cpp} */
namespace { // BEGIN_EMPTY_NAMESPACE

template <class Base>
class atomic_eigen_mat_mul : public CppAD::atomic_base<Base> {
public:
   // -----------------------------------------------------------
   // type of elements during calculation of derivatives
   typedef Base              scalar;
   // type of elements during taping
   typedef CppAD::AD<scalar> ad_scalar;
   // type of matrix during calculation of derivatives
   typedef Eigen::Matrix<
      scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>     matrix;
   // type of matrix during taping
   typedef Eigen::Matrix<
      ad_scalar, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor > ad_matrix;
/* {xrst_code}
{xrst_spell_on}
Constructor
===========
{xrst_spell_off}
{xrst_code cpp} */
   // constructor
   atomic_eigen_mat_mul(void) : CppAD::atomic_base<Base>(
      "atom_eigen_mat_mul"                             ,
      CppAD::atomic_base<Base>::set_sparsity_enum
   )
   { }
/* {xrst_code}
{xrst_spell_on}
op
==
{xrst_spell_off}
{xrst_code cpp} */
   // use atomic operation to multiply two AD matrices
   ad_matrix op(
      const ad_matrix&              left    ,
      const ad_matrix&              right   )
   {  size_t  nr_left   = size_t( left.rows() );
      size_t  n_middle  = size_t( left.cols() );
      size_t  nc_right  = size_t( right.cols() );
      assert( n_middle  == size_t( right.rows() )  );
      size_t  nx      = 3 + (nr_left + nc_right) * n_middle;
      size_t  ny      = nr_left * nc_right;
      size_t n_left   = nr_left * n_middle;
      size_t n_right  = n_middle * nc_right;
      size_t n_result = nr_left * nc_right;
      //
      assert( 3 + n_left + n_right == nx );
      assert( n_result == ny );
      // -----------------------------------------------------------------
      // packed version of left and right
      CPPAD_TESTVECTOR(ad_scalar) packed_arg(nx);
      //
      packed_arg[0] = ad_scalar( nr_left );
      packed_arg[1] = ad_scalar( n_middle );
      packed_arg[2] = ad_scalar( nc_right );
      for(size_t i = 0; i < n_left; i++)
         packed_arg[3 + i] = left.data()[i];
      for(size_t i = 0; i < n_right; i++)
         packed_arg[ 3 + n_left + i ] = right.data()[i];
      // ------------------------------------------------------------------
      // Packed version of result = left * right.
      // This as an atomic_base function call that CppAD uses
      // to store the atomic operation on the tape.
      CPPAD_TESTVECTOR(ad_scalar) packed_result(ny);
      (*this)(packed_arg, packed_result);
      // ------------------------------------------------------------------
      // unpack result matrix
      ad_matrix result(nr_left, nc_right);
      for(size_t i = 0; i < n_result; i++)
         result.data()[i] = packed_result[ i ];
      //
      return result;
   }
/* {xrst_code}
{xrst_spell_on}
Private
*******

Variables
=========
{xrst_spell_off}
{xrst_code cpp} */
private:
   // -------------------------------------------------------------
   // one forward mode vector of matrices for left, right, and result
   CppAD::vector<matrix> f_left_, f_right_, f_result_;
   // one reverse mode vector of matrices for left, right, and result
   CppAD::vector<matrix> r_left_, r_right_, r_result_;
   // -------------------------------------------------------------
/* {xrst_code}
{xrst_spell_on}
forward
=======
{xrst_spell_off}
{xrst_code cpp} */
   // forward mode routine called by CppAD
   virtual bool forward(
      // lowest order Taylor coefficient we are evaluating
      size_t                          p ,
      // highest order Taylor coefficient we are evaluating
      size_t                          q ,
      // which components of x are variables
      const CppAD::vector<bool>&      vx ,
      // which components of y are variables
      CppAD::vector<bool>&            vy ,
      // tx [ 3 + j * (q+1) + k ] is x_j^k
      const CppAD::vector<scalar>&    tx ,
      // ty [ i * (q+1) + k ] is y_i^k
      CppAD::vector<scalar>&          ty
   )
   {  size_t n_order  = q + 1;
      size_t nr_left  = size_t( CppAD::Integer( tx[ 0 * n_order + 0 ] ) );
      size_t n_middle = size_t( CppAD::Integer( tx[ 1 * n_order + 0 ] ) );
      size_t nc_right = size_t( CppAD::Integer( tx[ 2 * n_order + 0 ] ) );
# ifndef NDEBUG
      size_t  nx        = 3 + (nr_left + nc_right) * n_middle;
      size_t  ny        = nr_left * nc_right;
# endif
      //
      assert( vx.size() == 0 || nx == vx.size() );
      assert( vx.size() == 0 || ny == vy.size() );
      assert( nx * n_order == tx.size() );
      assert( ny * n_order == ty.size() );
      //
      size_t n_left   = nr_left * n_middle;
      size_t n_right  = n_middle * nc_right;
      size_t n_result = nr_left * nc_right;
      assert( 3 + n_left + n_right == nx );
      assert( n_result == ny );
      //
      // -------------------------------------------------------------------
      // make sure f_left_, f_right_, and f_result_ are large enough
      assert( f_left_.size() == f_right_.size() );
      assert( f_left_.size() == f_result_.size() );
      if( f_left_.size() < n_order )
      {  f_left_.resize(n_order);
         f_right_.resize(n_order);
         f_result_.resize(n_order);
         //
         for(size_t k = 0; k < n_order; k++)
         {  f_left_[k].resize( long(nr_left), long(n_middle) );
            f_right_[k].resize( long(n_middle), long(nc_right) );
            f_result_[k].resize( long(nr_left), long(nc_right) );
         }
      }
      // -------------------------------------------------------------------
      // unpack tx into f_left and f_right
      for(size_t k = 0; k < n_order; k++)
      {  // unpack left values for this order
         for(size_t i = 0; i < n_left; i++)
            f_left_[k].data()[i] = tx[ (3 + i) * n_order + k ];
         //
         // unpack right values for this order
         for(size_t i = 0; i < n_right; i++)
            f_right_[k].data()[i] = tx[ ( 3 + n_left + i) * n_order + k ];
      }
      // -------------------------------------------------------------------
      // result for each order
      // (we could avoid recalculting f_result_[k] for k=0,...,p-1)
      for(size_t k = 0; k < n_order; k++)
      {  // result[k] = sum_ell left[ell] * right[k-ell]
         f_result_[k] = matrix::Zero( long(nr_left), long(nc_right) );
         for(size_t ell = 0; ell <= k; ell++)
            f_result_[k] += f_left_[ell] * f_right_[k-ell];
      }
      // -------------------------------------------------------------------
      // pack result_ into ty
      for(size_t k = 0; k < n_order; k++)
      {  for(size_t i = 0; i < n_result; i++)
            ty[ i * n_order + k ] = f_result_[k].data()[i];
      }
      // ------------------------------------------------------------------
      // check if we are computing vy
      if( vx.size() == 0 )
         return true;
      // ------------------------------------------------------------------
      // compute variable information for y; i.e., vy
      // (note that the constant zero times a variable is a constant)
      scalar zero(0.0);
      assert( n_order == 1 );
      for(size_t i = 0; i < nr_left; i++)
      {  for(size_t j = 0; j < nc_right; j++)
         {  bool var = false;
            for(size_t ell = 0; ell < n_middle; ell++)
            {  // left information
               size_t index   = 3 + i * n_middle + ell;
               bool var_left  = vx[index];
               bool nz_left   = var_left |
                         (f_left_[0]( long(i), long(ell) ) != zero);
               // right information
               index          = 3 + n_left + ell * nc_right + j;
               bool var_right = vx[index];
               bool nz_right  = var_right |
                         (f_right_[0]( long(ell), long(j) ) != zero);
               // effect of result
               var |= var_left & nz_right;
               var |= nz_left  & var_right;
            }
            size_t index = i * nc_right + j;
            vy[index]    = var;
         }
      }
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
reverse
=======
{xrst_spell_off}
{xrst_code cpp} */
   // reverse mode routine called by CppAD
   virtual bool reverse(
      // highest order Taylor coefficient that we are computing derivative of
      size_t                     q ,
      // forward mode Taylor coefficients for x variables
      const CppAD::vector<double>&     tx ,
      // forward mode Taylor coefficients for y variables
      const CppAD::vector<double>&     ty ,
      // upon return, derivative of G[ F[ {x_j^k} ] ] w.r.t {x_j^k}
      CppAD::vector<double>&           px ,
      // derivative of G[ {y_i^k} ] w.r.t. {y_i^k}
      const CppAD::vector<double>&     py
   )
   {  size_t n_order  = q + 1;
      size_t nr_left  = size_t( CppAD::Integer( tx[ 0 * n_order + 0 ] ) );
      size_t n_middle = size_t( CppAD::Integer( tx[ 1 * n_order + 0 ] ) );
      size_t nc_right = size_t( CppAD::Integer( tx[ 2 * n_order + 0 ] ) );
# ifndef NDEBUG
      size_t  nx        = 3 + (nr_left + nc_right) * n_middle;
      size_t  ny        = nr_left * nc_right;
# endif
      //
      assert( nx * n_order == tx.size() );
      assert( ny * n_order == ty.size() );
      assert( px.size() == tx.size() );
      assert( py.size() == ty.size() );
      //
      size_t n_left   = nr_left * n_middle;
      size_t n_right  = n_middle * nc_right;
      size_t n_result = nr_left * nc_right;
      assert( 3 + n_left + n_right == nx );
      assert( n_result == ny );
      // -------------------------------------------------------------------
      // make sure f_left_, f_right_ are large enough
      assert( f_left_.size() == f_right_.size() );
      assert( f_left_.size() == f_result_.size() );
      // must have previous run forward with order >= n_order
      assert( f_left_.size() >= n_order );
      // -------------------------------------------------------------------
      // make sure r_left_, r_right_, and r_result_ are large enough
      assert( r_left_.size() == r_right_.size() );
      assert( r_left_.size() == r_result_.size() );
      if( r_left_.size() < n_order )
      {  r_left_.resize(n_order);
         r_right_.resize(n_order);
         r_result_.resize(n_order);
         //
         for(size_t k = 0; k < n_order; k++)
         {  r_left_[k].resize( long(nr_left), long(n_middle) );
            r_right_[k].resize( long(n_middle), long(nc_right) );
            r_result_[k].resize( long(nr_left), long(nc_right) );
         }
      }
      // -------------------------------------------------------------------
      // unpack tx into f_left and f_right
      for(size_t k = 0; k < n_order; k++)
      {  // unpack left values for this order
         for(size_t i = 0; i < n_left; i++)
            f_left_[k].data()[i] = tx[ (3 + i) * n_order + k ];
         //
         // unpack right values for this order
         for(size_t i = 0; i < n_right; i++)
            f_right_[k].data()[i] = tx[ (3 + n_left + i) * n_order + k ];
      }
      // -------------------------------------------------------------------
      // unpack py into r_result_
      for(size_t k = 0; k < n_order; k++)
      {  for(size_t i = 0; i < n_result; i++)
            r_result_[k].data()[i] = py[ i * n_order + k ];
      }
      // -------------------------------------------------------------------
      // initialize r_left_ and r_right_ as zero
      for(size_t k = 0; k < n_order; k++)
      {  r_left_[k]   = matrix::Zero( long(nr_left), long(n_middle) );
         r_right_[k]  = matrix::Zero( long(n_middle), long(nc_right) );
      }
      // -------------------------------------------------------------------
      // matrix reverse mode calculation
      for(size_t k1 = n_order; k1 > 0; k1--)
      {  size_t k = k1 - 1;
         for(size_t ell = 0; ell <= k; ell++)
         {  // nr x nm       = nr x nc      * nc * nm
            r_left_[ell]    += r_result_[k] * f_right_[k-ell].transpose();
            // nm x nc       = nm x nr * nr * nc
            r_right_[k-ell] += f_left_[ell].transpose() * r_result_[k];
         }
      }
      // -------------------------------------------------------------------
      // pack r_left and r_right int px
      for(size_t k = 0; k < n_order; k++)
      {  // dimensions are integer constants
         px[ 0 * n_order + k ] = 0.0;
         px[ 1 * n_order + k ] = 0.0;
         px[ 2 * n_order + k ] = 0.0;
         //
         // pack left values for this order
         for(size_t i = 0; i < n_left; i++)
            px[ (3 + i) * n_order + k ] = r_left_[k].data()[i];
         //
         // pack right values for this order
         for(size_t i = 0; i < n_right; i++)
            px[ (3 + i + n_left) * n_order + k] = r_right_[k].data()[i];
      }
      //
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
for_sparse_jac
==============
{xrst_spell_off}
{xrst_code cpp} */
   // forward Jacobian sparsity routine called by CppAD
   virtual bool for_sparse_jac(
      // number of columns in the matrix R
      size_t                                       q ,
      // sparsity pattern for the matrix R
      const CppAD::vector< std::set<size_t> >&     r ,
      // sparsity pattern for the matrix S = f'(x) * R
      CppAD::vector< std::set<size_t> >&           s ,
      const CppAD::vector<Base>&                   x )
   {
      size_t nr_left  = size_t( CppAD::Integer( x[0] ) );
      size_t n_middle = size_t( CppAD::Integer( x[1] ) );
      size_t nc_right = size_t( CppAD::Integer( x[2] ) );
# ifndef NDEBUG
      size_t  nx        = 3 + (nr_left + nc_right) * n_middle;
      size_t  ny        = nr_left * nc_right;
# endif
      //
      assert( nx == r.size() );
      assert( ny == s.size() );
      //
      size_t n_left = nr_left * n_middle;
      for(size_t i = 0; i < nr_left; i++)
      {  for(size_t j = 0; j < nc_right; j++)
         {  // pack index for entry (i, j) in result
            size_t i_result = i * nc_right + j;
            s[i_result].clear();
            for(size_t ell = 0; ell < n_middle; ell++)
            {  // pack index for entry (i, ell) in left
               size_t i_left  = 3 + i * n_middle + ell;
               // pack index for entry (ell, j) in right
               size_t i_right = 3 + n_left + ell * nc_right + j;
               // check if result of for this product is alwasy zero
               // note that x is nan for commponents that are variables
               bool zero = x[i_left] == Base(0.0) || x[i_right] == Base(0);
               if( ! zero )
               {  s[i_result] =
                     CppAD::set_union(s[i_result], r[i_left] );
                  s[i_result] =
                     CppAD::set_union(s[i_result], r[i_right] );
               }
            }
         }
      }
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
rev_sparse_jac
==============
{xrst_spell_off}
{xrst_code cpp} */
   // reverse Jacobian sparsity routine called by CppAD
   virtual bool rev_sparse_jac(
      // number of columns in the matrix R^T
      size_t                                      q  ,
      // sparsity pattern for the matrix R^T
      const CppAD::vector< std::set<size_t> >&    rt ,
      // sparsity pattern for the matrix S^T = f'(x)^T * R^T
      CppAD::vector< std::set<size_t> >&          st ,
      const CppAD::vector<Base>&                   x )
   {
      size_t nr_left  = size_t( CppAD::Integer( x[0] ) );
      size_t n_middle = size_t( CppAD::Integer( x[1] ) );
      size_t nc_right = size_t( CppAD::Integer( x[2] ) );
      size_t  nx        = 3 + (nr_left + nc_right) * n_middle;
# ifndef NDEBUG
      size_t  ny        = nr_left * nc_right;
# endif
      //
      assert( nx == st.size() );
      assert( ny == rt.size() );
      //
      // initialize S^T as empty
      for(size_t i = 0; i < nx; i++)
         st[i].clear();

      // sparsity for S(x)^T = f'(x)^T * R^T
      size_t n_left = nr_left * n_middle;
      for(size_t i = 0; i < nr_left; i++)
      {  for(size_t j = 0; j < nc_right; j++)
         {  // pack index for entry (i, j) in result
            size_t i_result = i * nc_right + j;
            st[i_result].clear();
            for(size_t ell = 0; ell < n_middle; ell++)
            {  // pack index for entry (i, ell) in left
               size_t i_left  = 3 + i * n_middle + ell;
               // pack index for entry (ell, j) in right
               size_t i_right = 3 + n_left + ell * nc_right + j;
               //
               st[i_left]  = CppAD::set_union(st[i_left],  rt[i_result]);
               st[i_right] = CppAD::set_union(st[i_right], rt[i_result]);
            }
         }
      }
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
for_sparse_hes
==============
{xrst_spell_off}
{xrst_code cpp} */
   virtual bool for_sparse_hes(
      // which components of x are variables for this call
      const CppAD::vector<bool>&                   vx,
      // sparsity pattern for the diagonal of R
      const CppAD::vector<bool>&                   r ,
      // sparsity pattern for the vector S
      const CppAD::vector<bool>&                   s ,
      // sparsity patternfor the Hessian H(x)
      CppAD::vector< std::set<size_t> >&           h ,
      const CppAD::vector<Base>&                   x )
   {
      size_t nr_left  = size_t( CppAD::Integer( x[0] ) );
      size_t n_middle = size_t( CppAD::Integer( x[1] ) );
      size_t nc_right = size_t( CppAD::Integer( x[2] ) );
      size_t  nx        = 3 + (nr_left + nc_right) * n_middle;
# ifndef NDEBUG
      size_t  ny        = nr_left * nc_right;
# endif
      //
      assert( vx.size() == nx );
      assert( r.size()  == nx );
      assert( s.size()  == ny );
      assert( h.size()  == nx );
      //
      // initilize h as empty
      for(size_t i = 0; i < nx; i++)
         h[i].clear();
      //
      size_t n_left = nr_left * n_middle;
      for(size_t i = 0; i < nr_left; i++)
      {  for(size_t j = 0; j < nc_right; j++)
         {  // pack index for entry (i, j) in result
            size_t i_result = i * nc_right + j;
            if( s[i_result] )
            {  for(size_t ell = 0; ell < n_middle; ell++)
               {  // pack index for entry (i, ell) in left
                  size_t i_left  = 3 + i * n_middle + ell;
                  // pack index for entry (ell, j) in right
                  size_t i_right = 3 + n_left + ell * nc_right + j;
                  if( r[i_left] && r[i_right] )
                  {  h[i_left].insert(i_right);
                     h[i_right].insert(i_left);
                  }
               }
            }
         }
      }
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
rev_sparse_hes
==============
{xrst_spell_off}
{xrst_code cpp} */
   // reverse Hessian sparsity routine called by CppAD
   virtual bool rev_sparse_hes(
      // which components of x are variables for this call
      const CppAD::vector<bool>&                   vx,
      // sparsity pattern for S(x) = g'[f(x)]
      const CppAD::vector<bool>&                   s ,
      // sparsity pattern for d/dx g[f(x)] = S(x) * f'(x)
      CppAD::vector<bool>&                         t ,
      // number of columns in R, U(x), and V(x)
      size_t                                       q ,
      // sparsity pattern for R
      const CppAD::vector< std::set<size_t> >&     r ,
      // sparsity pattern for U(x) = g^{(2)} [ f(x) ] * f'(x) * R
      const CppAD::vector< std::set<size_t> >&     u ,
      // sparsity pattern for
      // V(x) = f'(x)^T * U(x) + sum_{i=0}^{m-1} S_i(x) f_i^{(2)} (x) * R
      CppAD::vector< std::set<size_t> >&           v ,
      // parameters as integers
      const CppAD::vector<Base>&                   x )
   {
      size_t nr_left  = size_t( CppAD::Integer( x[0] ) );
      size_t n_middle = size_t( CppAD::Integer( x[1] ) );
      size_t nc_right = size_t( CppAD::Integer( x[2] ) );
      size_t  nx        = 3 + (nr_left + nc_right) * n_middle;
# ifndef NDEBUG
      size_t  ny        = nr_left * nc_right;
# endif
      //
      assert( vx.size() == nx );
      assert( s.size()  == ny );
      assert( t.size()  == nx );
      assert( r.size()  == nx );
      assert( v.size()  == nx );
      //
      // initilaize return sparsity patterns as false
      for(size_t j = 0; j < nx; j++)
      {  t[j] = false;
         v[j].clear();
      }
      //
      size_t n_left = nr_left * n_middle;
      for(size_t i = 0; i < nr_left; i++)
      {  for(size_t j = 0; j < nc_right; j++)
         {  // pack index for entry (i, j) in result
            size_t i_result = i * nc_right + j;
            for(size_t ell = 0; ell < n_middle; ell++)
            {  // pack index for entry (i, ell) in left
               size_t i_left  = 3 + i * n_middle + ell;
               // pack index for entry (ell, j) in right
               size_t i_right = 3 + n_left + ell * nc_right + j;
               //
               // back propagate T(x) = S(x) * f'(x).
               t[i_left]  |= bool( s[i_result] );
               t[i_right] |= bool( s[i_result] );
               //
               // V(x) = f'(x)^T * U(x) +  sum_i S_i(x) * f_i''(x) * R
               // U(x)   = g''[ f(x) ] * f'(x) * R
               // S_i(x) = g_i'[ f(x) ]
               //
               // back propagate f'(x)^T * U(x)
               v[i_left]  = CppAD::set_union(v[i_left],  u[i_result] );
               v[i_right] = CppAD::set_union(v[i_right], u[i_result] );
               //
               // back propagate S_i(x) * f_i''(x) * R
               // (here is where we use vx to check for cross terms)
               if( s[i_result] && vx[i_left] && vx[i_right] )
               {  v[i_left]  = CppAD::set_union(v[i_left],  r[i_right] );
                  v[i_right] = CppAD::set_union(v[i_right], r[i_left]  );
               }
            }
         }
      }
      return true;
   }
/* {xrst_code}
{xrst_spell_on}
End Class Definition
********************
{xrst_spell_off}
{xrst_code cpp} */
}; // End of atomic_eigen_mat_mul class

}  // END_EMPTY_NAMESPACE
/* {xrst_code}
{xrst_spell_on}

{xrst_end atomic_two_eigen_mat_mul.hpp}
*/


# endif
