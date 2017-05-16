#!/bin/bash

MAX=5
TH=1

while [ $TH -lt $MAX ]; do
  export OMP_NUM_THREADS=$TH
  echo "NUM_THREADS=$OMP_NUM_THREADS"
  touch openmp_sections.$TH.txt
  ./openmp_sections.exe >> openmp_sections.$TH.txt 2>&1
  sleep 2
  touch openmp_tasks.$TH.txt
  ./openmp_tasks.exe >> openmp_tasks.$TH.txt 2>&1
  sleep 2
  TH=$[$TH+1]
done
