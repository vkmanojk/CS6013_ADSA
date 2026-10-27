________________________________________________________________________________________
Course: CS6013 - Advanced Data Structures and Algorithms
Coding Assignment 2
Name: Manoj Kumar V K
Roll Number: CS26RESCH11009
Compiler: g++ (GCC) 16.1.0 / C++17 Standard
________________________________________________________________________________________
Files Included:
Q1.cpp    - B-Tree Operations
Q2.cpp    - Red-Black Tree with Order Statistics
Q3.cpp    - Range Enumeration in a Red-Black Tree
Q4.cpp    - 2-CNF Satisfiability
Q5.cpp    - The Forbidden Shortcut
Q6.cpp    - Powering the Cities
Q7.cpp    - Mandatory Roads in a Minimum Spanning Tree
Q8.cpp    - Circular Pattern Matching Using KMP
Q9.cpp    - Polynomial Multiplication using FFT
Q3.pdf    - Proof of Correctness and Running-Time Analysis for Question 3
Q9(b).pdf - Written Response and Analysis for Question 9(b)
________________________________________________________________________________________
Compilation Instructions:
Open your terminal or command prompt, navigate to the extracted folder, 
and run the following commands to compile each program independently:

```
g++ -std=c++17 Q1.cpp -o Q1
g++ -std=c++17 Q2.cpp -o Q2
g++ -std=c++17 Q3.cpp -o Q3
g++ -std=c++17 Q4.cpp -o Q4
g++ -std=c++17 Q5.cpp -o Q5
g++ -std=c++17 Q6.cpp -o Q6
g++ -std=c++17 Q7.cpp -o Q7
g++ -std=c++17 Q8.cpp -o Q8
g++ -std=c++17 Q9.cpp -o Q9
```

Execution Instructions:

For Windows (Command Prompt / PowerShell):
To run manually and type input: .\Q1.exe
To run using a test case text file: .\Q1.exe < input.txt
To save output directly to a file: .\Q1.exe < input.txt > output.txt

For Linux and macOS (Terminal):
To run manually and type input: ./Q1
To run using a test case text file: ./Q1 < input.txt
To save output directly to a file: ./Q1 < input.txt > output.txt

(Repeat the same execution format for Q2 through Q9 by replacing the executable name)
________________________________________________________________________________________
Reports and Proofs:
1. Question 3 Report:
   Proof of correctness and running-time analysis for Range Enumeration in a Red-Black Tree
   is provided in the attached separate PDF: Q3.pdf.

2. Question 9(b) Report:
   Written response and analysis for Kronecker Product and its connection to FFT
   is provided in the attached separate PDF: Q9(b).pdf.
   (The written response is also included below under the Q9(b) heading.)
________________________________________________________________________________________
Q9(b) - Kronecker Product and its Connection to FFT:

(i) The claim is impossible.
    For two n-dimensional vectors u and v, their Kronecker product z = u (x) v has entries
    z_{in+j} = u_i * v_j for all 0 <= i, j < n.
    This results in n^2 individual entries.
    Since outputting each scalar requires Omega(1) time, explicitly outputting n^2 entries 
    requires Omega(n^2) time, which cannot be achieved in O(n log n) time.

(ii) Implicit representation using O(n) space:
     Store only the two original vectors u and v in O(n) space.
     Whenever an arbitrary entry z_{in+j} is queried, retrieve u_i and v_j and compute 
     u_i * v_j on-the-fly in O(1) time.

(iii) Distinction between Kronecker Product and Polynomial Convolution:
     The Kronecker product u (x) v independently pairs every element of u with every 
     element of v, resulting in n^2 distinct pairwise products u_i * v_j.
     Polynomial convolution, on the other hand, sums all pairwise products whose indices 
     i, j add up to k:
         s_k = sum_{i+j=k} u_i * v_j
     Thus, while the Kronecker product yields n^2 separate entries in R^{n^2}, convolution 
     produces a combined vector in R^{2n-1}.
     Ordinary polynomial convolution takes O(n^2) time. FFT optimizes this to O(n log n) 
     time by converting the coefficient representation into point-value representation at 
     roots of unity, performing point-wise multiplication in O(n) time, and using Inverse 
     FFT for interpolation to recover the resulting coefficients in O(n log n) time.
________________________________________________________________________________________
NOTE:
1. All programs are written in standard C++ using standard header files only.
2. Dynamic memory allocation is handled within each program to avoid memory leaks.
