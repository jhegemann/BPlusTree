# C++ template in-memory B+ tree implementation

## Performance
The implementation aims for performance and supports saving and bulk loading the B+ tree structure to disk in a binary format. To finetune the performance adapt
```
#define INNER_NODE_DEGREE 32
#define OUTER_NODE_DEGREE 32
```
to find the sweet spot of your processor's cache behavior. For very large nodes reactivate
```
#undef INNER_NODE_BINARY_SEARCH
#undef OUTER_NODE_BINARY_SEARCH
```
which are deactivated in the standard implementation.

## Compilation
Compile the test suite with
```
g++ db_test.cc -o test
```
and execute accordingly with
```
./test
```

## Serialization
To support for custom serialization with your own classes specialize the template
```
template <class T> class Serializer;
```
according to the many examples that are given for the standard library containers.
