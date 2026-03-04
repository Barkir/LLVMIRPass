; ModuleID = './CollapseIdenticalNodesTests/cpp/test_simd.cpp'
source_filename = "./CollapseIdenticalNodesTests/cpp/test_simd.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: mustprogress noinline nounwind uwtable
define dso_local noundef i32 @_Z19test_vector_compareDv4_ii(<4 x i32> noundef %a, i32 noundef %b) #0 {
entry:
  %retval = alloca i32, align 4
  %a.addr = alloca <4 x i32>, align 16
  %b.addr = alloca i32, align 4
  %cmp = alloca <4 x i32>, align 16
  %.compoundliteral = alloca <4 x i32>, align 16
  %reduced = alloca i32, align 4
  store <4 x i32> %a, ptr %a.addr, align 16
  store i32 %b, ptr %b.addr, align 4
  %0 = load <4 x i32>, ptr %a.addr, align 16
  store <4 x i32> splat (i32 5), ptr %.compoundliteral, align 16
  %1 = load <4 x i32>, ptr %.compoundliteral, align 16
  %cmp1 = icmp eq <4 x i32> %0, %1
  %sext = sext <4 x i1> %cmp1 to <4 x i32>
  store <4 x i32> %sext, ptr %cmp, align 16
  %2 = load <4 x i32>, ptr %cmp, align 16
  %vecext = extractelement <4 x i32> %2, i32 0
  %3 = load <4 x i32>, ptr %cmp, align 16
  %vecext2 = extractelement <4 x i32> %3, i32 1
  %and = and i32 %vecext, %vecext2
  %4 = load <4 x i32>, ptr %cmp, align 16
  %vecext3 = extractelement <4 x i32> %4, i32 2
  %and4 = and i32 %and, %vecext3
  %5 = load <4 x i32>, ptr %cmp, align 16
  %vecext5 = extractelement <4 x i32> %5, i32 3
  %and6 = and i32 %and4, %vecext5
  store i32 %and6, ptr %reduced, align 4
  %6 = load i32, ptr %reduced, align 4
  %tobool = icmp ne i32 %6, 0
  br i1 %tobool, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  store i32 100, ptr %retval, align 4
  br label %return

if.end:                                           ; preds = %entry
  %7 = load i32, ptr %b.addr, align 4
  %cmp7 = icmp eq i32 %7, 5
  br i1 %cmp7, label %if.then8, label %if.end9

if.then8:                                         ; preds = %if.end
  store i32 100, ptr %retval, align 4
  br label %return

if.end9:                                          ; preds = %if.end
  store i32 100, ptr %retval, align 4
  br label %return

return:                                           ; preds = %if.end9, %if.then8, %if.then
  %8 = load i32, ptr %retval, align 4
  ret i32 %8
}

attributes #0 = { mustprogress noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="128" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 22.0.0git (https://github.com/llvm/llvm-project 78636be4d6ff5be00d7ec0a599eb9f5daa391bc9)"}
