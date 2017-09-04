// -*- compile-command: "g++ -std=c++11 -Wno-pmf-conversions -Wall -Werror -pedantic -Weffc++ -ggdb -o 01_qsort 01_qsort.cpp" -*-

#include <stdlib.h>   // for qsort
#include <string.h>   // for strcasecmp
#include <stdio.h>    // for printf, fputs, putchar

int comps_made;

int compar(const void* left, const void* right)
{
   ++comps_made;
   return strcasecmp(*static_cast<const char* const*>(left),
                     *static_cast<const char* const*>(right));
}

void sort(char** strings, int count)
{
   comps_made = 0;

   // Make copy of strings array
   char **arr = new char*[count];
   for (int i=0; i<count; ++i)
      arr[i] = strings[i];

   qsort(arr, count, sizeof(char*), compar);

   puts("Array after sorting:\n");
   for (int i=0; i<count; ++i)
   {
      if (i)
         putchar(' ');

      fputs(arr[i], stdout);
   }

   putchar('\n');

   printf("Comparisons made: %d\n", comps_made);
}

int main(int argc, char** argv)
{
   sort(&argv[1], argc-1);
   return 0;
}
