# RayTracing
C program to draw three spheres on a checkered background using RayTracing

raymp.c when ran will draw three spheres on top of a checkered floor against a sky background in a 1920x1080 ppm file. A simple ray tracing algorithm is used to draw reflections and shadows within the spheres and floor using a predertmined eye and light position within the frame. More about the algorithm used can be read here https://cs.stanford.edu/people/eroberts/courses/soco/projects/1997-98/ray-tracing/implementation.html.

This program can be ran in parallel on the CPU using OpenMP. This will have pixels calculate their color values in parallel based on the number of threads assigned to OpenMP. More can be read on how to use OpenMP here https://cs.stanford.edu/people/eroberts/courses/soco/projects/1997-98/ray-tracing/implementation.html.

Project created in 2018-2020 with help of Dr. Shane Torbert in TJHSST's Parallel Computing Class.

