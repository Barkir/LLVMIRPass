This is a research about this example ([previous part of the research](./README_old.md))

## Source code
```cpp
int test1(int a, int b, int c) {
	if (a == 46346 && b == 5745 && c == 5)
		return a * b / c;

return a * b / c;
}
```
## Compilation commands

compile it with `clang` using this command and then use `opt`:
```
clang++ -S -emit-llvm -O0 -Xclang -disable-O0-optnone mul.cpp -o mulb4.ll
```

```
opt -S -O2 mulb4.ll -o mulb4O2.ll -print-after-all &> mulb4_after_all.txt
```

## Current result

get this result at the end:
```cpp
define dso_local noundef i32 @_Z5test1iii(i32 noundef %a, i32 noundef %b, i32 noundef %c) local_unnamed_addr #0 {
entry:
  %cmp = icmp eq i32 %a, 46346
  %cmp1 = icmp eq i32 %b, 5745
  %or.cond = and i1 %cmp, %cmp1
  %cmp3 = icmp eq i32 %c, 5
  %or.cond1 = and i1 %or.cond, %cmp3
  br i1 %or.cond1, label %return, label %if.end

if.end:                                           ; preds = %entry
  %mul4 = mul nsw i32 %b, %a
  %div5 = sdiv i32 %mul4, %c
  br label %return

return:                                           ; preds = %entry, %if.end
  %retval.0 = phi i32 [ %div5, %if.end ], [ 53251554, %entry ]
  ret i32 %retval.0
}

```
## The result we actually want

instead we want to get this ->
```cpp
define dso_local noundef i32 @_Z5test1iii(i32 noundef %a, i32 noundef %b, i32 noundef %c) local_unnamed_addr #0 {

  %mul4 = mul nsw i32 %b, %a
  %div5 = sdiv i32 %mul4, %c
  ret i32 %div5
}

```
