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
#include <signal.h>
#define PERMS 0666


using namespace std;
int readFlag;
////////sinartisi gia to viii
void expired(int signum){
	if(readFlag == 0){
		cout << "time for input expired" << endl;
		kill(getppid(),SIGUSR1);
		exit(0);
	}
}


void readDir(DIR* dir, char* mirrorName, char* client, int fifoId, int b, fstream& logFile){

	DIR* newDir;
	char temp[50];
	char temp2[50];
	strcpy(temp, mirrorName);
	strcat(temp, "/");
	strcat(temp, client);
	//cout << "make in the mirror " << temp << endl;
	mkdir(temp, EEXIST | S_IRWXU | S_IRWXG | S_IRWXO);
	cout << "make in the mirror " << temp << endl;
	strcat(temp, "/");
	strcpy(temp2, temp);

	short int nameSize;
	int fileSize;
	int filedes;
	int loop;

	char* buffer = new char[b];
	char* readBuf;
	char* filePath;
	char* mirrorPath;
	char* zero;
	//zero = new char[2];
	readFlag = 0;
	alarm(30);
	signal(SIGALRM,expired);
	read(fifoId,&nameSize, 2);
	readFlag = 1;
	////// DIAVAZEI MEXRI NA PAREI 00 
	zero = (char*)&nameSize;
	while(zero[0] != '0' || zero[1] != '0'){
		readBuf = new char[nameSize+1];
		filePath = new char[nameSize+1];
		mirrorPath = new char[nameSize+10];
		cout << "name size "<< nameSize << endl;
		read(fifoId, readBuf, nameSize+1);
		strcpy(filePath, readBuf);

		int i=0;
		int slashes =0;
		/////FTIAXNW TA PATHS KAI TOUS FAKELOUS
                while(readBuf[i] != '\0'){
                        if(readBuf[i] == '/'){
                                slashes++;
                                if(slashes == 2){
                                        strncpy(readBuf,readBuf+i+1, strlen(readBuf+i) +1);
                                        break;
                                }
                        }
                        i++;
                }

		slashes =0;
                i = 0;
		while(readBuf[i] != '\0'){

			if(readBuf[i] == '/'){
				strncpy(filePath, readBuf,i);
                                filePath[i] = '\0';
				strcpy(mirrorPath, temp);
				strcat(mirrorPath,filePath);
				cout << "make dir " << mirrorPath << endl;
				mkdir(mirrorPath, EEXIST | S_IRWXU | S_IRWXG | S_IRWXO);

			}
			i++;
		}
		strcpy(mirrorPath, temp);
		strcat(mirrorPath,readBuf);
		////////////////////////////////////////
		cout << "file gia na ftia3ei "<<mirrorPath << endl;
		if ((filedes=open(mirrorPath ,O_CREAT,PERMS))==-1){
                	perror("creating file");
                	exit(1);
        	}

	        close(filedes);
        	if ((filedes=open(mirrorPath ,O_RDWR,PERMS))==-1){
                	perror("opening file");
                	exit(1);
        	}
		read(fifoId,&fileSize, 4);
		///////////////////ANTIGRAFW TO PERIEXOMENO
		loop = fileSize/b;
		loop = loop + 1;
		int mod = fileSize%b;
		for(int j=0; j < loop; j++){
			strcpy(buffer, "");
			if(j == loop -1){
				read(fifoId, buffer, mod);
                       		write(filedes,buffer,mod);
			}else{
				read(fifoId, buffer, b);
                                write(filedes,buffer,b);
			}
		}
		close(filedes);

		logFile << "read"<< " " << client << " " << mirrorPath << " " << fileSize << endl;
		//logFile << nameSize;
        	read(fifoId, &nameSize, 2);
		zero = (char*)&nameSize;
                cout << "I am reading nameSize : " << nameSize << " " << zero << endl;
		delete[] readBuf;
        	delete[] filePath;
        	delete[] mirrorPath;
	}
	cout << "done reading" << endl;
	delete[] buffer;
//	delete[] zero;
//	return 0;
}
