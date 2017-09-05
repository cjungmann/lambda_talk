#!/usr/bin/env node

console.log("Hello, world!");

function get_cli_data_args()
{
   var arr = [];
   for (var i=2, stop=process.argv.length; i<stop; ++i)
      arr.push(process.argv[i]);

   return arr;
}

function write_array(arr)
{
   var lines_printed = 0;

   var f = function(el)
   {
      if (lines_printed++)
         process.stdout.write(' ');

      process.stdout.write(el);
   };

   arr.forEach(f);
   process.stdout.write('\n');
}

function main()
{
   var dargs = get_cli_data_args();

   console.log("\nWords as input:");
   write_array(dargs);

   dargs.sort();
   console.log("\nSame words after standard sorting:");
   write_array(dargs);

   dargs.sort(function(left,right) {return left.toLowerCase().localeCompare(right.toLowerCase());});
   console.log("\nSame words after case-insensitive sorting:");
   write_array(dargs);
}

main();
