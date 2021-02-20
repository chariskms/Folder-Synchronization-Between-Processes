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
