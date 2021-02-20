#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h> 
#include "list.h"
#include <sys/wait.h>
#include "writeDir.h"
#include "readDir.h"

#define PERMS 0666

using namespace std;

void end(int);

char mirror_dir[50];
char newFile[60];
fstream logFile;

int main(int argc,char **argv){
	//////////////////DIAVASMA PARAMETRWN
	int id, b;

	char common_dir[50], input_dir[50], log_file[50], strId[10];

	for(int j = 1;j < argc;j++){

                if(strcmp(argv[j],"-c")==0){
                         strcpy(common_dir, argv[j+1]);
		}
		if(strcmp(argv[j],"-i") == 0){
                         strcpy(input_dir, argv[j+1]);
                }
		if(strcmp(argv[j],"-m")== 0){
                         strcpy(mirror_dir, argv[j+1]);
                }
		if(strcmp(argv[j],"-b")== 0){
                         b = atoi(argv[j+1]);
                }
		if(strcmp(argv[j],"-l")== 0){
                         strcpy(log_file, argv[j+1]);
                }
		if(strcmp(argv[j],"-n")== 0){
                        id = atoi(argv[j+1]);
			strcpy(strId, argv[j+1]);
                }
	}

////////////1. ERWTIMA

	DIR *commonDir = NULL;
        DIR *inputDir = NULL;
        DIR *mirrorDir = NULL;
	fstream logFile(log_file, fstream::in | fstream::out | fstream::trunc);
        mirrorDir = opendir(mirror_dir);
        if(mirrorDir != NULL){
                //cout << "mirror_dir directory already exists" << endl;
		printf("errno = %d \n", errno); 
		perror("mirror_dir directory already exists");
		exit(1); 
        }
        inputDir = opendir(input_dir);
        if(inputDir == NULL){
                //cout << "input_dir directory does not exist" << endl;
		printf("errno = %d \n", errno);
                perror("input_dir directory does not exist");
                exit(1);
        }

	closedir(inputDir);
        commonDir = opendir(common_dir);
        if(commonDir == NULL){
                mkdir(common_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		commonDir = opendir(common_dir);
        }
        mirrorDir = opendir(mirror_dir);
        if(mirrorDir == NULL){
		mkdir(mirror_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		mirrorDir = opendir(mirror_dir);
        }

//////////////2. ERWTIMA
	char strId2[10];
	strcpy(strId2,strId);
	char prId[10];
	strcpy(newFile, common_dir);
	strcat(newFile,"/");
	strcat(newFile,strId);
	strcat(newFile, ".id");
	//cout << newFile << endl;

	int filedes, myPrId = ::getpid();
	char str[10];
	sprintf(prId, "%d", myPrId);
	//cout << prId << endl;

	if ((filedes=open(newFile ,O_CREAT,PERMS))==-1){
		perror("creating file");
		exit(1);
	}

	//filedes=open(newFile ,O_RDWR,PERMS);
	//cout << filedes << endl;
	close(filedes);
	if ((filedes=open(newFile ,O_RDWR,PERMS))==-1){
                perror("opening file");
                exit(1);
        }
	write(filedes,prId,strlen(prId));

	close(filedes);
	if ((filedes=open(newFile ,O_RDWR,PERMS))==-1){
                perror("opening file");
                exit(1);
        }


	int bytes;
	char buf[10];
	bytes=read(filedes, buf, strlen(prId));
	buf[bytes]='\0';
	printf("client's process id: %s\n",buf);

	close(filedes);

////////////////////3.
	list clientList;
	dirent* record;
	dirent* inputRecord;

	char strFifo[20];

	pid_t child1 = 1;
	pid_t child2 = 1;

	clientList.insertClient(id);

	closedir(commonDir);


/////////////////////signals for quit
	signal(SIGINT, end);
	signal(SIGQUIT, end);

/////////////////////4. ERWTIMA
	logFile.is_open(); //anoigw ta logs

	while(true){
		signal(SIGUSR1,SIG_IGN);
		commonDir = opendir(common_dir);
		while((record = readdir(commonDir)) != NULL){
			if(record->d_name[strlen(record->d_name)-1] != 'd' || record->d_name[strlen(record->d_name)-2] != 'i'){
				continue;
			}
			record->d_name[strlen(record->d_name)-3] = '\0';
			///////////TSEKARW AN YPARXEI STIN LISTA
			if(!clientList.search(atoi(record->d_name))){

				child1 = fork();

				if(child1 == 0){
					strcpy(strFifo, common_dir);
	        			strcat(strFifo,"/");
					strcat(strFifo, "id");
					strcat(strFifo, strId2);
					cout << "child 1 pid: " << ::getpid() <<endl;
					strcat(strFifo, "_to_");
					strcat(strFifo, "id");
					strcat(strFifo,  record->d_name);
					mkfifo(strFifo, 0666);
					inputDir = opendir(input_dir);

					if(inputDir == NULL){
                				//cout << "input_dir directory does not exist" << endl;
                				printf("errno = %d \n", errno);
                				perror("input_dir directory does not exist");
                				exit(1);
        				}

					char zero[2];
        				zero[1] = '0';
        				zero[0] = '0';
					int f_id1;
					char writeBuf[80];
					f_id1 = open(strFifo, O_WRONLY);
					//////////////KALW TIN WRITEDIR
					writeDir(inputDir,input_dir, record->d_name, f_id1, b,logFile);
					////////////VAZW TA MIDENIKA 
					write(f_id1,zero, 2);
                                        close(f_id1);
					closedir(inputDir);
					exit(0);

				}else if(child1 > 0){

					child2 = fork();

					if(child2 == 0){
						cout << "child 2 pid: " << ::getpid() <<endl;
						strcpy(strFifo, common_dir);
                               		 	strcat(strFifo,"/");
						strcat(strFifo, "id");
                                		strcat(strFifo, record->d_name);
                                		strcat(strFifo, "_to_");
						strcat(strFifo, "id");
                                		strcat(strFifo,  strId2);
                                		mkfifo(strFifo, 0666);
						int f_id2;
						short int nameSize;
						char readBuf[80];
                                                f_id2 = open(strFifo, O_RDONLY);
						////////////KALW TIN READDIR
						readDir(mirrorDir, mirror_dir, record->d_name,  f_id2, b, logFile);
						close(f_id2);
						exit(0);

					}else if (child1 < 0){
                       		         	perror("error in fork");
                        		}else if(waitpid(child2, NULL, 0) != 0){
						cout << "transfer completed successfully" << endl;
					}
				}else if(child1 < 0){
					perror("error in fork");
				}
				////////////AFOY TON EPE3ERGASTIKA TON VAZW STIN LISTA
				clientList.insertClient(atoi(record->d_name));
			}

		}
		closedir(commonDir);
		////////////////KWDIKAS GIA TO ERWTIMA 5(EINAI MESA STIN MEGALI WHILE)
		list* clientList2;
		clientList2 = new list;
		commonDir = opendir(common_dir);
                while((record = readdir(commonDir)) != NULL){
                        if(record->d_name[strlen(record->d_name)-1] != 'd' || record->d_name[strlen(record->d_name)-2] != 'i'){
                                continue;
                        }
                        record->d_name[strlen(record->d_name)-3] = '\0';
                        clientList2->insertClient(atoi(record->d_name));
		}
		int removedId = -1;
		int deleteChild;
		if((removedId = clientList.compare(clientList2)) != -1){
			cout << "client " << removedId  << " is out" << endl;
			clientList.deleteClient(removedId);
			deleteChild = fork();
                                if(deleteChild == 0){
					char rId[10];
					sprintf(rId, "%d", removedId);
					char rmcommand[40];
                			strcpy(rmcommand, "rm -rf ");
                			strcat(rmcommand, mirror_dir);
					strcat(rmcommand, "/");
					strcat(rmcommand, rId);
                			system(rmcommand);

					exit(0);
				}
		}
		delete clientList2;
		closedir(commonDir);
	}

}

/////////////////TO ERWTIMA 6.
void end(int signum){
	DIR* dir = opendir(mirror_dir);
	if(dir){
		cout << "delete mirror Dir " << mirror_dir << endl;
    		closedir(dir);
		char rmcommand[40];
        	strcpy(rmcommand, "rm -rf ");
        	strcat(rmcommand, mirror_dir);
        	system(rmcommand);
	}
	remove(newFile);
	cout << "i removed a client with " << newFile <<endl;
	logFile.close();
	exit(0);
	

}
