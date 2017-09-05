// -*- compile-command: "g++ -std=c++11 -Wno-pmf-conversions -Wall -Werror -pedantic -Weffc++ -ggdb -o recursion recursion.cpp" -*-

#include <stdlib.h>
#include <stdio.h>


/** Interface class to abstract the task. */
class Factorial_Interface
{
public:
   virtual ~Factorial_Interface() {}
   virtual unsigned long long operator()(unsigned long long number) const = 0;
};

/**
 * Template-based implementation class to host the lambda function.
 *
 * Note constructor takes a reference.  If you use a value parameter,
 * the runtime will create a copy of the lambda, but it will be
 * without the captures.
 *
 * For some reason, the compile doesn't complain about the copy, despite
 * deleting the copy constructor...I'll look into that when I have some time.
 */
template <typename Func>
class Factorial_Implementation : public Factorial_Interface
{
   Func &m_f;
public:
   Factorial_Implementation(Func &f) : m_f(f) { }

   // To dispell effc++ warnings:
   virtual ~Factorial_Implementation() { }
   Factorial_Implementation(const Factorial_Implementation&) = delete;
   Factorial_Implementation& operator=(const Factorial_Implementation&) = delete;

   virtual unsigned long long operator()(unsigned long long number) const { return m_f(number); }
};

/** Function that hosts the lambda used recursively. */
int factorial(long long unsigned number)
{
   Factorial_Interface *fi = nullptr;

   auto f = [&fi](unsigned long long number) -> unsigned long long
   {
      if (number > 1)
         return number * (*fi)(number-1);
      else
         return 1;
   };

   Factorial_Implementation<decltype(f)> fic(f);
   fi = &fic;

   return (*fi)(number);
}

int main(int argc, char** argv)
{
   unsigned long long num = 25;
   unsigned long long result = factorial(num);
   printf("Calculated result of %llu factorial: %llu.\n", num, result);

   return 0;
}
