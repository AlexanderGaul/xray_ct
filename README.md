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
Qt5. We use CMake (minimum version 3.5.1) to  
manage our program's structure to simplify changes 
in it.

### How to run

The program can be executed typing

<<executable_name>> <<path_to_volume.edf>>

into a terminal.
It is important to use only valid paths to .edf files 
containing three dimensional volumes.

### Acquisition widget manual

In the acquisition widget, the user can see the position of 
X-ray emitter and detector (who form an 
acquisition pose together) from three all axes.
Using the keyboard arrow keys, the acquisition pose can be rotated 
all around the volume, the position of emitter and detector are 
updated accordingly. The user can also show the rays between 
the emitter and the pixels of the detector.

The user can add and remove acquisition poses by clicking "store current pose" and "delete current pose". These poses are then used for 
reconstruction.

In experiments, we found out 20 circles of poses with 20 poses per 
circle (maximum) are sufficient to reconstruct all information 
of a 10x10x10 volume (using 20 iterations of CG).

It would be cumbersome to enter all these poses manually - therefore
we made it possible to generate poses these poses automatically. The 
user has to provide the number of circles and the number of poses 
per circle only.

All positions of acquisition poses' emitters are shown on the screen, 
to convince the user of a regular sampling of the volume.

On the right side of the screen, the user can see the current acquisition
data (the image that reaches the detector in the current acquisition 
pose). It is also possible to show all acquisitions in a 
linearized way. 

TODO SIMON: IS ALL ACQUISITONS DESCRIPTION CORRECT?

### Reconstruction widget manual

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
- The user has the possibility to add a custom amount of noise to the
acquisition (to simulate the situation in real world scenarios).
- Regularization can be enabled to reduce the impact of noise on 
reconstruction results. The user has the possibilty to enter a 
custom value for lambda, to change the 
regularization parameter of the Tikhonov regularization.
Warning: Too high values for lambda can degrade the reconstruction 
results to a level below the noisy image!

After changing the setup of reconstruction, the user has to hit 
"Update reconstruction" to apply his changes and recompute the 
reconstructed volume using the new setup. 

### Visualization widget manual

TODO