#!/usr/bin/env node

/**
 * Get CLI arguments to create an array for the demonstration.
 *
 * Omit first two arguments, "node" and "lambda.js", so the
 * array only includes the arguments after the script name.
 */
function get_cli_data_args()
{
   var arr = [];
   for (var i=2, stop=process.argv.length; i<stop; ++i)
      arr.push(process.argv[i]);

   return arr;
}

/**
 * Print each array element in a line
 *
 * Uses the closure to track elements printed for adding spaces.
 * Uses a *parameter-instatiated internal function*
 *
 * Lambda lifetime: while arr.forEach() is running.
 * Lambda strength: persistent lines_printed variable.
 */
function write_array_param_instantiated(arr)
{
   var lines_printed = 0;

   arr.forEach(function(el) {
      if (lines_printed++)
         process.stdout.write(el);
      process.stdout.write(el);} );

   process.stdout.write('\nPARAMETER-INSTANTIATION version\n');
}

/**
 * Print each array element in a line
 *
 * Uses the closure to track elements printed for adding spaces.
 * Uses a *named internal function*
 *
 * Lambda lifetime: duration of the host function.
 * Lambda strength: persistent lines_printed variable.
 */
function write_array_named_internal(arr)
{
   var lines_printed = 0;

   function writer(el)
   {
      if (lines_printed++)
         process.stdout.write(' ');

      process.stdout.write(el);
   };

   arr.forEach(writer);
   process.stdout.write('\nNAMED INTERNAL version\n');
}

/**
 * Print each array element in a line
 *
 * Uses the closure to track elements printed for adding spaces.
 * Uses a *unnamed internal function*
 *
 * Lambda lifetime: duration of the host function.
 * Lambda strength: persistent lines_printed variable.
 */
function write_array_unnamed_internal(arr)
{
   var lines_printed = 0;

   var f =  function(el)
   {
      if (lines_printed++)
         process.stdout.write(' ');

      process.stdout.write(el);
   }

   arr.forEach(f);

   process.stdout.write('\nUNNAMED INTERNAL version\n');
}


/**
 * Convert XML-problem characters with appropriate entity.
 *
 * This function only executes once. Afterwards calling
 * escape_xml will call the internal anonymous function.
 * This is more efficient because the internal function
 * and the RegExp object need be created only once.
 *
 * Demonstrates how a nested function can be assigned to a
 * global identifier while maintaining access to the local
 * variables of the initial function.
 *
 * Lambda lifetime: from first call to escape_xml() to
 *                  end of program.
 *
 * Lambda strengths: 1. manage RegExp lifetime
 *                   2. expensive setup once
 */
function escape_xml(str)
{
   // named internal function
   function conv(s)
   {
      switch(s)
      {
         case '>': return "&gt;";
         case '<': return "&lt;";
         case '&': return "&amp;";
         case '"': return "&quot;";
         case '\'': return "&apos;";
         default: return substr;
      }
   }

   // Closure variable, available to internal functions
   var re_xml = /[<>&\'\"]/g;

   // Reassigning global identifier to internal unnamed function
   escape_xml = function(str) { return str.replace(re_xml, cb); };

   // Calling reassigned anonymous function to complete first call.
   return escape_xml(str);
}
 

/**
 * Simple class to illustrate a global anonymous function.
 *
 * Is the function attached to process() a lambda function?
 *    It's anonymous, but it's global.
 *
 * Are classes lambdas?
 *    The class returns a *this* value whose internal structure
 *    lasts as long as the class instance.
 */
function MyClass() { this.id = "I am a MyClass instance"; }

MyClass.prototype.process = function()
{
 console.log("global anonyous function"); 
};

MyClass.prototype.execute = function myclass_execute()
{
   if ("id" in this)
      console.log("myclass_execute for MyClass instance");
   else
      console.log("myclass_execute for global window object");
};

function test_myclass()
{
   console.log("About to demonstrate MyClass.");
   var myclass = new MyClass;

   myclass.process();
   myclass.execute();

   // Calling global function, unattached to any object:
   myclass_execute();
}




function main()
{
   var dargs = get_cli_data_args();

   var write_array = write_array_param_instantiated;

   console.log("\nWords as input:");
   write_array(dargs);

   write_array = write_array_named_internal;

   dargs.sort();
   console.log("\nSame words after standard sorting:");
   write_array(dargs);

   write_array = write_array_unnamed_internal;

   dargs.sort(function(left,right) {return left.toLowerCase().localeCompare(right.toLowerCase());});
   console.log("\nSame words after case-insensitive sorting:");
   write_array(dargs);

   
}

main();
