# xv6-riscv Operating System Enhancements – Academic Project

This repository is a customized implementation of the [xv6-riscv](https://github.com/mit-pdos/xv6-riscv) teaching operating system. It was developed as part of an Operating Systems course to explore internal mechanisms of UNIX-like kernels. The work is structured in three distinct parts: user-level command development, system call implementation, and kernel-level scheduling with performance analysis.

---

## Project Objectives

This project aims to:
- Extend the xv6 shell with new user-space commands
- Implement and test new system calls
- Integrate multiple CPU scheduling algorithms
- Track and report performance metrics such as turnaround and waiting time

---

## Project Breakdown

### Part 1: User Commands

Several new user-space commands were created to emulate common UNIX functionality:

| Command | Description |
|---------|-------------|
| `sleep` | Suspends execution for a number of ticks |
| `find`  | Recursively searches for a file in a directory and subdirectories |
| `touch` | Creates an empty file or raises an error if it exists |
| `mv`    | Moves or renames a file using `link` and `unlink` |
| `cp`    | Copies file content using `open`, `read`, and `write` |
| `add`   | Accepts two integers, prints their sum |
| `fact`  | Computes factorial of a non-negative integer |

Usage and error-checking were handled to match typical UNIX-like behavior (e.g., `add ?` outputs help info).

---

### Part 2: System Calls

The following system calls were added and integrated into the kernel:

| System Call   | Purpose |
|---------------|---------|
| `kbdint`      | Returns total number of keyboard interrupts |
| `countsyscall`| Returns number of system calls invoked since boot |
| `getppid`     | Returns parent process ID |
| `getptable`   | Returns an array of process info: PID, PPID, state, size, name |
| `datetime`    | Returns system date and time (based on `mtime`) |
| `rand`        | Returns pseudo-random integer (LCG seeded by ticks) |

Each system call includes corresponding user-level testing programs.

---

### Part 3: CPU Scheduling and Metrics

The default Round-Robin scheduler was enhanced to support:

- **First-Come First-Served (FCFS)** – Non-preemptive, based on process `ctime`
- **Priority-Based Scheduling** – Preemptive, using user-defined `priority`

#### Performance Metrics:
A new syscall `get_metrics()` was implemented to compute:
- **Turnaround Time** = `etime - ctime`
- **Waiting Time** = `Turnaround - rtime`

Metrics are collected from all finished processes and averaged for evaluation of scheduler effectiveness.

---

## Build and Run Instructions

### Requirements
- Linux/macOS
- RISC-V Toolchain (`riscv64-unknown-elf-gcc`)
- QEMU (`qemu-system-riscv64`)

### Build and Execute

```bash
git clone https://github.com/salmayasser23/xv6-riscv-os-project.git
cd xv6-riscv
make qemu
