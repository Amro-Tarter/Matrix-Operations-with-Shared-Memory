# Matrix-Operations-with-Shared-Memory
This C program performs various matrix operations using shared memory for inter-process communication. It reads matrix data from shared memory, performs requested operations, and stores results back in shared memory. The program supports multiple matrix types and operations with proper synchronization.
Features
Matrix Operations:

ADD: Matrix addition

SUB: Matrix subtraction

MUL: Matrix multiplication

AND: Logical AND (for binary matrices)

OR: Logical OR (for binary matrices)

NOT: Logical NOT (for binary matrices)

TRANSPOSE: Matrix transposition

Supported Matrix Types:

Integer matrices

Double matrices (with rounding)

Complex number matrices

IPC Features:

Shared memory for data exchange

Semaphore synchronization

Support for multiple matrix operations in sequence

Compilation and Usage
Compile the program:

bash
Copy
gcc Ex3q2b.c -o matrix_operations -lpthread -lrt
Run the program (must be run after the writer process that creates the shared memory):

bash
Copy
./matrix_operations
Input/Output Format
Input: Received through shared memory in the format:

Copy
(rows,columns:value1,value2,...,valueN)
Output:

Results are stored back in shared memory

Printed to stdout in the same matrix format

"ERR" displayed for invalid operations

Shared Memory Structure
The program uses a shared memory segment containing an array of structures:

c
Copy
typedef struct sharedData {
    char data[3][MAX_CHAR];  // Holds matrix and operation data
} sharedData;
Synchronization
Uses two named semaphores:

/writer - controls write access

/reader - controls read access

Implements proper wait/post sequencing for process synchronization

Error Handling
The program handles errors in:

Shared memory operations (ftok, shmget, shmat)

Semaphore operations

Matrix operations (invalid types, non-binary matrices for logical ops)

Memory allocation

Matrix Operations Details
Integer matrices: Whole numbers only

Double matrices: Rounded to one decimal place

Complex matrices: Format as "a+bi"

Binary matrices: Only 0 and 1 values allowed for logical operations

Example Session
(Assuming proper shared memory setup by writer process)

Copy
(2,2:1+2i,3+4i,5+6i,7+8i)
(2,2:1+1i,2+2i,3+3i,4+4i)
ADD
(2,2:2+3i,5+6i,8+9i,11+12i)
Dependencies
Standard C libraries

POSIX semaphores (semaphore.h)

System V shared memory (sys/shm.h)

Complex number support (complex.h)

Notes
Shared memory segment must be created by another process first

Proper cleanup of shared memory should be handled by the creating process

Maximum matrix size is determined by MAX_CHAR (128) for string representation

Supports up to MAX_STRUCTS (16) matrix operations per session

Security Considerations
Uses fixed path ("/tmp") for ftok key generation

Shared memory permissions set to 0600 (owner read/write only)

Named semaphores use standard permissions

