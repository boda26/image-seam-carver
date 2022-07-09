CXX=clang++
INCLUDES=-Iincludes/
CXX_FLAGS=-std=c++20 -g -fstandalone-debug -Wall -Wextra -Werror -pedantic -fsanitize=address,null -fno-omit-frame-pointer $(INCLUDES)

exec: bin/exec

bin/exec: src/main.cc src/seam_carver.cc src/image_ppm.cc includes/image_ppm.hpp includes/seam_carver.hpp
	${CXX} ${CXX_FLAGS} src/main.cc src/seam_carver.cc src/image_ppm.cc -o bin/exec

obj/catch.o: tests/catch.cc
	$(CXX) $(CXX_FLAGS) -c $^ -o $@

.DEFAULT_GOAL := exec
.PHONY: clean exec

clean:
	rm -fr bin/* obj/*