```markdown
# MiniRTOS - Cooperative Round-Robin Scheduler

MiniRTOS is a lightweight, educational Real-Time Operating System (RTOS) implemented in C, designed to explore and demonstrate core concepts of cooperative multitasking and task scheduling in embedded systems. This project is intended for personal learning, experimentation, and as a reference for building small-scale operating systems.

---

## Table of Contents

- [Project Overview](#project-overview)  
- [Features](#features)  
- [Project Structure](#project-structure)  
- [Building the Project](#building-the-project)  
- [Usage](#usage)  
- [Contributing](#contributing)  
- [License](#license)  

---

## Project Overview

The goal of MiniRTOS is to implement a **cooperative round-robin scheduler** in a small-scale RTOS environment. The project emphasizes:

- **Task management:** Creating, switching, and scheduling multiple tasks.  
- **Cooperative scheduling:** Tasks voluntarily yield control to allow multitasking.  
- **Modular design:** Separated components for ease of extension and experimentation.  
- **Integration with custom libraries:** Utilizes a small utility library (`libft`) and a banner module for demonstration and logging purposes.

This project serves as a foundation for understanding the internals of an RTOS without the complexity of full preemptive multitasking or hardware-specific constraints.

---

## Features

- Cooperative round-robin task scheduler.  
- Simple task API for creating, running, and yielding tasks.  
- Task queuing and management using linked lists.  
- Integration with a custom `libft` library for utility functions.  
- Banner module to provide formatted output and visual feedback.  
- Modular Makefile system, building both sub-libraries and the main RTOS.  

---

## Project Structure

```

minirtos/
├── src/                 # Source files for the RTOS
│   ├── main.c           # Entry point
│   ├── rtos.c           # Scheduler implementation
│   └── utils.c          # Utility functions for the RTOS
├── objs/                # Object files generated during compilation
├── bin/                 # Compiled binaries and archives
│   └── minirtos.a
├── libft/               # Custom utility library
│   ├── src/             # Source files for libft
│   └── bin/             # Compiled libft library (libft.a)
├── banner/              # Banner/visual output module
│   ├── src/             # Source files for banner
│   └── bin/             # Compiled banner library (banner.a)
├── Makefile             # Main Makefile coordinating compilation
└── README.md            # Project documentation

````

**Key modules:**

- `libft` – Custom utility library including string, memory, and matrix operations.  
- `banner` – Module for ASCII banners and formatted logging.  
- `src/` – Core RTOS source code: scheduler, task management, and main program.

---

## Building the Project

This project uses a **modular Makefile system** that compiles sub-libraries (`libft` and `banner`) first, then links them with the main RTOS sources.

### Prerequisites

- C compiler (`clang` or `gcc`)  
- GNU Make  

### Compilation

From the project root:

```bash
make
````

This will:

1. Compile `libft` and `banner` into static libraries (`.a`).
2. Compile the RTOS source files into object files.
3. Link all objects and libraries into the final executable `bin/minirtos`.

### Cleaning

```bash
make help        # Show help message with other rules
make clean       # Remove object files
make fclean      # Remove object files and binaries
```

---

## Usage

After building, run the MiniRTOS executable:

```bash
./bin/minirtos
```

Currently, the RTOS demonstrates cooperative task scheduling with sample tasks defined in `main.c`. Tasks yield control voluntarily, showing the round-robin behavior in the console.

---

## Contributing

This project is primarily for personal learning, but contributions for:

* Bug fixes
* Documentation improvements
* Adding additional scheduler features

are welcome via pull requests.

---

## License

This project is released under the MIT License. See `LICENSE` for details.

---

## Notes

* This RTOS is educational and does not include preemptive scheduling or hardware-specific drivers.
* The project uses `libft` and `banner` modules to demonstrate modular design and linking with static libraries.
* The scheduler implementation can be extended to include time slices, preemption, or inter-task communication for further learning purposes.

```

