all:
	g++ -Wall FinanCalc.cpp -o finan

run:
	./finan

try: all run
