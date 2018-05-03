#!/bin/bash

set -e

for i in `seq 1 1000`
do
		./soda > /dev/null
done