/*
 * FileCopy.c
 * 
 * This program opens a file and writes its contents to a pipe 
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // This header is needed for pipe() and fork()
#include <fcntl.h>   // For open(), O_RDONLY, O_WRONLY, O_CREAT, etc.


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
   ssize_t bytes_read_by_child;
   ssize_t bytes_read_by_parent;


   if (argc < 3 || argc > 3) {
      printf("Please type ./filecopy input.txt output.txt\n");
      return 1; 
    }
   // Extract the input and output files from command line
   inputFile = argv[1];
   outputFile = argv[2];



    // Create the pipe 
    if (pipe(file_descriptors_list) != 0) {
      printf("Pipe Failed\n");
      return 1;
    }

    // Fork a child process
    process_id = fork();

    // if pid<0 -> error code
    if(process_id < 0) {
      printf("Fork Failed\n");
      return 1;
    }
    // else if pid=0 -> child code
    else if(process_id == 0) {
      close(file_descriptors_list[WRITE_END]);


      FILE *write_file_descriptor = fopen(outputFile, "w");
      if (write_file_descriptor == NULL) {
        printf("Failed to open output file\n");
        return 1;
      }
      // Read from pipe
      // Write to output file
      while ((bytes_read_by_child = read(file_descriptors_list[READ_END], buffer, BUFFER_SIZE)) > 0) {

        fwrite(buffer, 1, bytes_read_by_child, write_file_descriptor);
      }
      if (bytes_read_by_child == -1){
        printf("Error Reading File\n");
        return 1;
      }
      fclose(write_file_descriptor);
      close(file_descriptors_list[READ_END]);  // Close read end

      exit(EXIT_SUCCESS);
    }
        // else -> parent code 
    else {
      // close read end of the pipe
      close(file_descriptors_list[READ_END]);  

      FILE *read_file_descriptor = fopen(inputFile, "r");
      if (read_file_descriptor == NULL) {
        printf("Failed to open input file\n");
        return 1;
      }

      // write to the write end of the pipe
      while ((bytes_read_by_parent = fread(buffer, 1, BUFFER_SIZE, read_file_descriptor)) > 0){
        write(file_descriptors_list[WRITE_END], buffer, bytes_read_by_parent);
      }

      if (bytes_read_by_parent == -1){
        printf("Error Reading File\n");
        exit(EXIT_FAILURE);
      }

      fclose(read_file_descriptor);
      close(file_descriptors_list[WRITE_END]); // close the write end of pipe

      wait(NULL);
    }
    // stdin -> keyboard
    // stdout -> talking to user
    // stderr -> error
    printf("File successfully copied from input.txt to output.txt\n");
    return 0;
               
}
