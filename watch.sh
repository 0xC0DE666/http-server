#!/bin/bash

# Watch for changes in the src directory and recompile on modification
inotifywait -m -r -e modify,create,delete src | while read path action file; do
  echo "Detected $action on $file. Recompiling..."
  make;
done
