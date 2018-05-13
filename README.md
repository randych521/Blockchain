# Blockchain

1. Build
   $ make
2. Run
   blockchain_demo transactions.txt >& output.txt
3. This is my first attempt to build an in-memory block chain.
   It is written in C++. For hash I use a tiny library called picosha2.
   Transaction hashes from the content of the transaction.
   Block hashes from transaction hashes in the block.
