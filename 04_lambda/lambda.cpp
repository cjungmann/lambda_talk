// -*- compile-command: "g++ -std=c++11 -Wno-pmf-conversions -Wall -Werror -pedantic -Weffc++ -ggdb -o lambda lambda.cpp" -*-

#include <stdlib.h>
#include <stdio.h>

/**
 * Fills an array with a set of randomized, unduplicated values.
 *
 * @param arr An array of integers in which to store random values.
 * @param count Number of elements in the integer array.
 */
void randomize_ints(int *arr, int count)
{
   int val;
   bool is_unique;
   for (int i=0; i<count; ++i)
   {
      do
      {
         val = rand() % count + 1;

         // Check that the value has not been used:
         is_unique = true;
         for (int j=0; j<i; ++j)
            if (arr[j]==val)
            {
               is_unique = false;
               break;
            }
      } while (!is_unique);

      arr[i] = val;
   }
}

/**
 * Fills an array with a set of randomized, unduplicated values.
 *
 * @param arr An array of integers in which to store random values.
 * @param count Number of elements in the integer array.
 *
 * This function uses a lambda function to detect if a given integer
 * has already been used.
 */
void randomize_ints_lambda(int* arr, int count)
{
   // for-loop index variable must be available to lambda
   int i;

   auto is_unique = [&arr, &count, &i](int val) -> bool
   {
      for (int j=0; j<i; ++j)
         if (arr[j]==val)
            return false;

      return true;
   };
   
   int val;
   for (i=0; i<count; ++i)
   {
      // Keep getting random values until we find an unused value.
      while(!is_unique(val=rand()%count+1))
         ;

      arr[i] = val;
   }
}

/**
 * Print integer list in current order.  Use to compare before and after sorting.
 */
void print_ints(const int* arr, int count)
{
   for (int i=0; i<count; ++i)
   {
      if (i)
         putchar(' ');

      printf("%3d", arr[i]);
   }

   putchar('\n');
}

/** Interface class that will be used for parameter declaration. */
class Compar_Base
{
public:
   virtual ~Compar_Base()              { }
   virtual int comp(const void* left, const void* right) const = 0;

   /** Static function is suitable for fourth parameter of qsort_r. */
   static int compare(const void* left, const void* right, void* obj)
   {
      return static_cast<Compar_Base*>(obj)->comp(left,right);
   }
};

/** Template class derived from interface class for implementations. */
template <typename Func>
class Compar_Concrete : public Compar_Base
{
   Func &m_f;
public:
   /**
    * Constructor must use a reference parameter so we don't get a copy
    * of the lambda function, which would have an invalid closure.
    */
   Compar_Concrete(Func &f) : m_f(f) { }

   // To dispell effc++ warnings:
   virtual ~Compar_Concrete()       { }
   Compar_Concrete(const Compar_Concrete&) = delete;
   Compar_Concrete& operator=(const Compar_Concrete&) = delete;

   virtual int comp(const void* left, const void* right) const
   {
      return m_f(left,right);
   }

};

/**
 * qsort that uses a lambda function to compare elements.
 */
template <typename Func>
void qsort_l(void *base, size_t member_count, size_t member_size, Func f)
{
   Compar_Concrete<Func> comp(f);
   qsort_r(base, member_count, member_size, Compar_Base::compare, &comp);
}


/**
 * Usage example of qsort_l.
 */
void sort_ascending(int* intlist, int count)
{
   auto f = [](const void* left, const void* right) -> int
   {
      int l = *static_cast<const int*>(left);
      int r = *static_cast<const int*>(right);
      return l - r;
   };

   qsort_l(intlist, count, sizeof(int), f);
}

/**
 * Usage example of qsort_l.
 */
void sort_descending(int* intlist, int count)
{
   int testcount = 0;
   auto f = [&testcount](const void* left, const void* right) -> int
   {
      ++testcount;
      int l = *static_cast<const int*>(left);
      int r = *static_cast<const int*>(right);
      return r - l;
   };

   qsort_l(intlist, count, sizeof(int), f);

   printf("\nIt took %d tests to complete the sort.\n", testcount);
}





#define ARRLEN 25

int main(int argc, char** argv)
{
   int* intarr = new int[ARRLEN];

   randomize_ints(intarr, ARRLEN);
   print_ints(intarr,ARRLEN);
   sort_ascending(intarr,ARRLEN);
   print_ints(intarr,ARRLEN);
   
   randomize_ints(intarr, ARRLEN);
   print_ints(intarr,ARRLEN);
   sort_descending(intarr,ARRLEN);
   print_ints(intarr,ARRLEN);
   return 0;
}
