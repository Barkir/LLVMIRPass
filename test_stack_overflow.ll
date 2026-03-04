; ModuleID = './CollapseIdenticalNodesTests/cpp/test_stack_overflow.cpp'
source_filename = "./CollapseIdenticalNodesTests/cpp/test_stack_overflow.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local noundef i32 @_Z15test_cyclic_phiii(i32 noundef %n, i32 noundef %init) #0 {
entry:
  %n.addr = alloca i32, align 4
  %init.addr = alloca i32, align 4
  %result = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %i = alloca i32, align 4
  %temp = alloca i32, align 4
  store i32 %n, ptr %n.addr, align 4
  store i32 %init, ptr %init.addr, align 4
  %0 = load i32, ptr %init.addr, align 4
  %cmp = icmp eq i32 %0, 1
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  store i32 0, ptr %x, align 4
  store i32 1, ptr %y, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.then
  %1 = load i32, ptr %i, align 4
  %2 = load i32, ptr %n.addr, align 4
  %cmp1 = icmp slt i32 %1, %2
  br i1 %cmp1, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %3 = load i32, ptr %x, align 4
  store i32 %3, ptr %temp, align 4
  %4 = load i32, ptr %y, align 4
  store i32 %4, ptr %x, align 4
  %5 = load i32, ptr %temp, align 4
  %6 = load i32, ptr %y, align 4
  %add = add nsw i32 %5, %6
  store i32 %add, ptr %y, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %7 = load i32, ptr %i, align 4
  %inc = add nsw i32 %7, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
  %8 = load i32, ptr %x, align 4
  store i32 %8, ptr %result, align 4
  br label %if.end

if.else:                                          ; preds = %entry
  store i32 0, ptr %result, align 4
  br label %if.end

if.end:                                           ; preds = %if.else, %for.end
  %9 = load i32, ptr %result, align 4
  ret i32 %9
}

attributes #0 = { mustprogress noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 22.0.0git (https://github.com/llvm/llvm-project 78636be4d6ff5be00d7ec0a599eb9f5daa391bc9)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
