# Specs

All tests were run on AWS instance type **m4.2xlarge**, which was chosen because it represents a good compromise between compute and memory efficiency along with having high network performance.

## Specs of the System: AWS m4.2xlarge
- 8 vCPUs
- 4 Cores
- 2 threads per core
- 32GiB mem
- 1,000 Mbps Dedicated EBS Bandwidth 
- CPU - Intel(R) Xeon(R) CPU E5-2686 v4 @ 2.30GHz
- 2299.939 CPU MHz
- L1d cache - 32K, L1i cache - 32K, L2 cache - 256K, L3 cache - 46080K

## Network Specs
- 2 **m4.2xlarge** instances in a VPC
- Bandwidth  - 1.02 Gbits/sec (tested with `iperf` )
- Ping - 0.177 ms (tested with `ping` )

## Operating System
- Ubuntu Server 18.04 LTS
- Kernel - 4.15.0-1065-aws

## Compiler
- `mpic++` , mpicxx for MPICH version 3.3a2
- gcc version 7.5.0
- Flags `-fopenmp` 

## Libraries (C++)
- `iostream` 
- `queue`
- `fstream`
- `list`
- `algorithm`
- `stdio`
- `cstdio`
- `ctime`
- `map`
- `numeric`
- `array`
- `bits/stdc++.h`

## Configuration Instructions
- A VPC was created in AWS for the instances, per the [CS205 Lab I7 Lab Instructions](https://harvard-iacs.github.io/2020-CS205/lab/I7/guide/Guide_I7.pdf). 
- The MPI port range was configured with the following commands:
```export MPIR_CVAR_CH3_PORT_RANGE=10000:10100
export MPICH_PORT_RANGE=10000:10100
```
- Number of OpenMP flags configured via `export OMP_NUM_THREADS=x`
- MPI ran via `mpirun -np [num_nodes] -hosts [list of instances] ./jacobi1d [ncells] [nsteps] [fname]` 

