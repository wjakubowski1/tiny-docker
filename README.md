# Tiny Docker

A lightweight Linux container runtime written from scratch in C. This project explores low-level Linux kernel primitives that power modern containerization engines like Docker and containerd, focusing on process isolation, filesystem jails, and hardware resource restriction without relying on external libraries.

## Current Features

* Process Creation with `clone()`: Uses low-level `clone()` system call with a manually allocated 1 MB downward-growing heap stack instead of standard `fork()`.
* UTS Namespace Isolation (`CLONE_NEWUTS`): Isolates the container's hostname (`tiny-container`) so modifications inside the container do not affect the host Linux environment.
* PID Namespace Isolation (`CLONE_NEWPID`): Detaches the process ID tree so the spawned child process views itself as `PID 1` (the init process) inside its own isolated namespace.

## Next Steps

* Filesystem Isolation (`chroot`): Trap the container inside a dedicated Alpine Linux Mini Root Filesystem (`rootfs`) so it cannot access host files.
* Mount Namespace (`CLONE_NEWNS`): Mount an isolated `/proc` virtual filesystem inside the container so tools like `ps` and `top` only report container processes.
* Control Groups (`cgroups v2`): Restrict memory and CPU consumption via `/sys/fs/cgroup` to prevent resource exhaustion and fork bombs on the host.

## How to compile and run

Use GCC to compile the source code:

`gcc main.c -o tiny-docker`

`./tiny-docker`

## Execution
