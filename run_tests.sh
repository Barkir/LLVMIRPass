#!/bin/bash

test_directory_cpp="./CollapseIdenticalNodesTests/cpp"
test_directory_ll="./CollapseIdenticalNodesTests/ll"
test_directory_opt="./CollapseIdenticalNodesTests/ll_opt"
test_directory_dump="./CollapseIdenticalNodesTests/dump_after_all"

RED='\e[31m'
BLU='\e[34m'
GRN='\e[32m'
DEF='\e[0m'

passed=()
failed=()
overall=()

run_clang_function() {
    ../build-clang2/bin/clang -O0 -Xclang -disable-O0-optnone -S -emit-llvm "$1" -o "$2"
}

run_opt_function() {
    cmd='../build-llvm/bin/opt -S -O2 "$1" -o "$2" -print-after-all &> "$3"'
    if eval "$cmd"; then
        passed+=("$1")
        echo -e "${GRN}PASSED ✅${DEF}"
    else
        failed+=("$1")
        echo -e "${RED}FAILED 🪦${DEF}"
    fi
}


for filepath in "$test_directory_cpp"/*; do
    filename=$(basename "$filepath" .cpp)
    ll_filepath="$test_directory_ll/$filename.ll"
    opt_filepath="$test_directory_opt/${filename}O2.ll"
    dump_filepath="$test_directory_dump/${filename}.txt"
    run_clang_function "$filepath" "$ll_filepath"
    run_opt_function "$ll_filepath" "$opt_filepath" "$dump_filepath"
    overall+=(ll_filepath)

done

counter=1
echo -e "${BLU}==OVERALL==${DEF}"
echo -e "failed ${#failed[@]}/${#overall[@]}"
echo -e "${RED}----------------------F A I L E D------------------------------${DEF}"

for test in "${failed[@]}"; do
    echo "#$counter : $(basename "$test" .cpp)"
    ((counter++))
done

echo -e "${RED}---------------------------------------------------------------${DEF}"
echo -e "${BLU}/ / / / / / / / / / / / / / / / / / / / / / / / / / / / / / / /${DEF}"
echo -e "${GRN}----------------------P A S S E D------------------------------${DEF}"
counter=1
for test in "${passed[@]}"; do
    echo "#$counter : $(basename "$test" .cpp)"
    ((counter++))
done
echo -e "${GRN}---------------------------------------------------------------${DEF}"
