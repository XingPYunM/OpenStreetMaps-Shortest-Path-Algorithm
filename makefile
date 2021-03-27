build:
	rm -f application.exe
	g++ -std=c++11 -Wall application.cpp dist.cpp osm.cpp tinyxml2.cpp -o application.exe

run:
	./application.exe

buildtest:
	rm -f testing.exe
	g++ -std=c++11 -Wall testing.cpp -o testing.exe

runtest:
	./testing.exe

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./application.exe
	
app:
	rm -f application.exe
	g++ -std=c++11 -Wall application.cpp dist.cpp osm.cpp tinyxml2.cpp -o application.exe
	./application.exe

app_input:
	rm -f application.exe
	g++ -std=c++11 -Wall application.cpp dist.cpp osm.cpp tinyxml2.cpp -o application.exe
	./application.exe < input.txt

val_test:
	rm -f application.exe
	g++ -std=c++11 -Wall application.cpp dist.cpp osm.cpp tinyxml2.cpp -o application.exe
	valgrind --tool=memcheck --leak-check=yes ./application.exe

b_test:
	rm -f testing.exe
	g++ -std=c++11 -Wall testing.cpp -o testing.exe
	./testing.exe

bench:
	rm -f testbench.exe
	g++ testbench.cpp -o testbench.exe -lgtest -lgtest_main -lpthread
	./testbench.exe
