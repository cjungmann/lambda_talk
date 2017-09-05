// -*- compile-command: "g++ -std=c++11 -Wno-pmf-conversions -Wall -Werror -pedantic -Weffc++ -ggdb -o qsort qsort.cpp" -*-

#include <stdlib.h>   // for qsort
#include <string.h>   // for strcasecmp
#include <stdio.h>    // for printf, fputs, putchar

int comps_made;

const char** copy_array(const char**target, const char* const* source, int count)
{
   for (int i=0; i<count; ++i)
      target[i] = source[i];

   return target;
}

void print_array(const char**array, int count)
{
   for (int i=0; i<count; ++i)
   {
      if (i)
         putchar(' ');
      fputs(array[i], stdout);
   }
   putchar('\n');
}

/** Note how compar() sets global varialbe comps_made. */
int compar(const void* left, const void* right)
{
   ++comps_made;
   return strcasecmp(*static_cast<const char* const*>(left),
                     *static_cast<const char* const*>(right));
}

void sort(const char** strings, int count)
{
   qsort(strings, count, sizeof(char*), compar);

   puts("Sentence after sorting:\n");
   print_array(strings, count);

   printf("\n%d comparisons made to sort %d elements.\n", comps_made, count);
}

int main(int argc, char** argv)
{
   if (argc < 2)
   {
      printf("Please enter a list of words after qsort command name.\n\n");
      return 1;
   }

   // Make copy of strings array
   const char **arr = copy_array(new const char*[argc-1], &argv[1], argc-1);

   sort(arr, argc-1);

   delete [] arr;

   return 0;
}
