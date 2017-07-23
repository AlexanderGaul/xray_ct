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
Using the keyboard arrow keys, the acquisition pose can be rotated 
all around the volume, the position of emitter and detector are 
updated accordingly. The user can also show the rays between 
the emitter and the pixels of the detector.

The user can add and remove acquisition poses by clicking "store current pose"
and "delete current pose". These poses are then used for 
reconstruction.

In experiments, we found out 30 circles of poses with 30 poses per 
circle (maximum) are sufficient to reconstruct all information 
of a 10x10x10 volume (using 40 iterations of CG for reconstruction).

It would be cumbersome to enter all these poses manually - therefore
we made it possible to generate poses these poses automatically. The 
user has to provide the number of circles and the number of poses 
per circle only.

To simulate different real-world imaging setups in our software, we
added the possibility to adjust the resolution and the size of the 
detector. It is also possible for the user to change the distance 
between detector and volume. This distance is relative to the volumes 
size, such that similar distances lead to similar behavior 
on different volumes.

All positions of acquisition poses' emitters are shown on the screen, 
to convince the user of a regular sampling of the volume.

On the right side of the screen, the user can see the current acquisition
data (the image that reaches the detector in the current acquisition 
pose). It is also possible to show all acquisitions in a 
linearized way. 

### ReconstructionWidget manual

The top part of this widget shows the reconstruction results. 

To perform the reconstruction using Conjugate Gradient (CG) algorithm, 
press "Load Acquisition". 
This task is computationally expensive and can take several
seconds. After the computations have finished, it is possible
to scroll through the reconstructed volume (along all three 
axes, which can be chosen using a slider).

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
We recommend to set lambda to 10,00 (or similar values).

After changing the setup of reconstruction, the user has to hit 
"Update reconstruction" to apply his changes and recompute the 
reconstructed volume using the new setup. 

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
