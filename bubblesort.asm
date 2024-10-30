; Bubble sort by Anshurup 2301ai02
; sorts in ascending order
result: data -2
        data -10
        data 4
        data 1
        data 10
        data 503
        data 4
len: SET 7

; index
i: data 0
j: data 0

; main
main: ldc len
    adc -1

    ; set i
    ldc i
    stnl 0

; bubble sorting
sort: ldc i
    ldnl 0

    ; when i < 0
    brlz exit

    ;set j = i
    ldc j
    stnl 0

inner_loop: ldc j
    ldnl 0

    ; j < 1
    brz exit_innerloop

    a2sp
    ;adj result
    adj -1
    ldl 0
    adj 1
    ldl 0

    sub

    ; if arr[j] > arr[j - 1] else br continue
    brlz continue

    ldc j
    ldnl 0
    a2sp
    ;adj result
    adj -1
    ldl 0
    adj 1
    ldl 0

    ; swap
    adj -1
    stl 0
    adj 1
    stl 0
continue: ldc j
    a2sp
    ldl 0

    ;j = j -1
    adc -1
    stl 0
    br inner_loop

exit_innerloop: ldc i
    a2sp
    ldl 0
    ; i = i - 1
    adc -1
    stl 0
    br sort
    
exit: HALT

