// -*- compile-command: "g++ -std=c++11 -ggdb -Wno-pmf-conversions -Wall -Werror -Weffc++ -pedantic -o lambda lambda.cpp" -*-

#include <stdlib.h>
#include <stdio.h>


/**
 * Creates a randomized list of unique integers.
 *
 * @param arr An array of integers in which to store random values.
 * @param count Number of elements in the integer array.
 *
 * This function uses a lambda function to detect if a given integer
 * has already been used.

 */
void randomize_ints(int* arr, int count)
{
   // for-loop index variable must be available to lambda
   int i;

   auto is_unique = [&arr, &count, &i](int val) -> int
   {
      for (int j=0; j<i; ++j)
         if (arr[j]==val)
            return false;

      return true;
   };
   
   int val;
   for (i=0; i<count; ++i)
   {
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

#define EFFC_2(X) X(const X&) = delete; X& operator=(const X&) = delete

/**
 * Use for parameter definition.
 */
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

/**
 * Base class for implementations
 */
template <typename Func>
class Compar_Concrete : public Compar_Base
{
   Func &m_f;
public:
   Compar_Concrete(Func f) : m_f(f) { }
   // virtual ~Compar_Concrete()       { }
   virtual int comp(const void* left, const void* right) const
   {
      return m_f(left,right);
   }

   // EFFC_2(Compar_Concrete);

   // Compar_Concrete(const Compar_Concrete&) = delete;
   // Compar_Concrete& operator=(const Compar_Concrete&) = delete;

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
      return *static_cast<const int*>(left) - *static_cast<const int*>(right);
   };

   qsort_l(intlist, count, sizeof(int), f);
}

/**
 * Usage example of qsort_l.
 */
void sort_descending(int* intlist, int count)
{
   int testcount = 0;
   auto f = [&testcount](const void* left, const void* right) mutable -> int
   {
      ++testcount;
      return *static_cast<const int*>(right) - *static_cast<const int*>(left);
   };

   qsort_l(intlist, count, sizeof(int), f);

   // printf("\nIt took %d tests to complete the sort.\n", testcount);
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
