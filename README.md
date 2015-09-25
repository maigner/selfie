# Selfie

Selfie is a project of the [Computational Systems Group](http://www.cs.uni-salzburg.at/~ck) at the Department of Computer Sciences of the University of Salzburg in Austria.

The Selfie Project provides an educational platform for teaching undergraduate and graduate students the design and implementation of programming languages and runtime systems. The focus is on the construction of compilers, libraries, operating systems, and even virtual machine monitors. The common theme is to identify and resolve self-reference in systems code which is seen as the key challenge when teaching systems engineering, hence the name.

There is a free [book](http://leanpub.com/selfie) in early draft form using selfie even more ambitiously reaching out to everyone with an interest in learning about computer science.

Selfie is a fully self-referential 4k-line C implementation of:

1. a self-compiling compiler called cstarc that compiles a tiny but powerful subset of C called C Star (C*) to a tiny but powerful subset of MIPS32 called MIPSter,
2. a self-executing emulator called mipster that executes MIPSter code including itself when compiled with cstarc, and
3. a tiny C* library called libcstar utilized by cstarc and mipster.
 
Selfie is kept minimal for simplicity and implemented in a single file. There is no linker, assembler, or debugger. However, there is minimal operating system support in the form of MIPS32 o32 system calls built into the emulator. Selfie is meant to be extended in numerous ways.

C* is a tiny Turing-complete subset of C that includes dereferencing (the * operator) but excludes data structures, Boolean expressions, and many other features. There are only signed 32-bit integers and pointers, and character constants for constructing word-aligned strings manually. This choice turns out to be helpful for students to understand the true role of composite data structures such as arrays and records. Bitwise operations are implemented in libcstar using signed integer arithmetics helping students gain true understanding of two's complement. C* is supposed to be close to the minimum necessary for implementing a self-compiling, single-pass, recursive-descent compiler. C* can be taught in around two weeks of classes depending on student background.

The compiler can readily be extended to compile features missing in C* and to improve performance of the generated code. The compiler generates MIPSter executables that can directly be executed by the emulator or written to a file in a simple, custom-defined format. Support of standard MIPS32 ELF binaries should be easy but remains future work.

MIPSter is a tiny Turing-complete subset of the MIPS32 instruction set. It only features arithmetic, memory, and control-flow instructions but neither bitwise nor byte-level instructions. MIPSter can be properly explained in a single week of classes.

The emulator implements minimal operating system support that is meant to be extended by students, first as part of the emulator, and then ported to run on top of it, similar to an actual operating system or virtual machine monitor. The fact that the emulator can execute itself helps exposing the self-referential nature of that challenge.

Selfie is the result of many years of teaching systems engineering. The design of the compiler is inspired by the Oberon compiler of Professor Niklaus Wirth from ETH Zurich.

### selfie on Linux (32bit)
Requirements: Ubuntu GNU/Linux 32bit, Linux, gcc 4.8. The 
first step is to produce a binary that is compatible with your 
computer: To do that, use 'gcc' and compile selfie.c with it.
```
$ gcc selfie.c -o selfie
```
You can use the output either as emulator or compiler:
```
selfie [-c | -m <memory size in MB> <binary> ]
```
To compile selfie.c for mips, use the following commands.  Be aware
that the compiler requires an empty file 'out' inside the current
execution directory to write its output into it.
```
$ touch out
$ cat selfie.c | ./selfie -c
$ mv out selfie.mips
```

#### Self-compilation
This is a complete example for testing self-compilation:
```
$ gcc selfie.c -o selfie
$ cat selfie.c | ./selfie -c
$ mv out selfie.mips1
$ cat selfie.c | ./selfie -m 128 selfie.mips1 -c
$ mv out selfie.mips2
$ diff -s selfie.mips1 selfie.mips2
Files selfie.mips2 and selfie.mips are identical
```

#### Self-execution
To test self-execution, you can use the following commands.
It is important to always specify the amount of memory to be
assigned to each emulator instance:
```
$ ./selfie.x86 -m 128 selfie.mips <binary>
$ ./selfie.x86 -m 128 selfie.mips -m 64 selfie.mips -m 32 <binary>
```
It is possible to compile any C* source either with "selfie"
(on i386 computers), or "selfie.mips" on top of the emulator:
```
$ touch out
$ cat my-source.c | ./selfie -m 128 selfie.mips -c
$ mv out cstarbinary
$ ./selfie -m 128 selfie.mips -m 64 cstarbinary
```
#### Debugging 
By default, the boot prompt shows the amount of memory used
by the emulator instance and how the binary file terminated
(exit code).

You can enable verbose debugging with variables in selfie.c:

 - debug_diassemble: Put a disassemble output on the screen
 - debug_registers: Print register content
 - debug_syscalls: Print debugging information on every syscall
 - debug_load: Print hex code what selfie loaded

Output may be very long.

### selfie on Mac OS X / Linux (64bit)
On Mac OS X and Linux 64 bit, you may use the following command to
compile your selfie file:
```
clang -m32 -D'main(a, b)=main(int argc, char **argv)' -w selfie.c
```
After that, you can proceed with the same commands as for Ubuntu 32
bit.


