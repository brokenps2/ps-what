define target remote
target extended-remote $arg0
symbol-file bin/prog.elf
monitor reset shellhalt
load bin/prog.elf
end
