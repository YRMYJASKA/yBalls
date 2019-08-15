# yBalls
An ideal ellastic circular object collision physics simulator (in real time)

## The Idea
There are a certain amount of balls trapped inside a window of certain dimensions with certain velocities.
yBalls is responsible for computing the collisions and subsequent new velocities of all these balls.

## Requirements
- ```g++```  
- ```make```  
- ```SDL2``` C++ library 

## Building 
1. ```git clone https:://github.com/yrmyjaska/yBalls && cd yBalls```
2. ```make```
3. Run yBalls with ```./yballs```

## Structure of Config files
Configuration files that are passed alongside the '-f' parameter will define a preset set of balls within yballs. These configuration files are wholly customisable
when it comes to the qualities of the balls. They can also be used to determine the dimensions of the yballs window in which the simulation will take place.
The configuration files come in the following format (and usually with a '.yballs' extension):

	WIDTH <some int>
	HEIGHT <some int>
	<x-pos> <y-pos> <radius> <mass> <velocity x> <velocity y>
	...

See the [examples](examples/)
