# Simple Garbage Collector

This is a simple garbage collector implemented in C, which uses the Mark and Sweep algorithm to manage memory and automatically deallocate objects that are no longer in use. It helps prevent memory leaks in your C programs. Below is a basic overview of the key components and functionality of this garbage collector.

## Components

1. **Object Type**:
   - The garbage collector manages objects of two types: integers and pairs.
   - Integers (`OBJ_INT`) are single values.
   - Pairs (`OBJ_PAIR`) are objects that consist of two linked objects, a "head" and a "tail."

2. **Object Structure**:
   - Objects are represented as structures with fields for type, marking status, and the next object in the linked list.
   - Depending on the type, objects also contain specific data such as integer values or references to other objects for pairs.

3. **Virtual Machine (VM)**:
   - The `VM` struct maintains a stack and tracks the total number of objects.
   - It also has a threshold for triggering garbage collection (`gc`) when the number of objects exceeds a specified limit.

## Mark and Sweep Algorithm

- **Marking Phase**: The algorithm marks all reachable objects starting from the objects on the stack and any objects they reference. This ensures that reachable objects are not deallocated.

- **Sweeping Phase**: Unmarked objects are considered garbage and are freed during this phase. The garbage collector reclaims memory by removing unreferenced objects from the linked list.

## Key Functions

- `newVM()`: Initializes a new virtual machine.
- `push()` and `pop()`: Manages a stack of objects in the virtual machine.
- `newObject()`: Creates a new object of a specified type.
- `pushInt()` and `pushPair()`: Convenience functions for creating and pushing objects onto the stack.
- `gc()`: Initiates the garbage collection process using the Mark and Sweep algorithm to free unreferenced objects.
- `freeVM()`: Cleans up the virtual machine and performs garbage collection before deallocating the VM itself.

## Usage

1. Create a new virtual machine using `newVM()`.

2. Use `push()` to push objects onto the stack.

3. When the number of objects reaches a defined threshold (`INITIAL_GC_THRESHOLD`), the `gc()` function is automatically triggered to free unreferenced objects using the Mark and Sweep algorithm.

4. Finally, clean up and deallocate memory using `freeVM()` when you're done with the virtual machine.

## Example

Here's a simple example of how to use this garbage collector:

```c
int main() {
    VM* vm = newVM();

    // Push integers onto the stack
    pushInt(vm, 42);
    pushInt(vm, 100);

    // Create and push a pair of integers
    pushPair(vm);

    // Perform operations with the stack

    // Free memory and clean up the virtual machine
    freeVM(vm);

    return 0;
}
```

This simple garbage collector uses the Mark and Sweep algorithm to manage memory efficiently in your C programs, preventing memory leaks and improving the overall reliability of your code.
