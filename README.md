# ZPRIZE-23-Prize Prize 1B submission

The main code of the submission located in the original baseline directory `./Prize 1B/baseline/gpu`

## How to build

To build the project please do the following
```bash
cd "Prize 1B/baseline/gpu"
./build.sh
```

It'll build all dependencies, e.g. Yrrid MSM. 

## How to run

```bash
cd "Prize 1B/baseline/gpu/merkle-tree"
cargo run --release
```

For the Merkle tree of height 15 the proving time of our solution is `762 ms` on RTX6000 ADA.

## The approach

We highly optimized all the steps of proof and gadget generation:
 - all the measured gadget generation (`./Prize 1B/baseline/gpu/TrapdoorTech-zprize-crypto-cuda/src/cub/poseidon_hash.cu`) and operation over polynomials (`./Prize 1B/baseline/gpu/TrapdoorTech-zprize-crypto-cuda/src/cub/poly_functions.cu`) were moved to CUDA
 - the solution works really carefully with CUDA memory and streams copying poly in the batch mode
 - the original solution used a lot of memory and computation efforts for parts in Garage library which aren't really used, e.g. lookup tables, they were optimized or removed
 - division for `x^n - 1` was optimized for GPU
 - the Yrrid MSM (for our case it works better comparing to the Matter Labs's approach) is used

More info is [here](https://docs.google.com/document/d/1cxjlyX6dSt25k1hd6KHSHCrgJuGMps8rztSYIw7gcJI/edit?usp=sharing).
