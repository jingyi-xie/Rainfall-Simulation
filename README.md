# ECE 565 Homework 5
Usage:
    1. Install boost library on Ubuntu: 
        `sudo apt-get install libboost-all-dev`
    2. Build executables:
        `make`
    3. Run program:
    <!-- P = # of threads
    M = # of simulation time steps with rainfall
    A = absorption rate
    N = dimension of lanescape
    elevation_file = file name
    -->
        `./rainfall <P> <M> <A> <N> <elevation_file>`
    4. Validate results:
        `python ./check.py [dimension] [validation file] [output file]`

