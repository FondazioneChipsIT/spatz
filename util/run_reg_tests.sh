#!/bin/bash

# set CTEST command
export CTEST=/usr/local/bin/ctest

# move to test directory
cd hw/system/spatz_cluster

for cfg in spatz_cluster.default.dram spatz_cluster.smallvrf.dram; do

    echo "=========================================="
    echo " Running tests for config: $cfg"
    echo "=========================================="

    # compile
    make sw.vsim -B SPATZ_CLUSTER_CFG=${cfg}.hjson

    # run snRuntime test class
    cd sw/build/snRuntime
    ${CTEST} --output-on-failure
    cd ../../..

    # run riscvTests test class
    cd sw/build/riscvTests
    ${CTEST} --output-on-failure -j4
    cd ../../..

    # run spatzBenchmarks test class
    cd sw/build/spatzBenchmarks
    ${CTEST} --verbose
    cd ../../..

done