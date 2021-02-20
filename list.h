
struct listNode{
        int id;
        listNode* next;
};

class list{
	listNode* start;
	listNode* end;
	public:
		list();
		~list();
		bool search(int);
		int insertClient(int);
		void printList();
		int deleteClient(int);
		int compare(list*);
};

