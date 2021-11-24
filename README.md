# Assembly and C study guide

## Assembly basics
### GDB
```gdb
gdb hello
list
run
set disassembly-flavor intel
disassemble main
break main
info registers
```

/usr/bin/valgrind --tool=callgrind --dump-instr=yes /home/lxm/Documents/Code/redis-study/cmake-build-debug/redis_study
kcachegrind callgrind.out.286469

### Reading materials
`http://highscalability.com/blog/2014/9/8/how-twitter-uses-redis-to-scale-105tb-ram-39mm-qps-10000-ins.html`

####TODO
- Insertion is still 5 times slower than that of REDIS