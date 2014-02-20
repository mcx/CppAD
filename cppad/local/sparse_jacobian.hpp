/* $Id$ */
# ifndef CPPAD_SPARSE_JACOBIAN_INCLUDED
# define CPPAD_SPARSE_JACOBIAN_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin sparse_jacobian$$
$spell
	recomputed
	valarray
	std
	CppAD
	Bool
	jac
	Jacobian
	const
	Taylor
$$

$section Sparse Jacobian: Easy Driver$$
$index SparseJacobian$$
$index jacobian, sparse$$

$head Syntax$$
$icode%jac% = %f%.SparseJacobian(%x%)
%jac% = %f%.SparseJacobian(%x%, %p%)
%n_sweep% = %f%.SparseJacobianForward(%x%, %p%, %row%, %col%, %jac%, %work%)
%n_sweep% = %f%.SparseJacobianReverse(%x%, %p%, %row%, %col%, %jac%, %work%)
%$$

$head Purpose$$
We use $latex n$$ for the $cref/domain/seq_property/Domain/$$ size,
and $latex m$$ for the $cref/range/seq_property/Range/$$ size of $icode f$$.
We use $latex F : \B{R}^n \rightarrow \B{R}^m$$ do denote the
$cref/AD function/glossary/AD Function/$$
corresponding to $icode f$$. 
The syntax above sets $icode jac$$ to the Jacobian 
$latex \[
	jac = F^{(1)} (x) 
\] $$
This routine takes advantage of the sparsity of the Jacobian
in order to reduce the amount of computation necessary.
If $icode row$$ and $icode col$$ are present, it also takes
advantage of the reduced set of elements of the Jacobian that
need to be computed.
One can use speed tests (e.g. $cref speed_test$$)
to verify that results are computed faster
than when using the routine $cref Jacobian$$.

$head f$$
The object $icode f$$ has prototype
$codei%
	ADFun<%Base%> %f%
%$$
Note that the $cref ADFun$$ object $icode f$$ is not $code const$$
(see $cref/Uses Forward/sparse_jacobian/Uses Forward/$$ below).

$head x$$
The argument $icode x$$ has prototype
$codei%
	const %VectorBase%& %x%
%$$
(see $cref/VectorBase/sparse_jacobian/VectorBase/$$ below)
and its size 
must be equal to $icode n$$, the dimension of the
$cref/domain/seq_property/Domain/$$ space for $icode f$$.
It specifies
that point at which to evaluate the Jacobian.

$head p$$
The argument $icode p$$ is optional and has prototype
$codei%
	const %VectorSet%& %p%
%$$
(see $cref/VectorSet/sparse_jacobian/VectorSet/$$ below).
If it has elements of type $code bool$$,
its size is $latex m * n$$.
If it has elements of type $code std::set<size_t>$$,
its size is $latex m$$ and all its set elements are between
zero and $latex n - 1$$.
It specifies a 
$cref/sparsity pattern/glossary/Sparsity Pattern/$$ 
for the Jacobian $latex F^{(1)} (x)$$.
$pre

$$
If this sparsity pattern does not change between calls to 
$codei SparseJacobian$$, it should be faster to calculate $icode p$$ once 
(using $cref ForSparseJac$$ or $cref RevSparseJac$$)
and then pass $icode p$$ to $codei SparseJacobian$$.
In addition,
if you specify $icode p$$, CppAD will use the same
type of sparsity representation 
(vectors of $code bool$$ or vectors of $code std::set<size_t>$$)
for its internal calculations.
Otherwise, the representation
for the internal calculations is unspecified.

$head row, col$$
The arguments $icode row$$ and $icode col$$ are optional and have prototype
$codei%
	const %VectorSize%& %row%
	const %VectorSize%& %col%
%$$
(see $cref/VectorSize/sparse_jacobian/VectorSize/$$ below).
They specify which rows and columns of $latex F^{(1)} (x)$$ are
computes and in what order.
We use $latex K$$ to denote the value $icode%jac%.size()%$$
which must also equal the size of $icode row$$ and $icode col$$.
Furthermore,
for $latex k = 0 , \ldots , K-1$$, it must hold that
$latex row[k] < m$$ and $latex col[k] < n$$.
All of the $latex (row[k], col[k])$$ pairs must correspond to an entry
in the sparsity pattern $icode p$$,
but not all such entries need be included
in the $latex (row[k], col[k])$$ pairs. 

$head jac$$
The result $icode jac$$ has prototype
$codei%
	%VectorBase%& %jac%
