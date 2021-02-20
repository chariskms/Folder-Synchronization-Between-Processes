# Folder-Synchronization-Between-Processes
## Assignment for lesson: K24 System Programming (DI University of Athens)
### Description:
A simplified version of File Syncing program where different processes should synchronize a set of files. All processes work with the same way and their goal is to communicate and synchronize with each other in order to have all from a common copy of the files.
### Compilation:
A makefile is included.
### Execution:
./mirror_client -n id -c common_dir -i input_dir -m mirror_dir -b buffer_size -l log_file

- id: is a number that is an identifier for that particular client
- common_dir: is a directory that will be used to communicate between mirror_clients.
- input_dir: is a directory that contains the files that this user contributes client.
- b: is the size of the buffer for reading over the pipes.
- mirror_dir: is a directory that contains files from all participating clients in mirroring. This directory is created by each client individually to store all files
to receive from other clients.
- log_file: all messages are written there during client execution
### Execution example:
./mirror_client -n 1 -c ./common -i ./1_input -m ./1_mirror -b 100 -l log_file1 <br>
./mirror_client -n 2 -c ./common -i ./2_input -m ./2_mirror -b 200 -l log_file2

We start two clients with ids 1 and 2, they will use the ./common directory to communicate with them
other clients and the 1_input and 2_input directory respectively for the files they contribute and
1_mirror and 2_mirror to save the downloaded files. The goal is for clients to get started
to communicate autonomously so that after some time everyone has a copy of everything
archives.

### script create_infiles.sh

In this script our goal is to create a hierarchy of files and directories that will
used as input by each client. The script create_infiles.sh works
as follows:

### Execution:

./create_infiles.sh dir_name num_of_files num_of_dirs levels 
- dir_name: the directory where the files will be written.
- num_of_files: the total number of files
- num_of_dirs: the total number of directories
- levels: how many levels the directories will be distributed

### script get_stats.sh 

The purpose of this script is to process logfiles from all clients and extract them
statistics. The script will run as:

### Execution:

cat log_file1 log_file2 … log_filen | ./get_stats.sh and prints:

- How many clients connected and who (a list of all ids from clients)
- The minimum and maximum id of clients
- The number of bytes sent
- The number of bytes read
- The number of files sent
- The number of files downloaded
- The number of clients that left the system
