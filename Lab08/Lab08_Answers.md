# SE4060 Parallel Computing - Lab 08
## MPI Lab Answers

Student ID: IT24101643

---

## Exercise 1

A GitHub repository was used to store and manage all solutions for this MPI lab.

Repository:

https://github.com/IT24101643/SE4060-MPI-Part1

All source code and results were committed and pushed to this repository.

---

## Exercise 2

The numbers from 1 to 10,000,000 were divided among multiple MPI processes.

Each process calculated a partial sum, and MPI_Reduce was used to combine all partial sums.

Final Total:

50000005000000

### Results

| Processes | Time (seconds) | Speedup |
|-----------|----------------|---------|
| 1 | 0.021212 | 1.00 |
| 2 | 0.011308 | 1.88 |
| 4 | 0.011061 | 1.92 |

The execution time decreased when more processes were used.

---

## Exercise 3

The Monte Carlo method was used to estimate Pi using 10,000,000 trials.

Random points were generated inside a square.

A point was counted as being inside the circle when:

x² + y² <= 1

Pi was estimated using:

Pi = 4 × (Points Inside Circle / Total Points)

### Results

| Processes | Estimated Pi | Time (seconds) | Speedup |
|-----------|--------------|----------------|---------|
| 1 | 3.1417748000 | 0.138410 | 1.00 |
| 2 | 3.1427432000 | 0.069278 | 2.00 |
| 4 | 3.1425256000 | 0.069498 | 1.99 |

The estimated values are close to the actual value of Pi.

---

## Exercise 4

### Speedup Formula

Speedup = T1 / Tp

Where:

T1 = execution time using one process

Tp = execution time using P processes

### Observation

For both MPI programs, the execution time decreased when the number of processes increased from 1 to 2.

Exercise 2 achieved a speedup of approximately 1.88 using 2 processes and 1.92 using 4 processes.

Exercise 3 achieved approximately 2 times speedup using both 2 and 4 processes.

The 4-process execution did not show much additional improvement because the EC2 instance had limited CPU resources and the test was performed using oversubscription.

---

## Exercise 5.1

### What happens when the source and destination do not match?

The program hangs because the sender and receiver do not match.

Rank 1 sends a message to Rank 3, but Rank 3 waits for a message from Rank 2.

Because Rank 2 never sends the expected message, MPI_Recv continues waiting.

The synchronous MPI_Ssend operation can also remain blocked while waiting for a matching receive.

Therefore, the program results in a deadlock.

---

## Exercise 5.2

The original message program was rewritten using MPI_Bsend.

MPI_Bsend performs a buffered send.

A buffer is first attached using:

MPI_Buffer_attach()

The message is then sent using:

MPI_Bsend()

After the send operation, the buffer is detached using:

MPI_Buffer_detach()

Buffered send allows the sender to place the message into the attached buffer without waiting for the receiver to immediately receive it.

---

## Exercise 6

Exercise 3 was modified to use MPI_ANY_SOURCE.

In the original Exercise 3, MPI_Reduce was used to combine the results from all processes.

In Exercise 6, each worker process sends its calculated result to Rank 0.

Rank 0 receives the messages using MPI_ANY_SOURCE.

This means Rank 0 can receive a message from any worker process that finishes first.

### Results

2 Processes:

Estimated Pi = 3.1425280000

Execution Time = 0.138681 seconds

4 Processes:

Estimated Pi = 3.1425052000

Execution Time = 0.092758 seconds

### Comparison

Both Exercise 3 and Exercise 6 estimate Pi using 10,000,000 trials.

Exercise 3 uses MPI_Reduce to combine partial results.

Exercise 6 uses MPI_Send and MPI_Recv with MPI_ANY_SOURCE.

MPI_ANY_SOURCE allows Rank 0 to receive results in the order that the worker processes finish.

The execution times are different because the communication method is different.

---

## Exercise 7

Exercise 7 modifies Exercise 6 by using MPI_Bsend instead of the normal MPI_Send operation.

Each worker process calculates its part of the Monte Carlo simulation.

The worker attaches a communication buffer using MPI_Buffer_attach and sends the result using MPI_Bsend.

Rank 0 receives the results using MPI_ANY_SOURCE.

MPI_ANY_SOURCE allows Rank 0 to accept messages from whichever worker finishes first.

MPI_Bsend allows a worker to copy its message into the attached buffer and continue without waiting for the receiver to immediately receive the message.

Therefore, Exercise 7 combines buffered communication with flexible message reception using MPI_ANY_SOURCE.