%$$
In the case where the arguments $icode row$$ and $icode col$$ are not present,
the size of $icode jac$$ is $latex m * n$$ and
for $latex i = 0 , \ldots , m-1$$,
$latex j = 0 , \ldots , n-1$$,
$latex \[
	jac [ i * n + j ] = \D{ F_i }{ x_j } (x)
\] $$
$pre

$$
In the case where the arguments $icode row$$ and $icode col$$ are present,
we use $latex K$$ to denote the size of $icode jac$$. 
The input value of its elements does not matter.
Upon return, for $latex k = 0 , \ldots , K - 1$$,
$latex \[
	jac [ k ] = \D{ F_i }{ x_j } (x)
	\; , \;
	\; {\rm where} \;
	i = row[k]
	\; {\rm and } \;
	j = col[k]
\] $$

$head work$$
If this argument is present, it has prototype
$codei%
	sparse_jacobian_work& %work%
%$$
This object can only be used with the routines 
$code SparseJacobianForward$$ and $code SparseJacobianReverse$$.
During its the first use, information is stored in $icode work$$.
This is used to reduce the work done by future calls to the same mode
(forward or reverse), 
the same $icode f$$, $icode p$$, $icode row$$, and $icode col$$.
If a future call is for a different mode,
or any of these values have changed,
you must first call $icode%work%.clear()%$$
to inform CppAD that this information needs to be recomputed.

$head n_sweep$$
The return value $icode n_sweep$$ has prototype
$codei%
	size_t %n_sweep%
%$$
If $code SparseJacobianForward$$ ($code SparseJacobianReverse$$) is used, 
$icode n_sweep$$ is the number of first order forward (reverse) sweeps 
used to compute the requested Jacobian values. 
This is proportional to the total work that $code SparseJacobian$$ does, 
not counting the zero order forward sweep, 
or the work to combine multiple columns (rows) into a single sweep.

$head VectorBase$$
The type $icode VectorBase$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$icode Base$$.
The routine $cref CheckSimpleVector$$ will generate an error message
if this is not the case.

$head VectorSet$$
The type $icode VectorSet$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$code bool$$ or $code std::set<size_t>$$;
see $cref/sparsity pattern/glossary/Sparsity Pattern/$$ for a discussion
of the difference.
The routine $cref CheckSimpleVector$$ will generate an error message
if this is not the case.

$subhead Restrictions$$
If $icode VectorSet$$ has elements of $code std::set<size_t>$$,
then $icode%p%[%i%]%$$ must return a reference (not a copy) to the 
corresponding set.
According to section 26.3.2.3 of the 1998 C++ standard,
$code std::valarray< std::set<size_t> >$$ does not satisfy
this condition. 

$head VectorSize$$
The type $icode VectorSize$$ must be a $cref SimpleVector$$ class with
$cref/elements of type/SimpleVector/Elements of Specified Type/$$
$code size_t$$.
The routine $cref CheckSimpleVector$$ will generate an error message
if this is not the case.

$head Uses Forward$$
After each call to $cref Forward$$,
the object $icode f$$ contains the corresponding 
$cref/Taylor coefficients/glossary/Taylor Coefficient/$$.
After a call to any of the sparse Jacobian routines,
the zero order Taylor coefficients correspond to
$icode%f%.Forward(0, %x%)%$$
and the other coefficients are unspecified.

After $code SparseJacobian$$,
the previous calls to $cref Forward$$ are undefined.

$head Example$$
$children%
	example/sparse_jacobian.cpp
%$$
The routine
$cref sparse_jacobian.cpp$$
is examples and tests of $code sparse_jacobian$$.
It return $code true$$, if it succeeds and $code false$$ otherwise.

