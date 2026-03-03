#!/bin/bash
# finder.sh
# Usage: ./finder.sh <filesdir> <searchstr>

filesdir="$1"
searchstr="$2"

# 1) Validate args
if [ $# -lt 2 ]; then
  echo "Error: missing parameters."
  #echo "Usage: $0 <filesdir> <searchstr>"
  exit 1
fi

# 2) Validate filesdir is a directory
if [ ! -d "$filesdir" ]; then
  echo "Error: filesdir '$filesdir' does not represent a directory on the filesystem."
  exit 1
fi

# 3) Count files (recursive)
num_files=$(find "$filesdir" -type f | wc -l)

# 4) Count matching lines across all files (recursive)
# -R: recursive, -h: no filename, -I: ignore binary files
# If no matches, grep exits 1, so we add "|| true" to avoid breaking the script.
num_matching_lines=$(grep -R -h -I "$searchstr" "$filesdir" 2>/dev/null | wc -l)

echo "The number of files are $num_files and the number of matching lines are $num_matching_lines"
exit 0