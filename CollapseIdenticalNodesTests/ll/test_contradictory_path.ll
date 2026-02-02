; ModuleID = './CollapseIdenticalNodesTests/cpp/test_contradictory_path.cpp'
source_filename = "./CollapseIdenticalNodesTests/cpp/test_contradictory_path.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local noundef i32 @_Z30test_contradictory_constraintsi(i32 noundef %a) #0 {
entry:
  %retval = alloca i32, align 4
  %a.addr = alloca i32, align 4
  %c1 = alloca i8, align 1
  %c2 = alloca i8, align 1
  %both = alloca i8, align 1
  store i32 %a, ptr %a.addr, align 4
  %0 = load i32, ptr %a.addr, align 4
  %cmp = icmp eq i32 %0, 5
  %storedv = zext i1 %cmp to i8
  store i8 %storedv, ptr %c1, align 1
  %1 = load i32, ptr %a.addr, align 4
  %cmp1 = icmp eq i32 %1, 6
  %storedv2 = zext i1 %cmp1 to i8
  store i8 %storedv2, ptr %c2, align 1
  %2 = load i8, ptr %c1, align 1
  %loadedv = trunc i8 %2 to i1
  br i1 %loadedv, label %cond.true, label %cond.false

cond.true:                                        ; preds = %entry
  %3 = load i8, ptr %c2, align 1
  %loadedv3 = trunc i8 %3 to i1
  br label %cond.end

cond.false:                                       ; preds = %entry
  br label %cond.end

cond.end:                                         ; preds = %cond.false, %cond.true
  %cond = phi i1 [ %loadedv3, %cond.true ], [ false, %cond.false ]
  %storedv4 = zext i1 %cond to i8
  store i8 %storedv4, ptr %both, align 1
  %4 = load i8, ptr %both, align 1
  %loadedv5 = trunc i8 %4 to i1
  br i1 %loadedv5, label %if.then, label %if.end

if.then:                                          ; preds = %cond.end
  store i32 10, ptr %retval, align 4
  br label %return

if.end:                                           ; preds = %cond.end
  store i32 10, ptr %retval, align 4
  br label %return

return:                                           ; preds = %if.end, %if.then
  %5 = load i32, ptr %retval, align 4
  ret i32 %5
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
