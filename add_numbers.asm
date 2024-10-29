; Code to add numbers by anshurup 2301ai02
result: data 0
main: ldc len
    adc -1
    a2sp
add_loop: sp2a
    brz exit
    a2sp
    ldl array
    add
    adj -1
    br add_loop
exit: 
    a2sp
    ldc result
    stnl 0
    HALT
len: SET 10
array: data 10
    data 9
    data 8
    data 7
    data 6
    data 5
    data 4
    data 3
    data 2
    data 1