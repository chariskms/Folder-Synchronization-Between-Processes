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

#define PERMS 0666
using namespace std;

void writeDir(DIR* dir, char* folderName, char* client, int fifoId, int b, fstream& logFile){


	cout << "client :" << folderName << endl;
        dirent* inputRecord;
	DIR* newDir;
	char* newFolderName;

	short int nameSize;
	int fileSize;
	int filedes;
	int loop;

	newFolderName = new char[strlen(folderName) + 30];

	struct stat st;

	char* buffer = new char[b];

	char* writeBuf;
//	char zero[2];
//	zero[1] = '0';
//	zero[0] = '0';
	////////////////////////PROSPELASI STA PERIEXOMENA TOU FAKELOU
	while((inputRecord = readdir(dir)) != NULL){
    		if(strcmp(inputRecord->d_name, ".") !=0 && strcmp(inputRecord->d_name, "..") !=0){
			strcpy(newFolderName, folderName);
			strcat(newFolderName, "/");
			strcat(newFolderName, inputRecord->d_name);
			if((newDir = opendir(newFolderName)) != NULL){
				cout << "mpike se fakelo" << endl;
				////////////////ANADROMI EAN EXEI FAKELO
				writeDir(newDir, newFolderName, client, fifoId, b, logFile); 
          		}else{
				writeBuf = new char[strlen(folderName) + strlen(inputRecord->d_name) + 5];
				strcpy(writeBuf, folderName);
				strcat(writeBuf, "/");
				strcat(writeBuf,inputRecord->d_name);
				nameSize = strlen(writeBuf);
				write(fifoId,&nameSize, 2);
                		write(fifoId, writeBuf, strlen(writeBuf)+1);
                        	cout << "i am writing "<< writeBuf << " with nameSize"<< nameSize <<  endl;

				stat(writeBuf, &st);
        			fileSize = st.st_size;
				write(fifoId,&fileSize, 4);
				/////TA LOGS
				logFile << "write"<< " " << client << " " << writeBuf << " " << fileSize << endl;
				if ((filedes=open(writeBuf ,O_RDWR,PERMS))==-1){
                        		perror("opening .id file");
                        		exit(1);
                		}
				/////////GRAFW TO PERIEXOMENO
                		loop = fileSize/b;
            			loop = loop+1; 
				int mod = fileSize%b;
                		for(int j=0; j < loop; j++){
					strcpy(buffer, "");

					if(j ==loop-1){
						read(filedes,buffer,mod);
                                        	write(fifoId,buffer,mod);
					}else{
						read(filedes,buffer,b);
                	                	write(fifoId, buffer,b);
					}
                		}
                		close(filedes);


				delete[] writeBuf;
			}
                }
        }
//	write(fifoId,zero, 2);
	delete[] buffer;
	delete[] newFolderName;
	cout << "done writing " << endl;

}
