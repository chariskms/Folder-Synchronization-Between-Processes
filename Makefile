compile: mirror_main.cpp list.cpp readDir.cpp writeDir.cpp
	 g++ -o mirror_client mirror_main.cpp list.cpp readDir.cpp writeDir.cpp

clean:
	rm mirror_client
