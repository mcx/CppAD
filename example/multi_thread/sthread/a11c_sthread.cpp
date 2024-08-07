// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin a11c_sthread.cpp}

A Simple Boost Thread Example and Test
######################################

Purpose
*******
This example just demonstrates Boost threads and does not use CppAD at all.

Source Code
***********
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end a11c_sthread.cpp}
----------------------------------------------------------------------------
*/
// BEGIN C++
# include <thread>
# include <limits>
# include <cmath>
# include <cassert>
// for size_t
# include <cstddef>
//
# define NUMBER_THREADS 4

namespace { // Begin empty namespace
   class worker_t
   {
   private:
      int    n_;
      float* a_;
      float* b_;
   public:
      void setup(size_t n, float* a, float* b)
      {  n_ = static_cast<int>(n);
         a_ = a;
         b_ = b;
      }
      // Beginning of Example A.1.1.1c of OpenMP 2.5 standard document
      void a1(int n, float *a, float *b)
      {  int i;
         for(i = 1; i < n; i++)
            b[i] = (a[i] + a[i-1]) / 2.0f;
         return;
      }
      // End of Example A.1.1.1c of OpenMP 2.5 standard document
      void operator()()
      {  a1(n_, a_, b_); }
   };
}

bool a11c(void)
{  bool ok = true;

   // Test setup
   size_t i, j, n_total = 10;
   float *a = new float[n_total];
   float *b = new float[n_total];
   for(i = 0; i < n_total; i++)
      a[i] = float(i);

   // number of threads
   size_t number_threads = NUMBER_THREADS;

   // set of workers
   worker_t worker[NUMBER_THREADS];

   // threads for each worker
   std::thread* sthread[NUMBER_THREADS];

   // Break the work up into sub work for each thread
   size_t  n     = n_total / number_threads;
   size_t  n_tmp = n;
   float*  a_tmp = a;
   float*  b_tmp = b;
   worker[0].setup(n_tmp, a_tmp, b_tmp);
   for(j = 1; j < number_threads; j++)
   {  n_tmp = n + 1;
      a_tmp = a_tmp + n - 1;
      b_tmp = b_tmp + n - 1;
      if( j == (number_threads - 1) )
         n_tmp = n_total - j * n + 1;

      worker[j].setup(n_tmp, a_tmp, b_tmp);

      // create this thread
      sthread[j] = new std::thread(worker[j]);
   }

   // do this threads protion of the work
   worker[0]();

   // wait for other threads to finish
   for(j = 1; j < number_threads; j++)
    { sthread[j]->join();
      delete sthread[j];
      sthread[j] = nullptr;
   }

   // check the result
   float eps = 100.f * std::numeric_limits<float>::epsilon();
   for(i = 1; i < n ; i++)
      ok &= std::fabs( (2. * b[i] - a[i] - a[i-1]) / b[i] ) <= eps;

   delete [] a;
   delete [] b;

   return ok;
}
// END C++
