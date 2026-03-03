#!/bin/bash
# writer.sh
# Usage: ./writer.sh <writefile> <writestr>

writefile="$1"
writestr="$2"

# 1) Validate args
if [ $# -lt 2 ]; then
  #echo "Error: missing arguments."
  #echo "Usage: $0 <writefile> <writestr>"
  exit 1
fi

# Extra safety: reject empty strings
if [ -z "$writefile" ] || [ -z "$writestr" ]; then
  echo "Error: writefile and writestr must be specified and non-empty."
  exit 1
fi

# 2) Create parent path if it doesn't exist
dirpath="$(dirname "$writefile")"
if [ ! -d "$dirpath" ]; then
  mkdir -p "$dirpath"
  if [ $? -ne 0 ]; then
    echo "Error: could not create directory path '$dirpath'"
    exit 1
  fi
fi

# 3) Create/overwrite file with content
# Overwrites any existing file.
echo "$writestr" > "$writefile"
if [ $? -ne 0 ]; then
  echo "Error: could not create/write file '$writefile'"
  exit 1
fi

exit 0