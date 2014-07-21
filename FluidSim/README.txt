Fluid Sim Version 1.0

Author: Kirk Wallace

This fluid simulator's focus is on the physical movement of the fluid and comparing differences in methods for tracking fluid motion
and implementing new physical theories to render both large and small bodies of water accurately. The end goal is a small application 
where the user will be able to select the algorithm used to calculate the fluid's  motion. 

References:
r1) "Fluid Simulation for Computer Graphics" - Robert Bridson 2008
r2) "Fundamentals of Multiphase Flow" - Christopher E. Brennen 2005
r3) "Fluid Dynamics and Transport of Droplets and Sprays" - William A. Sirignano 2014


Requirements:
This simulator uses openGL, GLUT, and GLEW. Developed using openGL version 4.3, freeGLUT version 2.8.1, and GLEW version 1.10.0

Installing freeGLUT
Visit www.cs.uregina.ca/Links/class-info/315/WWW/Lab1/GLUT/windows.html or follow the steps below.
1) Get the freeglut 2.8.1 MSVC zip file from http://www.transmissionzero.co.uk/software/freeglut-devel/
2) Extract the files to your PC.
3) There are three types of files of importance: lib (in the lib folder), header (in the include\GL folder), and dll (in the bin folder).
4) If you are using 32-bit Windows with Visual Studio 2012, the following are the easiest places to put each of these files:
      []lib files: C:\Program Files\Microsoft Visual Studio 11.0\VC\lib
      []Header files: C:\Program Files\Microsoft Visual Studio 11.0\VC\include\gl
      []dll files: C:\WINDOWS\System32
5) If you are using 64-bit Windows Vista or Windows 7 with Visual Studio 2012 do the following:
      []Header files: C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\include\gl
      []32-bit lib files: C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\lib
      []32-bit dll files: C:\WINDOWS\SysWOW64
      []64-bit lib files: C:\Program Files (x86)\Microsoft Visual Studio 11.0\Windows\VC\libs\amd64
      []64-bit dll files: C:\WINDOWS\System32
6) Installing GLEW: download GLEW and unzip to a location of your choice.
7) Locate the headers, glew32.lib, and glew32.dll files and install them to the same locations you used for freeGLUT
8) Setting up your project in MSVC 
		[1] Right click the project in the solution explorer (not the "solution" or code files or anything else)
		[2] Go to Project -> Properties
		[3] expand "Configuration Properties"
		[4] expand the "Linker" folder
		[5] select "Input"
		[6] add these additional dependencies: opengl32.lib;glu32.lib;freeglut.lib;glew32.lib
9) Now all you have to do is include <GL/freeglut.h> for freeGLUT, <GL/glew.h> for glew (must include this one first) 


Use this website to install freeGLUT and GLEW in MSVS
http://www.cs.uregina.ca/Links/class-info/315/WWW/Lab1/GLUT/windows.html

Try using this for physically based rendering
https://github.com/mmp/pbrt-v2