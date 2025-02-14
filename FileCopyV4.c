/*
 * FileCopy.c
 * David Delatorre ID: 027826485
 * Francis Salonga ID: 032320195
 * CECS 326 - 02 
 * This program has a parent process open an input file and write its contents to a pipe.
 * Then a child process reads the contents of the pipe and writes it to an output file.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // This header is needed for pipe() and fork()


#define BUFFER_SIZE 25
#define BYTE_SIZE 1
#define READ_END 0 
#define WRITE_END 1

int main(int argc, char *argv[])
{

   char *inputFile; 
   char *outputFile;
   FILE *read_file_descriptor;
   FILE *write_file_descriptor;
   int pipe_file_descriptors[2];
   pid_t process_id;
   char buffer[BUFFER_SIZE];
   ssize_t bytes_read_by_child;
   ssize_t bytes_read_by_parent;

  // check for the correct number of arguements 
   if (argc < 3 || argc > 3) {
      printf("Please type ./filecopy input.txt output.txt\n");
      exit(EXIT_FAILURE);
    }

   // Extract the input and output files from command line
   inputFile = argv[1];
   outputFile = argv[2];

    // Create the pipe 
    if (pipe(pipe_file_descriptors) != 0) {
      printf("Pipe Failed\n");
      exit(EXIT_FAILURE);
    }

    // Fork a child process
    process_id = fork();

    // if pid<0 -> error code
    if(process_id < 0) {
      printf("Fork Failed\n");
      exit(EXIT_FAILURE);
    }
    // else if pid=0 -> child code
    else if(process_id == 0) {
      // close unused write end of pipe
      close(pipe_file_descriptors[WRITE_END]);

      // open output file in write mode 
      write_file_descriptor = fopen(outputFile, "w");
      if (write_file_descriptor == NULL) {
        printf("Failed to open output file\n");
        exit(EXIT_FAILURE);
      }

      // Read data from read end of pipe and write it to output file
      while ((bytes_read_by_child = read(pipe_file_descriptors[READ_END], buffer, BUFFER_SIZE)) > 0) {
        fwrite(buffer, BYTE_SIZE, bytes_read_by_child, write_file_descriptor);
      }
      if (bytes_read_by_child == -1){
        printf("Error Reading File\n");
        exit(EXIT_FAILURE);
      }

      fclose(write_file_descriptor);           // Close the output file
      close(pipe_file_descriptors[READ_END]);  // Close read end of pipe

      exit(EXIT_SUCCESS);
    }
        // else -> parent code 
    else {
      // close read end of the pipe
      close(pipe_file_descriptors[READ_END]);  

      // open the input file in read mode
      read_file_descriptor = fopen(inputFile, "r");
      if (read_file_descriptor == NULL) {
        printf("Failed to open input file\n");
        exit(EXIT_FAILURE);
      }

      // read from the input file and write to the write end of pipe
      while ((bytes_read_by_parent = fread(buffer, BYTE_SIZE, BUFFER_SIZE, read_file_descriptor)) > 0){
        write(pipe_file_descriptors[WRITE_END], buffer, bytes_read_by_parent);
      }

      if (bytes_read_by_parent == -1){
        printf("Error Reading File\n");
        exit(EXIT_FAILURE);
      }

      fclose(read_file_descriptor);            // close the input file
      close(pipe_file_descriptors[WRITE_END]); // close the write end of pipe

      wait(NULL); // waits for the child process to terminate
    }

    printf("File successfully copied from input.txt to output.txt\n");
    return 0;
               
}
