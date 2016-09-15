#!/bin/bash

for file in `find . -name \*.cpp -o -name \*.h*`; do 
  clang-format $file > /tmp/formatted_file;
  cat /tmp/formatted_file > $file;
done

rm -f /tmp/formatted_file
