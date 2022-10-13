CXX_FLAGS = -D _DEBUG -g -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++   -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE
ASSAN = -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

CXX_FLAGS += $(ASSAN)

all: DO



DO: DO_ASM DO_PROC DO_DISASM

DO_PROC: OBJECTS/m_proc.o OBJECTS/proc.o OBJECTS/stack.o
	g++ OBJECTS/m_proc.o OBJECTS/proc.o OBJECTS/stack.o -o proc $(CXX_FLAGS)

OBJECTS/m_proc.o: m_proc.cpp
	g++ -c -o OBJECTS/m_proc.o m_proc.cpp $(CXX_FLAGS)

OBJECTS/proc.o: proc.cpp
	g++ -c -o OBJECTS/proc.o proc.cpp $(CXX_FLAGS)



OBJECTS/stack.o: stack.cpp
	g++ -c -o OBJECTS/stack.o stack.cpp $(CXX_FLAGS)



DO_ASM: OBJECTS/m_asm.o OBJECTS/asm.o OBJECTS/stack.o
	g++ OBJECTS/m_asm.o OBJECTS/asm.o OBJECTS/stack.o -o asm $(CXX_FLAGS)

OBJECTS/m_asm.o: m_asm.cpp
	g++ -c -o OBJECTS/m_asm.o m_asm.cpp $(CXX_FLAGS)

OBJECTS/asm.o: asm.cpp
	g++ -c -o OBJECTS/asm.o asm.cpp $(CXX_FLAGS)



DO_DISASM: OBJECTS/m_disasm.o OBJECTS/disasm.o OBJECTS/stack.o
	g++ OBJECTS/m_disasm.o OBJECTS/disasm.o OBJECTS/stack.o -o disasm $(CXX_FLAGS)

OBJECTS/m_disasm.o: m_disasm.cpp
	g++ -c -o OBJECTS/m_disasm.o m_disasm.cpp $(CXX_FLAGS)

OBJECTS/disasm.o: disasm.cpp
	g++ -c -o OBJECTS/disasm.o disasm.cpp $(CXX_FLAGS)



clean:
	cd OBJECTS; rm -rf *.o; cd -
