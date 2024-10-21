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

x: data 0
y: data 0

main:
    ldc array
    a2sp
    ldl 0
    br exit
    ldl 1
    ldl 2
exit: HALT