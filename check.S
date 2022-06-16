    .text
    .global trap_and_check
trap_and_check:
    break   0

    bne     $r1, $r2, .Lsegv
    bne     $r2, $r3, .Lsegv
    bne     $r3, $r4, .Lsegv
    bne     $r4, $r5, .Lsegv
    bne     $r5, $r6, .Lsegv
    bne     $r6, $r7, .Lsegv
    bne     $r7, $r8, .Lsegv
    bne     $r8, $r9, .Lsegv
    bne     $r9, $r10, .Lsegv
    bne     $r10, $r11, .Lsegv
    bne     $r11, $r12, .Lsegv
    bne     $r12, $r13, .Lsegv
    bne     $r13, $r14, .Lsegv
    bne     $r14, $r15, .Lsegv
    bne     $r15, $r16, .Lsegv
    bne     $r16, $r17, .Lsegv
    bne     $r17, $r18, .Lsegv
    bne     $r18, $r19, .Lsegv
    bne     $r19, $r20, .Lsegv
    bne     $r20, $r21, .Lsegv
    bne     $r21, $r22, .Lsegv
    bne     $r22, $r23, .Lsegv
    bne     $r23, $r24, .Lsegv
    bne     $r24, $r25, .Lsegv
    bne     $r25, $r26, .Lsegv
    bne     $r26, $r27, .Lsegv
    bne     $r27, $r28, .Lsegv
    bne     $r28, $r29, .Lsegv
    bne     $r29, $r30, .Lsegv
    bne     $r30, $r31, .Lsegv
    li.d    $r1, 0xdeadbeefdeadbeef
    bne     $r31, $r1, .Lsegv

.Lexit:
    li.w    $a0, 0
    li.w    $a7, 93 // NR_exit
    syscall 0

.Lsegv:
    ld.w    $zero, $zero, 0
