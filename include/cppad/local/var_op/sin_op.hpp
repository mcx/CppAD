# ifndef CPPAD_LOCAL_VAR_OP_SIN_OP_HPP
# define CPPAD_LOCAL_VAR_OP_SIN_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------


namespace CppAD { namespace local { namespace var_op {


// See dev documentation: forward_unary_op
template <class Base>
inline void sin_forward_any(
   size_t        order_low   ,
   size_t        order_up    ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        cap_order   ,
   Base*         taylor      )
{   // p, q
   size_t p = order_low;
   size_t q = order_up;
   //
   // i_x
   size_t i_x = size_t(arg[0]);
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SinOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SinOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );

   // Taylor coefficients corresponding to argument and result
   Base* x = taylor + i_x * cap_order;
   Base* s = taylor + i_z * cap_order;
   Base* c = s      -       cap_order;

   // rest of this routine is identical for the following cases:
   // forward_sin_op, forward_cos_op, forward_sinh_op, forward_cosh_op.
   // (except that there is a sign difference for the hyperbolic case).
   size_t k;
   if( p == 0 )
   {  s[0] = sin( x[0] );
      c[0] = cos( x[0] );
      p++;
   }
   for(size_t j = p; j <= q; j++)
   {
      s[j] = Base(0.0);
      c[j] = Base(0.0);
      for(k = 1; k <= j; k++)
      {  s[j] += Base(double(k)) * x[k] * c[j-k];
         c[j] -= Base(double(k)) * x[k] * s[j-k];
      }
      s[j] /= Base(double(j));
      c[j] /= Base(double(j));
   }
}
// See dev documentation: forward_unary_op
template <class Base>
inline void sin_forward_dir(
   size_t        order_up    ,
   size_t        n_dir       ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        cap_order   ,
   Base*         taylor      )
{   // q, r
   size_t q = order_up;
   size_t r = n_dir;
   //
   // i_x
   size_t i_x = size_t(arg[0]);
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SinOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SinOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );

   // Taylor coefficients corresponding to argument and result
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   Base* x = taylor + i_x * num_taylor_per_var;
   Base* s = taylor + i_z * num_taylor_per_var;
   Base* c = s      -       num_taylor_per_var;


   // rest of this routine is identical for the following cases:
   // forward_sin_op, forward_cos_op, forward_sinh_op, forward_cosh_op
   // (except that there is a sign difference for the hyperbolic case).
   size_t m = (q-1) * r + 1;
   for(size_t ell = 0; ell < r; ell++)
   {  s[m+ell] =   Base(double(q)) * x[m + ell] * c[0];
      c[m+ell] = - Base(double(q)) * x[m + ell] * s[0];
      for(size_t k = 1; k < q; k++)
      {  s[m+ell] += Base(double(k)) * x[(k-1)*r+1+ell] * c[(q-k-1)*r+1+ell];
         c[m+ell] -= Base(double(k)) * x[(k-1)*r+1+ell] * s[(q-k-1)*r+1+ell];
      }
      s[m+ell] /= Base(double(q));
      c[m+ell] /= Base(double(q));
   }
}


// See dev documentation: forward_unary_op
template <class Base>
inline void sin_forward_0(
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        cap_order   ,
   Base*         taylor      )
{  //
   //
   // i_x
   size_t i_x = size_t(arg[0]);
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SinOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SinOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( 0 < cap_order );

   // Taylor coefficients corresponding to argument and result
   Base* x = taylor + i_x * cap_order;
   Base* s = taylor + i_z * cap_order;  // called z in documentation
   Base* c = s      -       cap_order;  // called y in documentation

   s[0] = sin( x[0] );
   c[0] = cos( x[0] );
}


// See dev documentation: reverse_unary_op
template <class Base>
inline void sin_reverse(
   size_t        i_z          ,
   const addr_t* arg          ,
   size_t        cap_order    ,
   const Base*   taylor       ,
   size_t        n_order      ,
   Base*         partial      )
{  // d  //
   //
   // i_x
   size_t i_x = size_t(arg[0]);
   //
   size_t d = n_order - 1;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SinOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SinOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( n_order <= cap_order );

   // Taylor coefficients and partials corresponding to argument
   const Base* x  = taylor  + i_x * cap_order;
   Base* px       = partial + i_x * n_order;

   // Taylor coefficients and partials corresponding to first result
   const Base* s  = taylor  + i_z * cap_order; // called z in doc
   Base* ps       = partial + i_z * n_order;

   // Taylor coefficients and partials corresponding to auxiliary result
   const Base* c  = s  - cap_order; // called y in documentation
   Base* pc       = ps - n_order;


   // rest of this routine is identical for the following cases:
   // reverse_sin_op, reverse_cos_op, reverse_sinh_op, reverse_cosh_op.
   size_t j = d;
   size_t k;
   while(j)
   {
      ps[j]   /= Base(double(j));
      pc[j]   /= Base(double(j));
      for(k = 1; k <= j; k++)
      {
         px[k]   += Base(double(k)) * azmul(ps[j], c[j-k]);
         px[k]   -= Base(double(k)) * azmul(pc[j], s[j-k]);

         ps[j-k] -= Base(double(k)) * azmul(pc[j], x[k]);
         pc[j-k] += Base(double(k)) * azmul(ps[j], x[k]);

      }
      --j;
   }
   px[0] += azmul(ps[0], c[0]);
   px[0] -= azmul(pc[0], s[0]);
}

} } } // END namespace
# endif
