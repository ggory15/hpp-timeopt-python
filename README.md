# Time-Optimization for loco-3d
This is module of time-optimization by B. Ponton for using loco-3d framework.

- Install Procedure
	To use this in Ubuntu 14.04 and 16.04

	(1) Download and intstall the original time-optimization.
	```
	git clone https://git-amd.tuebingen.mpg.de/bponton/timeoptimization
	   cd timeoptimization
	   ./src/catkin/third_party/catkin/bin/catkin_make
	   source ./devel/setup.bash
	```
	(2) Download and intstall the python binding module for time-optimization 
	```
	    git clone https://github.com/ggory15/timeopt --recursive
	    mkdir build && cd build
		cmake .. -DCMAKE_BUILD_TYPE=RELEASE
		make install
	   
	    To complie this, the binary of the original time-optimization of step#1 is required.
	    (In Cmakelists, you can see ADD_REQUIRED_DEPENDENCY("momentumopt"))
	```
	(3) Run Script

	```
	    ipython /script/biding_test.py
```




