# Parallel Real-Time Image Processing using OpenMP in C
## Overview

This project implements a **parallel image processing engine** in C using **OpenMP** to accelerate computationally intensive filters on large grayscale images.

The focus is on:

* Multicore parallelism (CPU)
* Performance analysis (time, speedup, efficiency)
* Real-time processing simulation
* Practical limitations of GPU acceleration (OpenCL)

---

## Objectives

* Implement image filters in **sequential and parallel (OpenMP)** modes
* Analyze performance using:

  * Execution time
  * Speedup
  * Efficiency
* Build a **multi-stage image processing pipeline**
* Evaluate real-world constraints (memory bottlenecks, scalability)

---

## Concepts Covered

* OpenMP parallel programming
* Memory-bound vs compute-bound workloads
* Cache behavior and memory bandwidth limitations
* Real-time processing constraints
* CPU performance profiling

---

## Features

### Image Generation

* Synthetic grayscale image (large 2D array)
* Salt & pepper noise injection

### Filters Implemented

* **Box Blur (3×3)**
* **Gaussian Blur (3×3)**

### Execution Models

* Sequential (single-thread)
* OpenMP (multi-thread)

---

## Parallelization Strategy

Parallelism is applied using OpenMP:

```c
#pragma omp parallel for schedule(static)
```

* Applied on outer loops
* Ensures balanced workload across threads

---

## Processing Pipeline

A real-world processing pipeline is implemented:

Input → Box Blur → Gaussian Blur → Output

Evaluated in:

* Sequential mode
* OpenMP parallel mode

---

## Project Structure

```
project/
│── main.c
│── image.c / image.h
│── filters.c / filters.h
│── opencl.c / opencl.h   (experimental)
│── kernel.cl             (OpenCL kernel)
```

Core filter implementations: 

---

## Build & Run

### Compile

```bash
gcc -fopenmp main.c image.c filters.c opencl.c -lOpenCL -o app
```

### Run

```bash
./app
```

---

## Experimental Results

### Box Blur

| Model      | Time (s) | Speedup  | Efficiency |
| ---------- | -------- | -------- | ---------- |
| Sequential | 0.0987   | 1.0      | N/A        |
| OpenMP (8) | 0.0381   | 2.59     | 0.32       |
| OpenCL     | 0.0079   | **12.5** | N/A        |

---

### Gaussian Blur

| Model      | Time (s) | Speedup | Efficiency |
| ---------- | -------- | ------- | ---------- |
| Sequential | 0.1124   | 1.0     | N/A        |
| OpenMP (8) | 0.0498   | 2.25    | 0.28       |

---

### Pipeline (Box → Gaussian)

| Model      | Time (s) | Speedup | Efficiency |
| ---------- | -------- | ------- | ---------- |
| Sequential | 0.2031   | 1.0     | N/A        |
| OpenMP (8) | 0.0830   | 2.44    | 0.30       |

---

## Performance Analysis Tools

### perf

```bash
perf stat ./app
```

Used to analyze:

* CPU cycles
* Instructions
* Cache misses

---

### htop

```bash
htop
```

Used to verify:

* Multi-core utilization
* Thread-level execution

---

## Key Observations

### 1. Memory-Bound Behavior

* Box blur and Gaussian filters are **memory-intensive**
* Limits OpenMP scalability

---

### 2. Limited Parallel Efficiency

* Efficiency ≈ 30%
* Due to:

  * Memory bandwidth contention
  * Cache limitations

---

### 3. Consistent Scaling

* Pipeline scaling follows individual filters
* Parallel efficiency remains stable across workloads

---

### 4. GPU Acceleration (OpenCL)

* Significant speedup observed for box blur (~12×)
* However:

  * System instability for large workloads
  * Limited by integrated GPU (Vega 8 + Mesa drivers)

---

## OpenCL Limitation

Although OpenCL implementation was attempted:

* System crashes occurred for larger workloads
* Instability observed with Mesa OpenCL on iGPU

### Conclusion:

> GPU acceleration is effective but depends heavily on hardware and driver stability.

---

## 🧾 Final Conclusion

* OpenMP provides **significant speedup (~2.5×)**
* Performance is limited by **memory bandwidth**
* Real-time processing for large images is challenging
* GPU acceleration can outperform CPU but requires stable environment

---
