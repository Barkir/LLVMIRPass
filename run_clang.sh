../build-clang2/bin/clang -O0 -Xclang -disable-O0-optnone -S -emit-llvm ./CollapseIdenticalNodesTests/cpp/test_stack_overflow.cpp -o test_stack_overflow.ll
