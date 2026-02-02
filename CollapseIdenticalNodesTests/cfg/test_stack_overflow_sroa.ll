define dso_local noundef i32 @_Z15test_cyclic_phiii(i32 noundef %n, i32 noundef %init) #0 {
entry:
  %cmp = icmp eq i32 %init, 1
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %entry
  br label %for.cond

for.cond:                                         ; preds = %for.body, %if.then
  %x.0 = phi i32 [ 0, %if.then ], [ %y.0, %for.body ]
  %y.0 = phi i32 [ 1, %if.then ], [ %add, %for.body ]
  %i.0 = phi i32 [ 0, %if.then ], [ %inc, %for.body ]
  %cmp1 = icmp slt i32 %i.0, %n
  br i1 %cmp1, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %add = add nsw i32 %x.0, %y.0
  %inc = add nsw i32 %i.0, 1
  br label %for.cond, !llvm.loop !6

for.end:                                          ; preds = %for.cond
  br label %if.end

if.else:                                          ; preds = %entry
  br label %if.end

if.end:                                           ; preds = %if.else, %for.end
  %result.0 = phi i32 [ %x.0, %for.end ], [ 0, %if.else ]
  ret i32 %result.0
}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 22.0.0git (https://github.com/llvm/llvm-project 78636be4d6ff5be00d7ec0a599eb9f5daa391bc9)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
