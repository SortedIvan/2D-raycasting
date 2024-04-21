# 2D-raycasting
A simple exercise for dynamic 2D raycasting for a ray with multiple line segments intersections. 

It works by generating direction vectors by going over 2PI (rotating 360 degrees) and then using each of those directions to create a seperate Ray object.

It further includes a simple line segment editor where a user can draw different line segments on the screen by pressing shift to toggle the edit mode and shift to toggle it back off.

The ray with line segment intersection works by solving a simple parametric equations problem using an inverted matrix, created by the ray and the line segment. Furthermore, the algorithm chooses the closest intersection point for a ray, ensuring that the rays work properly for stacked line segments.

The linear algebra derivation (done on paper) can be found here:

