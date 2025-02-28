# Hash table

A hash table is a data structure that implements an associative array
*(dictionary)*.

A hash table uses a **hash function** to compute an *index* also known 
as *hash code* from which the desired value can be found.

---
## Separate chaining

Separate chaining is a technique used to handle collisions in a hash table.
When two or more keys map to the same index in the array, we store them in a
linked list at that index.
This allows us to store multiple values at the same index and still be able to
retrieve them using their key.
