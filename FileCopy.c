/*
 * FileCopy.c
 * 
 * This program opens a file and writes its contents to a pipe 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // This header is needed for pipe() and fork()

#define BUFFER_SIZE 25
#define STRING_SIZE 25
#define READ_END 0 
#define WRITE_END 1

int main(int argc, char *argv[])
{

   char *inputFile; 
   char *outputFile;
   FILE *inFile;
   FILE *outFile;
   int file_descriptors_list[2];
   pid_t process_id;
   char buffer[BUFFER_SIZE];

   if (argc < 3 || argc > 3) {
      printf("Please type ./filecopy input.txt output.txt\n");
      return 1; 
    }
   // Extract the input and output files from command line
   inputFile = argv[1];
   outputFile = argv[2];


    // Open the input file in read mode
    inFile = fopen(inputFile, "r");
    if (inFile == NULL) {
        perror("Error opening input file");
        return 1;
    }

    // Open the output file in write mode
    outFile = fopen(outputFile, "w");
    if (outFile == NULL) {
        perror("Error opening output file");
        return 1;
    }


    // Create the pipe 
    if (pipe(file_descriptors_list) != 0) {
      fprintf(stderr,"Pipe failed");
      return 1;
    }

    // Fork a child process
    process_id = fork();

   fclose(inFile);
   fclose(outFile);
   printf("Success!\n");
   return 0;
               
}
