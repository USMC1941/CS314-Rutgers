# CS314 Fall 2018 Project 03 

## About

- Date: 11/27/2018

## Instuctions

1. Upload `match.tgz` to your ilab machine
2. Extract `match.tgz`
   ```sh
   tar -xvzf match.tgz
    ```
3. Download the matrix file
    e.g. for matrix `cant`:
    ```sh
    wget https://sparse.tamu.edu/MM/Williams/cant.tar.gz
    ```
    If the disk space is not enough, please use [freespace](https://resources.cs.rutgers.edu/docs/file-storage/storage-technology-options/).
4. Extract the matrix file
    ```sh
    tar -xvzf cant.tar.gz
    ```
5. `make match` should compile everything into a single executable - `match`.
6. Please check usage before you start
   ```sh
   ./match
   ```
7. Run. Ex:
   ```sh
   ./match cant/cant.mtx cant.out oneWay 2
   ```
8. Check matching result in `cant.out` (specified by the argument)
9.  You can use the provided binary `match-sol` to verify your output.

## Author 

Yanhao Chen
