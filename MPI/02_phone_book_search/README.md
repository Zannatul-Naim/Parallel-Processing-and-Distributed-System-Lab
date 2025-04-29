# 📘 MPI Distributed Phonebook Search
-----------------------------------

This program demonstrates a distributed phonebook search using **MPI in C++**. It takes one or more phonebook text files and a search term, distributes the search across multiple MPI processes, and returns matching results efficiently.

* * * * *

### 📁 File Structure



```bash
    search_optimized.cpp     # Main source file
    README.md                # This file`
```
* * * * *

### 📥 Input Format

Each phonebook text file should contain lines in the format:

```nginx
    Name PhoneNumber
```

**Example:**

```nginx
    Alice 123456
    Bob 789012
    Charlie 345678
```
* * * * *

### 🚀 Compilation

Use mpic++ to compile:

```bash 
    mpic++ -o search search_optimized.cpp
```

* * * * *

### 🧪 Run Example

Run with 4 processes on 2 phonebook files and search for "Bob":

```bash
    mpirun -np 4 ./search phonebook1.txt phonebook2.txt Bob
```
* * * * *

### 📤 Output

The program outputs all matching entries whose name contains the search term, followed by the total time taken:

```sql
Search Results:

Bob 01989123456
Bob 01999123456
Bob 01799123456
Process 0 execution time: 0.000018 seconds.
Process 1 execution time: 0.000020 seconds.
Process 2 execution time: 0.000018 seconds.
Process 3 execution time: 0.000010 seconds.
```

* * * * *

### ⚙️ Features & Optimizations

-   **Distributed Workload**: Dynamically divides phonebook entries across MPI processes.

-   **Efficient Broadcasts**: Uses MPI_Bcast for search term.

-   **Structured Data Sharing**: Distributes name/phone pairs without string serialization overhead.

-   **Dynamic Gathering**: Collects variable-length results using MPI_Gatherv.

-   **Timing Analysis**: Reports total distributed search time.

* * * * *

### 🧼 Cleanup

To remove compiled files: `rm search`

* * * * *

### 📚 Requirements

-   MPI (e.g., MPICH, OpenMPI)

-   C++ compiler supporting C++11 or newer

* * * * *

### 🧑‍💻 Author

- Developed by [ChatGPT] Contributions & suggestions welcome!