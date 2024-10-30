; Code to square a number by anshurup gupta 2301ai02
; only positive number

num: SET 16
result: data 0
main:
    ldc num
    a2sp
    ldc num
    sp2a
square:
    adj -1
    sp2a
    brz exit
    a2sp
    ldc num
    add
    br square
exit:
    a2sp
    ldc result
    stnl 0
    HALT