# ECE 565 Homework 5: Rainfall Simulation
Usage:  
    1. Install boost library on Ubuntu:  
        ```sudo apt-get install libboost-all-dev```  
    2. Build executables:  
        ```make```  
    3. Run program:
    <ul>
    <li>P = # of threads</li> 
    <li>M = # of simulation time steps with rainfall</li>
    <li>A = absorption rate  </li>
    <li>N = dimension of lanescape</li>
    <li>elevation_file = file name</li>
    </ul>
        ```./rainfall <P> <M> <A> <N> <elevation_file>```  
    4. Validate results:  
        ```python ./check.py [dimension] [validation file] [output file]```

