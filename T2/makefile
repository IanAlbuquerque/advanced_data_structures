
compile:
	clear
	@echo Compiling...
	@g++ -o -Wall -o grafos grafos.cpp CPUTimer/CPUTimer.cpp
	@echo DONE!

tests: compile
	for F in testes/*.clq ; do \
		echo Running Test For $$F ; \
		./grafos < $$F > $(addprefix output/,$$F) ; \
	done
