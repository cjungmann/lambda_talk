// -*- compile-command: "g++ -std=c++11 -Wno-pmf-conversions -Wall -Werror -pedantic -Weffc++ -ggdb -o passout passout.cpp" -*-

#include <stdlib.h>
#include <stdio.h>

class IFace
{
public:
   virtual ~IFace() {}
   virtual void operator()(void) const = 0;
};

template <typename Func>
class Implementation : public IFace
{
   Func &m_f;
public:
   Implementation(Func &f) : m_f(f) { }

   // To dispell effc++ warnings:
   virtual ~Implementation() { }
   Implementation(const Implementation&) = delete;
   Implementation& operator=(const Implementation&) = delete;

   virtual void operator()(void) const { m_f(); }
};

// global function pointer:
IFace* if_main = nullptr;
IFace* if_child = nullptr;

void grandchild(void)
{
   printf("\nIn grandchild, calling child and main.\nBoth should work.\n");
   printf("grandchild calling (*if_child):");
   (*if_child)();
   printf("\ngrandchild calling (*if_main):");
   (*if_main)();
}

void child(int number)
{
   auto f = [&number](void) -> void
   {
      printf("\nIn child's lambda, number = %d.\n", number);
   };

   Implementation<decltype(f)> imp(f);

   if_child = &imp;

   grandchild();
   printf("\nReturned from grandchild.\n");
}

int main(int argc, char** argv)
{
   auto f = [&argc, &argv](void) -> void
   {
      printf("\nIn main's lambda, with %d arguments.\n", argc);
      if (argc>1)
      {
         for (int i=1; i<argc; ++i)
         {
            if (i==1)
               fputs("---", stdout);
            else
               putchar(' ');

            fputs(argv[i], stdout);
         }
      }
      printf("\n");
   };

   Implementation<decltype(f)> imp(f);
   if_main = &imp;

   child(argc);

   printf("\nChild has returned, and its scope has ended.\n");
   printf("What will happen if we call the pointer to its lambda function?\n");
   printf("It should crash...here we go...\nmain calling (*if_child)()");

   (*if_child)();
   
   return 0;
}
