nasm -f win64 security_bypass.asm
link /subsystem:console /nodefaultlib /debug:full /entry:main security_bypass.obj kernel32.lib
