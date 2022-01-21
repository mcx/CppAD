# ifndef CPPAD_CORE_ATOMIC_FOUR_HES_SPARSITY_HPP
# define CPPAD_CORE_ATOMIC_FOUR_HES_SPARSITY_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/core/atomic/four/devel/hes_sparsity.hpp>
/*
$begin atomic_four_hes_sparsity$$
$spell
    Hessian
    afun
    hes
$$

$section Atomic Function Hessian Sparsity Patterns$$

$head Syntax$$
$icode%ok% = %afun%.hes_sparsity(
    %call_id%, %select_x%, %select_y%, %pattern_out%
)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head Implementation$$
This function must be defined if
$cref/afun/atomic_four_ctor/atomic_user/afun/$$ is
used to define an $cref ADFun$$ object $icode f$$,
and Hessian sparsity patterns are computed for $icode f$$.

$head Base$$
See $cref/Base/atomic_four_call/Base/$$.

$head call_id$$
See $cref/call_id/atomic_four_call/call_id/$$.

$head select_x$$
This argument has size equal to the number of arguments to this
atomic function; i.e. the size of $icode ax$$.
It specifies which domain components are included in
the calculation of $icode pattern_out$$.
If $icode%select_x%[%j%]%$$ is false, then there will be no indices
$icode k$$ such that either of the following hold:
$codei%
    %pattern_out%.row()[%k%] == %j%
    %pattern_out%.col()[%k%] == %j%
%$$.

$head select_y$$
This argument has size equal to the number of results to this
atomic function; i.e. the size of $icode ay$$.
It specifies which range component functions $latex g_i (x)$$ are included in
of $icode pattern_out$$.

$head pattern_out$$
This input value of $icode pattern_out$$ does not matter.
Upon return it is the union,
with respect to $icode i$$ such that $icode%select_y%[%i%]%$$ is true,
of the sparsity pattern for Hessian of $latex g_i (x)$$.
To be specific, there are non-negative indices
$icode r$$, $icode c$$, and $icode k$$ such that
$codei%
    %pattern_out%.row()[%k%] == %r%
    %pattern_out%.col()[%k%] == %c%
%$$
if and only if
there exists an index $icode i$$ such that,
$icode%select_y%[%i%]%$$ is true,
$icode%select_x%[%r%]%$$ is true,
$icode%select_x%[%c%]%$$ is true,
and
$latex \[
    \partial_{x(r)} \partial_{x(c)} g_i(x)
\] $$
is possibly non-zero.
Note that the sparsity pattern should be symmetric.

$head ok$$
If this calculation succeeded, $icode ok$$ is true.
Otherwise it is false.

$comment%
    example/atomic_four/hes_sparsity.cpp
%$$
$head Examples$$
The file $comment atomic_four_hes_sparsity.cpp$$ contains an example and test
that uses this routine.
$end
-----------------------------------------------------------------------------
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// BEGIN_PROTOTYPE
template <class Base>
bool atomic_four<Base>::hes_sparsity(
    size_t                                  call_id      ,
    const vector<bool>&                     select_x     ,
    const vector<bool>&                     select_y     ,
    sparse_rc< vector<size_t> >&            pattern_out  )
// END_PROTOTYPE
{   return false; }

} // END_CPPAD_NAMESPACE

# endif