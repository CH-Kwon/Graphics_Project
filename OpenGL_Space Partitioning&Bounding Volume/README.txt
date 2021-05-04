* Open the Assignment1.sln and build it with 32bit
   When you build the project, imgui box appears. 
   don't care about a solution name(Assinment1) becuase it is prolonged version of previous one.
   It is recommended to run the solution with Release mode(faster).
   Computing bsp tree takes too much time. So, I set models with same mid poly
	

   Imgui1
  -Control Rotating : bool type that stop or rotating light spheres.
  -Control Light Shperes : You can select number of light spheres.
  -Normals : You can select vertex or face normal.
  -Bounding volume methods
  -Octree and BspTree
   Imgui2
  - BVH Depth Control with top-bottom method
   Imgui3
  - LightTypes
 
* You must build the project by win32 platform(x86) for either Debug or Release mode.

*  Completed:      Completed Assignment1, Completed Bounding volumes and Top bottom Hierarchy
		Scene setup completed
		Implementing Octree completed
		Implementing BSPtree completed
		Toggle(interactivity) with different color in both octree and bsp
                    
* in the Assignment1 folder.
  Camera.cpp, Camera.h, Light.h, main.cpp, Model.cpp, Model.h, Shader.cpp, Shader.h
bspTree.cpp, bvhTree.cpp, Octree.cpp, bspTree.h, bvhTree.h, Octree.h


* You can handle camera by 'w' 'a' 's' 'd' keys and mouse. 