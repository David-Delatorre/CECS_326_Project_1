The Filecopy.c program has a parent process open an input file and write its contents to a pipe. 
Then a child process reads the contents of the pipe and writes it to an output file.
To compile type: ./executable_name source_file destination_file
For example: ./filecopy input.txt output.txt
