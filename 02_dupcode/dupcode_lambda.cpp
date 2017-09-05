// -*- compile-command: "g++ -std=c++11 -Wno-pmf-conversions -Wall -Werror -pedantic -Weffc++ -ggdb -o dupcode_lambda dupcode_lambda.cpp" -*-

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>  // for isdigit

const char* list[] = {"+8", "+12", "45-", "-85", "+34", "-65", "94+", "-32", "+73", "48+", "-21", "+88", "59-", nullptr};

int count_list(void)
{
   int count = 0;
   const char** arr = list;
   while (*arr)
   {
      ++count;
      ++arr;
   }

   return count;
}

void print_list(void)
{
   const char **arr = list;
   while (*arr)
   {
      printf("%6s", *arr);
      ++arr;
   }
}

void tally_list(void)
{
   int tally = 0;
   const char** arr = list;
   bool postfix = false;
   int curint, len;
   char curop;

   auto pre_print = [&tally, &postfix](void)
   {
      if (postfix)
         printf("%6d", tally);
   };

   auto post_print = [&tally, &postfix](void)
   {
      if (!postfix)
         printf("%6d", tally);
   };

   while (*arr)
   {
      const char *str = *arr;

      len = strlen(str);
      curop = '\0';
      
      postfix = isdigit(str[0]);
      if (postfix)
      {
         curint = atoi(str);
         curop = str[len-1];
      }
      else
      {
         curint = atoi(&str[1]);
         curop = *str;
      }

      switch(curop)
      {
         case '+':
            pre_print();
            tally += curint;
            post_print();
            break;
         case '-':
            pre_print();
            tally -= curint;
            post_print();
            break;
      }
      ++arr;
   }


   printf("\nThere were %d elements in the list.\n", count_list());
   printf("The tally ended up at %d.\n", tally);
}


int main(int argc, char** argv)
{
   print_list();
   putchar('\n');
   tally_list();
}
