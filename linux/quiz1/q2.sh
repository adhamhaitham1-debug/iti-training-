#!/bin/bash
vec=(3 7 4 9 12 6 11 2)
echo "${vec[@]}" | awk '{for(i=1;i<=NF;i++) a[$i]=1; for(i=2;i<=12;i++) if(!a[i]) print i}'
