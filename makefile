all:clean compile link run

link:
	g++ -o bin/warehouse bin/main.o  bin/Order.o bin/Customer.o bin/Volunteer.o bin/CollectorVolunteer.o bin/DriverVolunteer.o bin/WareHouse.o bin/Action.o

compile:
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/main.o src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Order.o src/Order.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Customer.o src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Volunteer.o src/Volunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/CollectorVolunteer.o src/CollectorVolunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/WareHouse.o src/WareHouse.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/DriverVolunteer.o src/DriverVolunteer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -Iinclude -c -o bin/Action.o src/Action.cpp


clean:
	rm -f ./bin/*.o ./bin/warehouse



run:
	valgrind --leak-check=full --show-reachable=yes ./bin/warehouse ./bin/rest/input_file.txt
