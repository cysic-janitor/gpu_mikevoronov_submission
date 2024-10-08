# ZPRIZE-23-Prize Prize 1B submission

The main code of the submission located in the original baseline directory `./Prize 1B/baseline/gpu`

## How to build

To build the project please do the following
```bash
cd "zprize-2024-prize-1B/Prize 1B/baseline/gpu"
./build.sh
```

It'll build all the dependencies, e.g. Yrrid MSM. 

## How to run

```bash
cd "zprize-2024-prize-1B/Prize 1B/baseline/gpu/merkle-tree"
cargo run --release
```

The measured proving time of our solution is `733 ms` on RTX6000 ADA for the Merkle tree with height 15 and takes `~35 Gb` of GPU memory.

## How to bench

```bash
cd "zprize-2024-prize-1B/Prize 1B/baseline/gpu"
cargo bench
```

Results:

```bash
==============================
Start generating 4 proofs
Proof 0 is generated
Time elapsed: 2.746389927s
Proof 1 is generated
Time elapsed: 5.458944113s
Proof 2 is generated
Time elapsed: 8.171230481s
Proof 3 is generated
Time elapsed: 10.872814553s
The total prove generation time is 10.872817328s
Amortized cost for each proof is 2.718205434s
Real amortized cost for each proof is 733 msec
==============================
Start verifying 4 proofs
Proof 0 is verified: true
Time elapsed: 236.446341ms
Proof 1 is verified: true
Time elapsed: 472.429216ms
Proof 2 is verified: true
Time elapsed: 709.232374ms
Proof 3 is verified: true
Time elapsed: 945.512454ms
The prove verification time is 945.515159ms
Amortized cost for each proof is 236.379736ms
==============================
```

Current benchmark is almost the same as the original except that prover is created once for each iteration, this creation was decoupled before the cycle. The main reason for it is that all other "real" proving code is so fast that it estimates as `o(prover creation time)`, moreover all circuits can be created with only one prover instance.

Additionally, the benchmark also shows "real" proving time which is a measurement of the proof generation without any preparation work.

## Description

We highly optimized all the steps of proof and gadget generations:
 - all the measured [gadget generation operations](./zprize-2024-prize-1B/Prize 1B/baseline/gpu/TrapdoorTech-zprize-crypto-cuda/src/cub/poseidon_hash.cu) and [operations over polynomials](./zprize-2024-prize-1B/Prize 1B/baseline/gpu/TrapdoorTech-zprize-crypto-cuda/src/cub/poly_functions.cu) were moved to CUDA
 - the solution works really carefully with CUDA memory and streams copying polys in the batch mode
 - the original solution used a lot of memory and computation efforts for parts in the Garage library which aren't really used, e.g. lookup tables, they were optimized or removed
 - division for `x^n - 1` was optimized for GPU
 - poly inversion algo was reimplemented for GPU
 - the Yrrid MSM (for our case it works better comparing to the Matter Labs's approach) is used

More info is [here](https://docs.google.com/document/d/1cxjlyX6dSt25k1hd6KHSHCrgJuGMps8rztSYIw7gcJI/edit?usp=sharing).
