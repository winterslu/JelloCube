Jello Cube Simulation:

All files are under Xcode project.
Please run program in MacOS system.
Program name JelloCube.
Video name: "Jello Cube Record 30s"

Key functions:
	Press down 'k' key will push the cube up along with the z-axis 
	Press down 'l' key will pull the cube down along with the z-axis
	Press down ',' key will push the cube left along with the y-axis
	Press down '.' key will pull the cube right along with the y-axis

Extra OpenGL render

1. Implemented AA (Anti-Aliasing) by using Multi-Sampling in OpenGL
2. Transparent Jello Cube using Blend function in OpenGL
	Blend function is glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_COLOR);
	Adding negative destination (background) color to front object to blend color.
3. Adjust lighting effect.
4. In-box Inclined Plane.


Submitted by Ke Lu 1670629406
If there is any question, please feel free to contact me through luke.s.winters@gmail.com
or USC mail, kelu@usc.edu