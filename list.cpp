#include <iostream>
#include "list.h"

using namespace std;

list::list(){
	this -> start = NULL;
	this -> end = NULL;
}

list::~list(){
	listNode* temp = start;
        while(temp != NULL){
       		this -> deleteClient(temp -> id);
        	temp = temp -> next;
        }
}

bool list::search(int id){
	listNode* temp = this -> start;
	while(temp != NULL){
		if(temp -> id == id){
			return true;
		}
		temp = temp -> next;
	}
	return false;
}

void list::printList(){
	listNode* temp = start;
        while(temp != NULL){
           	cout << temp -> id << endl;
                temp = temp -> next;
        }
}

int list::insertClient(int id){
	if(this -> start == NULL){
		this -> start = new listNode;
		this -> start -> next = NULL;
		this -> end = this -> start;
		this -> start -> id = id; 
	}else{
		this -> end -> next = new listNode;
		this -> end -> next -> next = NULL;
		this -> end -> next -> id = id;
		this -> end = this -> end -> next;
	}
}

int list::deleteClient(int id){
	listNode* temp = this -> start;
	listNode* previousTemp = this -> start;
        while(temp != NULL){
        	if(temp -> id == id){
			if(this -> start == temp){
				this -> start = temp -> next;
				if(this -> end == temp){
					this -> end = temp -> next;
				}
				delete temp;
				return 1;
			}
			if(this -> end == temp){
                                 this -> end = previousTemp;
                        }
			previousTemp -> next = temp -> next;
                        delete temp;
			return 1;
                }
		if(this -> start != temp){ 
			previousTemp = previousTemp -> next;
                     	temp = temp -> next;
		}else{
			temp = temp -> next;
		}
         }
        return -1;

}

int list::compare(list* list2){
	listNode* temp = start;
        while(temp != NULL){
                if(!(list2 -> search(temp -> id))){
			return (temp -> id);
		}
                temp = temp -> next;
        }
	return -1;
}
