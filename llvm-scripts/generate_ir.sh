#!/bin/bash
# Упрощённая версия: generate_ir_simple.sh

if [ $# -eq 0 ]; then
    echo "Использование: $0 <file.cpp> [output.ll]"
    exit 1
fi

INPUT_FILE=$1
OUTPUT_FILE=${2:-${INPUT_FILE%.cpp}.ll}

# Проверка clang++
if ! command -v clang++ &> /dev/null; then
    echo "Ошибка: clang++ не найден. Установите LLVM/clang"
    exit 1
fi

# Генерация LLVM IR
echo "Генерация LLVM IR для $INPUT_FILE -> $OUTPUT_FILE"
clang++ -S -emit-llvm -O0 "$INPUT_FILE" -o "$OUTPUT_FILE"

if [ $? -eq 0 ]; then
    echo "Успешно создан: $OUTPUT_FILE"
    echo "Размер: $(wc -l < "$OUTPUT_FILE") строк"
else
    echo "Ошибка при компиляции"
    exit 1
fi
