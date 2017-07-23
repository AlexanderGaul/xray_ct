# Final Project of Lab Course: Image Reconstruction and Visualization using C++

## Team Members of Team 6:
 - Alex Gaul
 - Simon Griebel
 - Maximilian Hornung

## Table of Contents

- Dependencies
- How to run
- Acquistion widget manual
- Reconstruction widget manual
- Visualization widget manual

### Dependencies

Our project depends on both Eigen3 and 
Qt5. 

We use CMake (minimum version 3.5.1) to  
manage our program's structure to simplify changes 
in it.

For documentation generation, we use doxygen (>1.8.0).

If available, at least the forward projection is parallelized with OpenMP.

### How to run

The program can be executed typing

./part2 path_to_volume.edf

into a terminal (at the location of the executable, which is
called part2 by default).
It is important to use only valid paths to .edf files 
containing three dimensional volumes.

The documentation is generated during the build process 
automatically, but it can also be updated by the 
command "make doc" in the build folder.

To view the documentation, open docs/html/index.html (relative 
to the location of the build folder). This will open a doxygen 
documentation of our project, including class hierarchies, 
class member overview etc.

### AcquisitionWidget manual

In the acquisition widget, the user can see the position of 
X-ray emitter and detector (who form an 
acquisition pose together) from three all axes.
After clicking on one of the poses, the current acquisition pose can be rotated with the arrow keys. 
The user can also show the rays between the emitter and the pixels of the detector.

The user can store the currently modified pose, for the use in the reconstruction, 
which means that a new pose that can be modified/stored again is generated.
The currently modified pose can also be deleted, so the last stored pose becomes the new current pose.

Poses can also be automatically generated. This is done by generating a certain amount of circles consisting of poses around the volume. 
These circles are themselves generated in a (half-)circle around the aquisition. 
The closer these circles get to the ends of this half circle the smaller the pose-circles get, 
which means that all the generated pose form a rough sphere.
The user has to provide the number of circles and the (maximum) number of poses per circle.

In experiments, we found out 30 circles of poses with 30 poses per 
circle (maximum) are sufficient to reconstruct all information 
of a 10x10x10 volume (using 40 iterations of CG for reconstruction).

To simulate different real-world imaging setups in our software, we
added the possibility to adjust the resolution and the size of the 
detector. It is also possible for the user to change the distance 
between detector and volume. This distance is relative to the volumes 
size, such that similar distances lead to similar behavior 
on different volumes.

All positions of stored acquisition poses' sources are shown on the screen as faded out red circle, 
while the brightest red circle is the position of the currently modified pose.

On the right side of the screen, the user can see the current acquisition
data (the image that reaches the detector in the current acquisition 
pose). It is also possible to show the data of all acquisition poses at the same time, 
where each line of pixels is the linearized result of each projection.

### ReconstructionWidget manual

The top part of this widget shows the reconstruction results. 

To perform the reconstruction using Conjugate Gradient (CG) algorithm, 
press "Load Acquisition". 
This task is computationally expensive and can take several
seconds. After the computations have finished, it is possible
to scroll through the reconstructed volume (along all three 
axes, which can be chosen using a slider). In each slice the ratio between the lengths of
the visible sides of the voxels is preserved but the actual lengths may differ from
perspective to perspective.

There are several possibilities of adjusting the reconstruction
setup in the lower part of this widget:

- The number of iterations for CG can be changed - more iterations
improve the results, but also increase the computational effort. 
We recommend using 40 iterations.
- The user has the possibility to add a custom amount of noise to the
acquisition (to simulate the situation in real world scenarios).
- Regularization can be enabled to reduce the impact of noise on 
reconstruction results. The user has the possibilty to enter a 
custom value for lambda, to change the 
regularization parameter of the Tikhonov regularization.
Warning: Too high values for lambda can degrade the reconstruction 
results to a level below the noisy image!
We recommend to set lambda between 10,00 and 20 for an error of 4%  (or similar values).

After changing the setup of reconstruction, the user has to hit 
"Update reconstruction" to apply his changes and recompute the 
reconstructed volume using the new setup. The error is only recalculated (or removed) when "Update Noise" is presesd.

### Visualization widget manual

In the visualization widget, we implemented the multiplanar 
reconstruction (MPR) and the direct volume rendering (DVR)
using maximum intensity projection (MIP). 

On the left side of the widget, the user has the possibility to 
configure the setup. He can both load a volume from a file and
use previous computed reconstruction results for visualization.

For DVR, we implemented not only the orthographic view, 
but also the perspective one. The user can zoom and rotate both 
kinds around Y and Z axis. Additionally, it is possible to 
influence the visualization results by changing the step size.
The user is able to focus on the important medical features by
changing the range of the linear ramp transfer function as well 
as the color of the visualization result.

In the MPR widget, we implemented also several extensions.
It is possible for the user, to rotate around the volume along 
Y and Z axis, as well as adjusting the position of view (initially,
this position is set to the origin, like in the original task). 
Additionally, it is possible to zoom and rotate around Y and Z axis,
such that the user can view any plane in the volume from any direction.
In the same way as in the DVR visualization, the MPR's transfer function
can be changed regarding range and color.
To better show where the cutting plane is currently located, the current rotation of it compared to the initial state is shown directly below the visualization.
