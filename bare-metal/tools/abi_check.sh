#!/bin/sh
# Verify the shared mailbox geometry on the host compiler.  If a 32-bit
# compiler is available, repeat the check there too: the A55 is 64-bit and the
# safety cores are 32-bit, so the packed layout must not change.
set -eu

cd "$(dirname "$0")/.."

expected="header=64 slot=24 channel=88 shm=328 tx=64 rx=240"
gcc -std=c11 -Wall -Wextra -Iinclude tests/abi_layout.c -o build/abi_layout
actual=$(./build/abi_layout)

if [ "$actual" != "$expected" ]; then
   echo "ABI CHECK FAILED (host): got '$actual', expected '$expected'"
   exit 1
fi

echo "ABI CHECK host: $actual"

if printf 'int main(void){return 0;}\n' | gcc -m32 -x c - -o /tmp/flow-abi32 2>/dev/null; then
   gcc -m32 -std=c11 -Wall -Wextra -Iinclude tests/abi_layout.c -o build/abi_layout32
   actual32=$(./build/abi_layout32)
   if [ "$actual32" != "$expected" ]; then
      echo "ABI CHECK FAILED (32-bit): got '$actual32', expected '$expected'"
      exit 1
   fi
   echo "ABI CHECK 32-bit: $actual32"
else
   echo "ABI CHECK 32-bit: skipped (no -m32 toolchain)"
fi
