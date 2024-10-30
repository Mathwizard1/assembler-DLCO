; Code to store Fibonacci by anshurup 2301ai02

main:
    ldc result
    a2sp
    sp2a
    stl 0

Fibonacci:
    ldc index
    a2sp
    ldl 0

    ; all terms saved
    brlz exit

    ldc a
    a2sp
    ldl 0

    adj +1
    ldl 0

    add

    ldl 0
    adj -1
    stl 0

    adj +1
    stl 0

    ; save terms after result
    ldc result
    a2sp
    ldl 0
    adc 1
    a2sp
    stl 0

    sp2a
    ldc result
    a2sp
    stl 0

    ldc index
    a2sp
    ldl 0
    adc -1
    stl 0

    br Fibonacci
exit: HALT

index: data 12
a: data 0
b: data 1
result: data 0