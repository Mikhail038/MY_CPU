CXX_FLAGS = -D _DEBUG -g -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr  -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE



all: DO



DO: DO_ASM DO_PROC

DO_PROC: O/m_proc.o O/proc.o O/stack.o
	g++ O/m_proc.o O/proc.o O/stack.o -o proc $(CXX_FLAGS)

O/m_proc.o: m_proc.cpp
	g++ -c -o O/m_proc.o m_proc.cpp $(CXX_FLAGS)

O/proc.o: proc.cpp
	g++ -c -o O/proc.o proc.cpp $(CXX_FLAGS)



O/stack.o: stack.cpp
	g++ -c -o O/stack.o stack.cpp $(CXX_FLAGS)



DO_ASM: O/m_asm.o O/asm.o O/stack.o
	g++ O/m_asm.o O/asm.o O/stack.o -o asm $(CXX_FLAGS)

O/m_asm.o: m_asm.cpp
	g++ -c -o O/m_asm.o m_asm.cpp $(CXX_FLAGS)

O/asm.o: asm.cpp
	g++ -c -o O/asm.o asm.cpp $(CXX_FLAGS)





clean:
	cd O; rm -rf *.o; cd -
