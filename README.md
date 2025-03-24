# B-TREE

### B-Tree Implementation Project

This project implements a B-Tree data structure in C++. B-Trees are balanced tree structures widely used in databases and file systems.

### Features

B-Tree node creation and management

Key insertion

Key deletion

Inorder traversal

Node splitting

Key redistribution

Node merging

### Key Functions

`insertBT`: Inserts a new key into the B-Tree

`deleteBT`: Removes a key from the B-Tree

`searchPath`: Searches for a key and returns the path

`splitNode`: Handles node splitting

`redistributeKey`: Redistributes keys when underflow occurs

`mergeNode`: Merges with sibling nodes

`inorder`: Displays the tree using inorder traversal

### Usage

The program accepts commands via standard input:

`i [key]`: Insert key

`d [key]`: Delete key

After each command, the tree state is displayed in inorder traversal format.

### Example

```
i 10
< 10 >
i 20
< 10 20 >
i 5
< 5 10 20 >
d 10
< 5 20 >
```

### Implementation Details
Implements a B-Tree of order 4

Handles node overflow through splitting and key promotion

Manages node underflow through key redistribution or node merging

Different deletion handling for leaf and internal nodes

### Notes

Call clearTree function before program termination to prevent memory leaks

Error messages are displayed when attempting to insert existing keys or delete non-existent keys

### Compilation and Execution

```
g++ -o btree btree.cpp
./btree
```

Input is accepted from standard input (stdin). To read from a file:

```
./btree < input.txt
```
