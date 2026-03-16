Circuit Simulator (Nodal Analysis in C++)

A small circuit simulator written in C++ that solves DC resistor networks using nodal analysis and Gaussian elimination.

The program reads a simple SPICE-like netlist, constructs the conductance matrix for the circuit, and solves for the unknown node voltages.

This project demonstrates core concepts used inside real circuit simulators such as SPICE.

Features:
- Parses a simple netlist file describing the circuit
- Automatically detects all circuit nodes
- Identifies known vs unknown nodes
- Builds the conductance matrix (G matrix) using nodal analysis
- Handles voltage sources connected to ground
- Solves the resulting linear system using Gaussian elimination
- Prints the solved node voltages

An example netlist can be seen in the example.cir file. 

How It Works:

The sim performs nodal analysis. Steps:
1. Read the circuit netlist
2. Determine all nodes in the circuit
3. Separate nodes into:
        known nodes (ground and voltage sources)
        unknown nodes
4. Build the conductance matrix G
5. Construct the RHS current vector
6. Solve Gx = b where x = node voltages. 

The system is solved with Gaussian elimination.

Running the program:
Compile with: clang++ main.cpp -std=c++17 -o sim
Run with: ./sim

Possible Future Improvements:

Potential extensions:
    Support for current sources
    Support for multiple voltage sources
    Support for larger circuits (currently capped at 10 unknown nodes)
    Sparse matrix optimization
    AC analysis
    Transient simulation
