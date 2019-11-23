#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[])
{
   // Initialize variables

   // Initialize a timer
   clock_t timer_running;  
   timer_running = clock();

   char ch;
   FILE *source, *target;
   long int size = 0;
   int errnum;

   // If statement to check whether the user has entered enough arguments for the program to run
   if (argc > 3) 
   {
      printf("duplicate: too many arguments!\n");
      printf("Usage: ./duplicate <source_file> <target_file>\n");
      exit(1);
   } 
   else if (argc < 3) 
   {
      printf("duplicate: too few arguments!\n");
      printf("Usage: ./duplicate <source_file> <target_file>\n");
      exit(1);
   }

   printf("start time is : %f seconds\n", (double) timer_running/CLOCKS_PER_SEC);

   // Assign the first argument to "source". "source" will contain the first file with read permissions
   source = fopen(argv[1], "r");

   if (source == NULL)
   {
      errnum = errno;
      fprintf(stderr, "Error opening file %s: %s\n", argv[1], strerror(errnum));
      exit(1);
   }

   // Assign the second argument to "target". "target" will contain the second file with write permissions
   target = fopen(argv[2], "w");

   if (target == NULL)
   {
      fclose(source);
      errnum = errno;
      fprintf(stderr, "Error opening file %s: %s\n", argv[2], strerror(errnum));
      exit(1);
   }

   // This while loop will go through the first file until end of file and save each character to "ch"
   while ((ch = fgetc(source)) != EOF)
   {
      // Every iteration, fputc saves each character read from "source" into "target"
      fputc(ch, target);

      // If statement to check each iteration whether the timer has reached greater than 1 second
      // If so, it will print "duplicate: still duplicating..."
      if (((double) timer_running/CLOCKS_PER_SEC) > 1)
      {
         printf("duplicate: still duplicating...\n");
      }
   }

   // Using fseek and ftell to get the size of the target file to see if all bytes were copied 
   // from "source" to "target"
   fseek(target, 0, SEEK_END);
   size = ftell(target);
   if(size!=-1)
      printf("Copied %ld bytes from file %s to %s\n", size, argv[1], argv[2]);
   else
      errnum = errno;
      fprintf(stderr, "Error with file %s: %s\n", argv[2], strerror( errnum ));
   
   // End timer, convert to double and display the total amount taken by the program
   timer_running = clock() - timer_running; 
   double time_taken = ((double) timer_running)/CLOCKS_PER_SEC;
   printf("end time is %f seconds\n", (double) timer_running/CLOCKS_PER_SEC);

   // Close both "source" file and "target" file
   fclose(source);
   fclose(target);

   return 0;
}