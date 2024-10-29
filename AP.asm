; Code to store AP by anshurup 2301ai02
main: ldc len
    a2sp
    ldc a
AP: 
    sp2a
    adc -1

    brlz exit

    a2sp
    ldc d
    add
    br AP
exit: HALT

len: SET 5
a: SET 5
d: SET 10