$end
==============================================================================
*/
# include <cppad/local/std_set.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\defgroup sparse_jacobian_hpp sparse_jacobian.hpp
\{
\file sparse_jacobian.hpp
Sparse Jacobian driver routine and helper functions.
*/
// ===========================================================================
/*!
class used by SparseJacobian to hold information so it does not need to be 
recomputed.
*/
class sparse_jacobian_work {
	public:
		/// version of user row array with the extra value m at end
		CppAD::vector<size_t> user_row;
		/// version of user col array with the extra value n at end
		CppAD::vector<size_t> user_col;
		/// indices that sort the user arrays by row 
		/// with the extra value K at the end
		CppAD::vector<size_t> sort_row;
		/// indices that sort the user arrays by column 
		/// with the extra value K at the end
		CppAD::vector<size_t> sort_col;
		/// results of the coloring algorithm
		CppAD::vector<size_t> color;
		/// inform CppAD that this information needs to be recomputed
		void clear(void)
		{	user_row.clear();
			user_col.clear();
			sort_row.clear();
			sort_col.clear();
			color.clear();
		}
};
// ===========================================================================
/*!
Private helper function forward mode cases

\tparam Base
is the base type for the recording that is stored in this
<code>ADFun<Base></code> object.

\tparam VectorBase
is a simple vector class with elements of type \a Base.

\tparam VectorSet
is either \c sparse_pack, \c sparse_set or \c sparse_list.

\param x [in]
is a vector specifing the point at which to compute the Jacobian.

\param p_transpose [in]
If <code>work.color.size() != 0</code>, 
then \c p_transpose is not used.
Otherwise, it is a
sparsity pattern for the transpose of the Jacobian of this ADFun<Base> object.
Note that we do not change the values in \c p_transpose,
but is not \c const because we use its iterator facility.

\param row [in]
is the vector of row indices for the returned Jacobian values.

\param col [in]
is the vector of columns indices for the returned Jacobian values.
It must have the same size as \c row.

\param jac [out]
is the vector of Jacobian values. We use \c K to denote the size of \c jac.
The return value <code>jac[k]</code> is the partial of the
<code>work.user_row[k]</code> range component of the function with respect
the the <code>work.user_col[k]</code> domain component of its argument.

\param work
This structure contains information that is computed by \c SparseJacobainFor.
If the sparsity pattern, \c user_row vector, or \c user_col vectors
are not the same between calls to \c SparseJacobianFor, 
\c work.clear() must be called to reinitialize \c work.

\return
Is the number of first order forward sweeps used to compute the
requested Jacobian values. The total work, not counting the zero order
forward sweep, or the time to combine computations, is proportional to this
return value.
*/
template<class Base>
template <class VectorBase, class VectorSet, class VectorSize>
size_t ADFun<Base>::SparseJacobianFor(
	const VectorBase&            x           ,
	      VectorSet&             p_transpose ,
	const VectorSize&            row         ,
	const VectorSize&            col         ,
	      VectorBase&            jac         ,
	       sparse_jacobian_work& work        )
{
	size_t i, j, k, ell;

	CppAD::vector<size_t>& sort_col(work.sort_col);
	CppAD::vector<size_t>& color(work.color);

	size_t m = Range();
	size_t n = Domain();

	// some values
	const Base zero(0);
	const Base one(1);

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CPPAD_ASSERT_UNKNOWN( size_t(x.size()) == n );
	CPPAD_ASSERT_UNKNOWN( color.size() == 0 || color.size() == n );

	// number of components of Jacobian that are required
	size_t K = size_t(jac.size());
	CPPAD_ASSERT_UNKNOWN( row.size() == K );
	CPPAD_ASSERT_UNKNOWN( col.size() == K );

	// Point at which we are evaluating the Jacobian
	Forward(0, x);

	if( color.size() == 0 )
	{
		CPPAD_ASSERT_UNKNOWN( p_transpose.n_set() ==  n );
		CPPAD_ASSERT_UNKNOWN( p_transpose.end() ==  m );

		// put sorting indices in sort_col
		work.sort_col.resize(K);
		index_sort(col, work.sort_col);

		// initialize columns that are in the returned jacobian
		CppAD::vector<bool> col_used(n);
		for(j = 0; j < n; j++)
				col_used[j] = false;

		// rows and columns that are in the returned jacobian
		VectorSet c2r_used, r2c_used;
		c2r_used.resize(n, m);
		r2c_used.resize(m, n);
		k = 0;
		while( k < K )
		{	CPPAD_ASSERT_UNKNOWN( 
				row[sort_col[k]] < m && col[sort_col[k]] < n 
			);
			CPPAD_ASSERT_UNKNOWN( 
				k == 0 || col[sort_col[k-1]] <= col[sort_col[k]] 
			);
			CPPAD_ASSERT_KNOWN(
				p_transpose.is_element(col[sort_col[k]], row[sort_col[k]]) ,
				"SparseJacobianForward: "
				"an (row, col) pair is not in sparsity pattern."
			);
			col_used[ col[k] ] = true;
			c2r_used.add_element(col[k], row[k]);
			r2c_used.add_element(row[k], col[k]);
			k++;
		}
	
		// given a row index, which columns are non-zero and not used
		VectorSet not_used;
		not_used.resize(m, n);
		for(j = 0; j < n; j++)
		{	p_transpose.begin(j);
			i = p_transpose.next_element();
			while( i != p_transpose.end() )
			{	if( ! r2c_used.is_element(i, j) )
					not_used.add_element(i, j);
				i = p_transpose.next_element();
			}
		}

		// initial coloring
		color.resize(n);
		ell = 0;
		for(j = 0; j < n; j++)
			if( col_used[j] )	
				color[j] = ell++;
			else	color[j] = n;
	
		// See GreedyPartialD2Coloring Algorithm Section 3.6.2 of
		// Graph Coloring in Optimization Revisited by
		// Assefaw Gebremedhin, Fredrik Maane, Alex Pothen
		CppAD::vector<bool> forbidden(n);
		for(j = 1; j < n; j++) if( color[j] < n )
		{
			// initial all colors as ok for this column
			// (value of forbidden for ell > j does not matter)
			for(ell = 0; ell <= color[j]; ell++)
				forbidden[ell] = false;
	
			// Forbid colors for which this row would destroy results:
			//
			// for each row that is non-zero for this column
			p_transpose.begin(j);
			i = p_transpose.next_element();
			while( i != p_transpose.end() )
			{	// for each column that this used with this row
				r2c_used.begin(i);
				ell = r2c_used.next_element();
				while( ell != r2c_used.end() )
				{	// if this is not the same column, forbid its color
					if( (ell < j) & (color[ell] < n) )
						forbidden[ color[ell] ] = true;
					ell = r2c_used.next_element();
				}
				i = p_transpose.next_element();
			}
	
			// Forbid colors that destroy results needed for this row
			//
			// for each row that this column uses
			c2r_used.begin(j);
			i = c2r_used.next_element();
			while( i != c2r_used.end() )
			{	// For each column that is non-zero for this row
				// (the used columns have already been checked above).
				not_used.begin(i);
				ell = not_used.next_element();
				while( ell != not_used.end() )
				{	// if this is not the same column, forbid its color
					if( (ell < j) & (color[ell] < n)  )
						forbidden[ color[ell] ] = true;
					ell = not_used.next_element();
				}
				i = c2r_used.next_element();
			}

			// pick the color with smallest index
			ell = 0;
			while( forbidden[ell] )
			{	ell++;
				CPPAD_ASSERT_UNKNOWN( ell <= color[j] );
			}
			color[j] = ell;
		}
	}
	size_t n_color = 1;
	for(j = 0; j < n; j++) if( color[j] < n )
		n_color = std::max(n_color, color[j] + 1);

	// direction vector for calls to forward
	VectorBase dx(n);

	// location for return values from forward
	VectorBase dy(m);

	// initialize the return value
	for(k = 0; k < K; k++)
		jac[k] = zero;

	// loop over colors
	size_t n_sweep = 0;
	for(ell = 0; ell < n_color; ell++)
	{	n_sweep++;

		// combine all columns with this color
		for(j = 0; j < n; j++)
		{	dx[j] = zero;
			if( color[j] == ell )
				dx[j] = one;
		}
		// call forward mode for all these columns at once
		dy = Forward(1, dx);

		// set the corresponding components of the result
		k = 0;
		for(j = 0; j < n; j++) if( color[j] == ell )
		{	// find first k index for this column
			while( k < K && col[sort_col[k]] < j )
				k++;
			// extract the row results for this column
			while( k < K && col[sort_col[k]] == j ) 
			{	jac[ sort_col[k] ] = dy[row[sort_col[k]]];
				k++;
			}
		}
	}
	return n_sweep;
}
/*!
Private helper function for reverse mode cases.

\tparam Base
is the base type for the recording that is stored in this
<code>ADFun<Base></code> object.

\tparam VectorBase
is a simple vector class with elements of type \a Base.

\tparam VectorSet
is either \c sparse_pack, \c sparse_set or \c sparse_list.

\param x [in]
is a vector specifing the point at which to compute the Jacobian.

\param p [in]
If <code>work.color.size() != 0</code>, then \c p is not used.
Otherwise, it is a
sparsity pattern for the Jacobian of this ADFun<Base> object.
Note that we do not change the values in \c p,
but is not \c const because we use its iterator facility.

\param row [in]
is the vector of row indices for the returned Jacobian values.

\param col [in]
is the vector of columns indices for the returned Jacobian values.
It must have the same size as \c row.

\param jac [out]
is the vector of Jacobian values.
It must have the same size as \c row. 
The return value <code>jac[k]</code> is the partial of the
<code>work.user_row[k]</code> range component of the function with respect
the the <code>work.user_col[k]</code> domain component of its argument.

\param work
This structure contains information that is computed by \c SparseJacobainFor.
If the sparsity pattern, \c user_row vector, or \c user_col vectors
are not the same between calls to \c SparseJacobianFor, 
\c work.clear() must be called to reinitialize \c work.

\return
Is the number of first order reverse sweeps used to compute the
reverse Jacobian values. The total work, not counting the zero order
forward sweep, or the time to combine computations, is proportional to this
return value.
*/
template<class Base>
template <class VectorBase, class VectorSet, class VectorSize>
size_t ADFun<Base>::SparseJacobianRev(
	const VectorBase&           x           ,
	      VectorSet&            p           ,
	const VectorSize&           row         ,
	const VectorSize&           col         ,
	      VectorBase&           jac         ,
	      sparse_jacobian_work& work        )
{
	size_t i, j, k, ell;

	CppAD::vector<size_t>& sort_row(work.sort_row);
	CppAD::vector<size_t>& color(work.color);

	size_t m = Range();
	size_t n = Domain();

	// some values
	const Base zero(0);
	const Base one(1);

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CPPAD_ASSERT_UNKNOWN( size_t(x.size()) == n );
	CPPAD_ASSERT_UNKNOWN (color.size() == m || color.size() == 0 );

	// number of components of Jacobian that are required
	size_t K = size_t(jac.size());
	CPPAD_ASSERT_UNKNOWN( row.size() == K );
	CPPAD_ASSERT_UNKNOWN( col.size() == K );

	// Point at which we are evaluating the Jacobian
	Forward(0, x);

	if( color.size() == 0 )
	{	CPPAD_ASSERT_UNKNOWN( p.n_set() ==  m );
		CPPAD_ASSERT_UNKNOWN( p.end() ==  n );

		// put sorting indices in sort_row
		work.sort_row.resize(K);
		index_sort(row, work.sort_row);

		// initialize rows that are in the returned jacobian
		CppAD::vector<bool> row_used(m);
		for(i = 0; i < m; i++)
				row_used[i] = false;

		// rows and columns that are in the returned jacobian
		VectorSet c2r_used, r2c_used;
		c2r_used.resize(n, m);
		r2c_used.resize(m, n);
		k = 0;
		while( k < K )
		{	CPPAD_ASSERT_UNKNOWN( 
				row[sort_row[k]] < m && col[sort_row[k]] < n 
			);
			CPPAD_ASSERT_UNKNOWN( 
				k == 0 || row[sort_row[k-1]] <= row[sort_row[k]] 
			);
			CPPAD_ASSERT_KNOWN(
				p.is_element(row[sort_row[k]], col[sort_row[k]]) ,
				"SparseJacobianReverse: "
				"an (row, col) pair is not in sparsity pattern."
			);
			row_used[ row[k] ] = true;
			c2r_used.add_element(col[k], row[k]);
			r2c_used.add_element(row[k], col[k]);
			k++;
		}
	
		// given a column index, which rows are non-zero and not used
		VectorSet not_used;
		not_used.resize(n, m);
		for(i = 0; i < m; i++)
		{	p.begin(i);
			j = p.next_element();
			while( j != p.end() )
			{	if( ! c2r_used.is_element(j , i) )
					not_used.add_element(j, i);
				j = p.next_element();
			}
		}
	
		// initial coloring
		color.resize(m);
		ell = 0;
		for(i = 0; i < m; i++)
			if( row_used[i] )
				color[i] = ell++;
			else	color[i] = m;
	
		// See GreedyPartialD2Coloring Algorithm Section 3.6.2 of
		// Graph Coloring in Optimization Revisited by
		// Assefaw Gebremedhin, Fredrik Maane, Alex Pothen
		CppAD::vector<bool> forbidden(m);
		for(i = 1; i < m; i++) if( color[i] < m )
		{
			// initial all colors as ok for this row
			// (value of forbidden for ell > i does not matter)
			for(ell = 0; ell <= color[i]; ell++)
				forbidden[ell] = false;
	
			// -----------------------------------------------------
			// Forbid colors for which this row would destroy results:
			//
			// for each column that is non-zero for this row
			p.begin(i);
			j = p.next_element();
			while( j != p.end() )
			{	// for each row that is used with this column uses
				c2r_used.begin(j);
				ell = c2r_used.next_element();
				while( ell != c2r_used.end() )
				{	// if this is not the same row, forbid its color 
					if( (ell < i) & (color[ell] < m) )
						forbidden[ color[ell] ] = true;
					ell = c2r_used.next_element();
				}
				j = p.next_element();
			}

	
			// -----------------------------------------------------
			// Forbid colors that destroy results needed for this row.
			//
			// for each column that this row uses
			r2c_used.begin(i);
			j = r2c_used.next_element();
			while( j != r2c_used.end() )
			{	// For each row that is non-zero for this column
				// (the used rows have already been checked above).
				not_used.begin(j);
				ell = not_used.next_element();
				while( ell != not_used.end() )
				{	// if this is not the same row, forbid its color 
					if( (ell < i) & (color[ell] < m) )
						forbidden[ color[ell] ] = true;
					ell = not_used.next_element();
				}
				j = r2c_used.next_element();
			}

			// pick the color with smallest index
			ell = 0;
			while( forbidden[ell] )
			{	ell++;
				CPPAD_ASSERT_UNKNOWN( ell <= color[i] );
			}
			color[i] = ell;
		}
	}
	size_t n_color = 1;
	for(i = 0; i < m; i++) if( color[i] < m ) 
		n_color = std::max(n_color, color[i] + 1);

	// weighting vector for calls to reverse
	VectorBase w(m);

	// location for return values from Reverse
	VectorBase dw(n);

	// initialize the return value
	for(k = 0; k < K; k++)
		jac[k] = zero;

	// loop over colors
	size_t n_sweep = 0;
	for(ell = 0; ell < n_color; ell++)
	{	n_sweep++;

		// combine all the rows with this color
		for(i = 0; i < m; i++)
		{	w[i] = zero;
			if( color[i] == ell )
				w[i] = one;
		}
		// call reverse mode for all these rows at once
		dw = Reverse(1, w);

		// set the corresponding components of the result
		k = 0;
		for(i = 0; i < m; i++) if( color[i] == ell )
		{	// find first k index for this row
			while( k < K && row[sort_row[k]] < i )
				k++;
			// extract the row results for this row
			while( k < K && row[sort_row[k]] == i ) 
			{	jac[ sort_row[k] ] = dw[col[sort_row[k]]];
				k++;
			}
		}
	}
	return n_sweep;
}
// ==========================================================================
// Public Member functions
// ==========================================================================
/*!
Compute user specified subset of a sparse Jacobian using forward mode.

The C++ source code corresponding to this operation is
\verbatim
	SparceJacobianForward(x, p, row, col, jac, work)
\endverbatim

\tparam Base
is the base type for the recording that is stored in this
<code>ADFun<Base></code> object.

\tparam VectorBase
is a simple vector class with elements of type \a Base.

\tparam VectorSet
is a simple vector class with elements of type 
\c bool or \c std::set<size_t>.

\tparam VectorSize
is a simple vector class with elements of type \c size_t.

\param x [in]
is a vector specifing the point at which to compute the Jacobian.

\param p [in]
is the sparsity pattern for the Jacobian that we are calculating.

\param row [in]
is the vector of row indices for the returned Jacobian values.

\param col [in]
is the vector of columns indices for the returned Jacobian values.
It must have the same size as \c row.

\param jac [out]
is the vector of Jacobian values.
It must have the same size as \c row. 
The return value <code>jac[k]</code> is the partial of the
<code>row[k]</code> range component of the function with respect
the the <code>col[k]</code> domain component of its argument.

\param work [in,out]
\c work contains information that depends on the function object, 
sparsity pattern, \c row vector, and \c col vector.
If they are not the same between calls to \c SparseJacobianForward, 
\c work.clear() must be called to reinitialize them.

\return
Is the number of first order forward sweeps used to compute the
requested Jacobian values. The total work, not counting the zero order
forward sweep, or the time to combine computations, is proportional to this
return value.
*/
template<class Base>
template <class VectorBase, class VectorSet, class VectorSize>
size_t ADFun<Base>::SparseJacobianForward(
	const VectorBase&     x    ,
	const VectorSet&      p    ,
	const VectorSize&     row  ,
	const VectorSize&     col  ,
	VectorBase&           jac  ,
	sparse_jacobian_work& work )
{
	size_t n = Domain();
	size_t m = Range();
# ifndef NDEBUG
	size_t k, K = jac.size();
	CPPAD_ASSERT_KNOWN(
		size_t(x.size()) == n ,
		"SparseJacobianForward: size of x not equal domain dimension for f."
	); 
	CPPAD_ASSERT_KNOWN(
		size_t(row.size()) == K && size_t(col.size()) == K ,
		"SparseJacobianForward: either r or c does not have "
		"the same size as jac."
	); 
	CPPAD_ASSERT_KNOWN(
		work.color.size() == 0 || work.color.size() == n,
		"SparseJacobianForward: invalid value in work."
	);
	for(k = 0; k < K; k++)
	{	CPPAD_ASSERT_KNOWN(
			row[k] < m,
			"SparseJacobianForward: invalid value in r."
		);
		CPPAD_ASSERT_KNOWN(
			col[k] < n,
			"SparseJacobianForward: invalid value in c."
		);
	}
	if( work.color.size() != 0 )
		for(size_t j = 0; j < n; j++) CPPAD_ASSERT_KNOWN(
			work.color[j] <= n,
			"SparseJacobianForward: invalid value in work."
	);
# endif

	typedef typename VectorSet::value_type Set_type;
	typedef typename internal_sparsity<Set_type>::pattern_type Pattern_type;
	Pattern_type s_transpose;
	if( work.color.size() == 0 )
	{	bool transpose = true;
		sparsity_user2internal(s_transpose, p, m, n, transpose);
	}
	size_t n_sweep = SparseJacobianFor(x, s_transpose, row, col, jac, work);
	return n_sweep;
}
/*!
Compute user specified subset of a sparse Jacobian using forward mode.

The C++ source code corresponding to this operation is
\verbatim
	SparceJacobianReverse(x, p, row, col, jac, work)
\endverbatim

\tparam Base
is the base type for the recording that is stored in this
<code>ADFun<Base></code> object.

\tparam VectorBase
is a simple vector class with elements of type \a Base.

\tparam VectorSet
is a simple vector class with elements of type 
\c bool or \c std::set<size_t>.

\tparam VectorSize
is a simple vector class with elements of type \c size_t.

\param x [in]
is a vector specifing the point at which to compute the Jacobian.

\param p [in]
is the sparsity pattern for the Jacobian that we are calculating.

\param row [in]
is the vector of row indices for the returned Jacobian values.

\param col [in]
is the vector of columns indices for the returned Jacobian values.
It must have the same size as \c row.

\param jac [out]
is the vector of Jacobian values.
It must have the same size as \c row. 
The return value <code>jac[k]</code> is the partial of the
<code>row[k]</code> range component of the function with respect
the the <code>col[k]</code> domain component of its argument.

\param work [in,out]
\c work contains information that depends on the function object, 
sparsity pattern, \c row vector, and \c col vector.
If they are not the same between calls to \c SparseJacobianReverse, 
\c work.clear() must be called to reinitialize them.

\return
Is the number of first order reverse sweeps used to compute the
reverse Jacobian values. The total work, not counting the zero order
forward sweep, or the time to combine computations, is proportional to this
return value.
*/
template<class Base>
template <class VectorBase, class VectorSet, class VectorSize>
size_t ADFun<Base>::SparseJacobianReverse(
	const VectorBase&     x    ,
	const VectorSet&      p    ,
	const VectorSize&     row  ,
	const VectorSize&     col  ,
	VectorBase&           jac  ,
	sparse_jacobian_work& work )
{
	size_t m = Range();
	size_t n = Domain();
# ifndef NDEBUG
	size_t k, K = jac.size();
	CPPAD_ASSERT_KNOWN(
		size_t(x.size()) == n ,
		"SparseJacobianReverse: size of x not equal domain dimension for f."
	); 
	CPPAD_ASSERT_KNOWN(
		size_t(row.size()) == K && size_t(col.size()) == K ,
		"SparseJacobianReverse: either r or c does not have "
		"the same size as jac."
	); 
	CPPAD_ASSERT_KNOWN(
		work.color.size() == 0 || work.color.size() == m,
		"SparseJacobianReverse: invalid value in work."
	);
	for(k = 0; k < K; k++)
	{	CPPAD_ASSERT_KNOWN(
			row[k] < m,
			"SparseJacobianReverse: invalid value in r."
		);
		CPPAD_ASSERT_KNOWN(
			col[k] < n,
			"SparseJacobianReverse: invalid value in c."
		);
	}
	if( work.color.size() != 0 )
		for(size_t i = 0; i < m; i++) CPPAD_ASSERT_KNOWN(
			work.color[i] <= m,
			"SparseJacobianReverse: invalid value in work."
	);
# endif
 
	typedef typename VectorSet::value_type Set_type;
	typedef typename internal_sparsity<Set_type>::pattern_type Pattern_type;
	Pattern_type s;
	if( work.color.size() == 0 )
	{	bool transpose = false;
		sparsity_user2internal(s, p, m, n, transpose);
	}
	size_t n_sweep = SparseJacobianRev(x, s, row, col, jac, work);
	return n_sweep;
}
/*!
Compute a sparse Jacobian.

The C++ source code corresponding to this operation is
\verbatim
	jac = SparseJacobian(x, p)
\endverbatim

\tparam Base
is the base type for the recording that is stored in this
<code>ADFun<Base></code> object.

\tparam VectorBase
is a simple vector class with elements of type \a Base.

\tparam VectorSet
is a simple vector class with elements of type 
\c bool or \c std::set<size_t>.

\param x [in]
is a vector specifing the point at which to compute the Jacobian.

\param p [in]
is the sparsity pattern for the Jacobian that we are calculating.

\return
Will be a vector if size \c m * n containing the Jacobian at the
specified point (in row major order).
*/
template <class Base>
template <class VectorBase, class VectorSet>
VectorBase ADFun<Base>::SparseJacobian(
	const VectorBase& x, const VectorSet& p
)
{	size_t i, j, k;

	size_t m = Range();
	size_t n = Domain();
	VectorBase jac(m * n);

	CPPAD_ASSERT_KNOWN(
		size_t(x.size()) == n,
		"SparseJacobian: size of x not equal domain size for f."
	);
	CheckSimpleVector<Base, VectorBase>();

	typedef typename VectorSet::value_type Set_type;
	typedef typename internal_sparsity<Set_type>::pattern_type Pattern_type;

	// initialize the return value as zero
	Base zero(0);
	for(i = 0; i < m; i++)
		for(j = 0; j < n; j++)
			jac[i * n + j] = zero;

	sparse_jacobian_work work;
	CppAD::vector<size_t> row;
	CppAD::vector<size_t> col;
	if( n <= m )
	{
		// need an internal copy of sparsity pattern
		Pattern_type s_transpose;
		bool transpose = true;
		sparsity_user2internal(s_transpose, p, m, n, transpose);

		k = 0;
		for(j = 0; j < n; j++)
		{	s_transpose.begin(j);
			i = s_transpose.next_element();
			while( i != s_transpose.end() )
			{	row.push_back(i);
				col.push_back(j);
				k++;
				i = s_transpose.next_element();
			}
		} 
		size_t K = k;
		VectorBase J(K);
	
		// now we have folded this into the following case
		SparseJacobianFor(x, s_transpose, row, col, J, work);

		// now set the non-zero return values
		for(k = 0; k < K; k++)
			jac[ row[k] * n + col[k] ] = J[k];
	}
	else
	{
		// need an internal copy of sparsity pattern
		Pattern_type s;
		bool transpose = false;
		sparsity_user2internal(s, p, m, n, transpose);

		k = 0;
		for(i = 0; i < m; i++)
		{	s.begin(i);
			j = s.next_element();
			while( j != s.end() )
			{	row.push_back(i);
				col.push_back(j);
				k++;
				j = s.next_element();
			}
		} 
		size_t K = k;
		VectorBase J(K);

		// now we have folded this into the following case
		SparseJacobianRev(x, s, row, col, J, work);

		// now set the non-zero return values
		for(k = 0; k < K; k++)
			jac[ row[k] * n + col[k] ] = J[k];
	}

	return jac;
} 

/*!
Compute a sparse Jacobian.

The C++ source code corresponding to this operation is
\verbatim
	jac = SparseJacobian(x)
\endverbatim

\tparam Base
is the base type for the recording that is stored in this
<code>ADFun<Base></code> object.

\tparam VectorBase
is a simple vector class with elements of the \a Base.

\param x [in]
is a vector specifing the point at which to compute the Jacobian.

\return
Will be a vector of size \c m * n containing the Jacobian at the
specified point (in row major order).
*/
template <class Base>
template <class VectorBase>
VectorBase ADFun<Base>::SparseJacobian( const VectorBase& x )
{	typedef CppAD::vectorBool   VectorBool;

	size_t m = Range();
	size_t n = Domain();

	// sparsity pattern for Jacobian
	VectorBool p(m * n);

	if( n <= m )
	{	size_t j, k;

		// use forward mode 
		VectorBool r(n * n);
		for(j = 0; j < n; j++)
		{	for(k = 0; k < n; k++)
				r[j * n + k] = false;
			r[j * n + j] = true;
		}
		p = ForSparseJac(n, r);
	}
	else
	{	size_t i, k;

		// use reverse mode 
		VectorBool s(m * m);
		for(i = 0; i < m; i++)
		{	for(k = 0; k < m; k++)
				s[i * m + k] = false;
			s[i * m + i] = true;
		}
		p = RevSparseJac(m, s);
	}
	return SparseJacobian(x, p);
}


/*! \} */
} // END_CPPAD_NAMESPACE
# endif
