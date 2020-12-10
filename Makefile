CXXFLAGS = -O3
CXXFLAGS_DEBUG = -fsanitize=address -mfpmath=sse -fstack-protector-all -g -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -Woverloaded-virtual -Wnon-virtual-dtor -Wcast-qual -Wno-suggest-attribute=format

B_TREE_PATH = ./b_tree
STUDENT_PATH = ./student


%.o: %.cpp
	g++ $(CXXFLAGS) -c $< -o $@
%_debug.o: %.cpp
	g++ $(CXXFLAGS_DEBUG) -c $< -o $@


all: test.out test_debug.out

test.out: main.o $(STUDENT_PATH)/student.o
	g++ $(CXXFLAGS) $^ -o $@
test_debug.out: main_debug.o $(STUDENT_PATH)/student_debug.o
	g++ $(CXXFLAGS_DEBUG) $^ -o $@

main.o: main.cpp $(STUDENT_PATH)/student.h $(B_TREE_PATH)/b_tree.h $(B_TREE_PATH)/add_value.h $(B_TREE_PATH)/print.h $(B_TREE_PATH)/calculate.h
main_debug.o: main.cpp $(STUDENT_PATH)/student.h $(B_TREE_PATH)/b_tree.h $(B_TREE_PATH)/add_value.h $(B_TREE_PATH)/print.h $(B_TREE_PATH)/calculate.h
$(STUDENT_PATH)/student.o: $(STUDENT_PATH)/student.cpp $(STUDENT_PATH)/student.h
$(STUDENT_PATH)/student_debug.o: $(STUDENT_PATH)/student.cpp $(STUDENT_PATH)/student.h

mrproper: mrproper_current mrproper_student
mrproper_current:
	rm -f *.o *.out
mrproper_student: $(STUDENT_PATH)
	rm -f $</*.o
