# code/controller

This folder contains the sketches that run on each of the building controllers. Since they are intended to be essentially interchangeable (aside from the pipe topology) each sketch folder contains only a minimal `.ino` file,
a `pipes.cpp` file that maps the unique LED addresses for each building to the pipe topologies, and an `update.bat` file that copies the all files from the `controller-template/` directory into the sketch. 

`update-all.bat` in the main folder runs each update batch script. **You must run the update scripts before the sketches will compile.** Any changes made to files not tracked by git in the sketches should be made in the
template folder instead so that it can be shared across all sketches.
