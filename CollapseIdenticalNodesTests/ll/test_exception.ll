; ModuleID = './CollapseIdenticalNodesTests/cpp/test_exception.cpp'
source_filename = "./CollapseIdenticalNodesTests/cpp/test_exception.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZTIi = external constant ptr

; Function Attrs: mustprogress noinline uwtable
define dso_local noundef i32 @_Z20test_exception_pathsi(i32 noundef %a) #0 personality ptr @__gxx_personality_v0 {
entry:
  %a.addr = alloca i32, align 4
  %res = alloca i32, align 4
  %exn.slot = alloca ptr, align 8
  %ehselector.slot = alloca i32, align 4
  store i32 %a, ptr %a.addr, align 4
  %0 = load i32, ptr %a.addr, align 4
  %cmp = icmp eq i32 %0, 42
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  store i32 100, ptr %res, align 4
  %exception = call ptr @__cxa_allocate_exception(i64 4) #1
  store i32 1, ptr %exception, align 16
  invoke void @__cxa_throw(ptr %exception, ptr @_ZTIi, ptr null) #2
          to label %unreachable unwind label %lpad

lpad:                                             ; preds = %if.then
  %1 = landingpad { ptr, i32 }
          catch ptr null
  %2 = extractvalue { ptr, i32 } %1, 0
  store ptr %2, ptr %exn.slot, align 8
  %3 = extractvalue { ptr, i32 } %1, 1
  store i32 %3, ptr %ehselector.slot, align 4
  br label %catch

catch:                                            ; preds = %lpad
  %exn = load ptr, ptr %exn.slot, align 8
  %4 = call ptr @__cxa_begin_catch(ptr %exn) #1
  store i32 100, ptr %res, align 4
  call void @__cxa_end_catch()
  br label %try.cont

try.cont:                                         ; preds = %catch, %if.end
  %5 = load i32, ptr %res, align 4
  ret i32 %5

if.else:                                          ; preds = %entry
  store i32 100, ptr %res, align 4
  br label %if.end

if.end:                                           ; preds = %if.else
  br label %try.cont

unreachable:                                      ; preds = %if.then
  unreachable
}

declare ptr @__cxa_allocate_exception(i64)

declare void @__cxa_throw(ptr, ptr, ptr)

declare i32 @__gxx_personality_v0(...)

declare ptr @__cxa_begin_catch(ptr)

declare void @__cxa_end_catch()

attributes #0 = { mustprogress noinline uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind }
attributes #2 = { noreturn }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 22.0.0git (https://github.com/llvm/llvm-project 78636be4d6ff5be00d7ec0a599eb9f5daa391bc9)"}
