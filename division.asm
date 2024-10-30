;code to perfectly divide b / a by Anshurup 2301ai02
; only positive a and b

b: SET 21
a: SET 3
result: data 0

main:
    ldc b
    ; starting to floor divide
divide:
    brlz exit
    brz exit_zero
    ldc a
    sub
    adj +1
    br divide
exit: 
    adj -1
exit_zero:
    sp2a
    ldc result
    a2sp
    stl 0
HALT