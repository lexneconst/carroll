cmd_/home/lexne/casenium_carroll/driver/ctl10/../../bin/driver/xctl10.mod := printf '%s\n'   ctl10.o gen10.o bcd10.o | awk '!x[$$0]++ { print("/home/lexne/casenium_carroll/driver/ctl10/"$$0) }' > /home/lexne/casenium_carroll/driver/ctl10/../../bin/driver/xctl10.mod
