3d-visualization
================
The sfs-visualizer is a GPU-based software for realtime calculation and visualization
of threedimensional soundfields

Mainfeatures
--------------------------

* im- and ~export of .mat files with WxHxD-matrizes
* interactive calculation of monofrequent soundfields via Greens Function
* usage of precalculated or custom sourceconfigurations
* adjusting the position of the primary soundsource
* easy extensible by GLSL-scripts
* taking screenshots
* for fieldvisualization two rendermethods are available (shearwarping and raymarching)
* userdefinable colormapping

Requirements
---------------------------

* Windows pc with an OpenGL 4.3 (compute shaders) enabled videocard 

Usage Examples
----------------------------

* display matlabfile 'data.mat':

	sfs-visualizer -m data.mat

* display multiple matlabfiles for animation:

	sfs-visualizer -m data1.mat -m data2.mat -m data3.mat...

* calculate a 128^3 field:

	sfs-visualizer -W 128 -H 128 -D 128
	
* calculate a field with custom sources 

	sfs-visualizer -s sources.csv

* use specific propertysettings
	
	sfs-visualizer -p properties.csv

Commandline Options
----------------------------

| longform			| shortform | description										|
| :----------------	|:--------- | :------------------------------------------------	|
| matlabfile		| m			| the matlabfilename(s) to be viewed				|
| glslfolder		| g         | the folder containing the glsl-files				| 
| sourcedefinition	| s			| the csv-file with sourcedefinitions				|
| colormap          | c			| a bitmap used for colormapping					|
| properties        | p			| a csv-file with propertyvalues					|
| gridWidth			| W			| the Width of the calculation-grid e.g. 300		| 
| gridHeight		| H			| the Height of the calculation-grid e.g. 300		|
| gridDepth			| D			| the Depth of the calculation-grid e.g. 300		| 
| interval			| i			| updateintervall for animations in milliseconds	|  
| workgroupsize		| w			| Workgroupsize for Compute Shader < 14				|
|					|			| (should divide WxHxD without rest)				|
| fullscreen		| f			| programm starts in fullscreenmode (experimental)	|

Interactive Options
---------------------------

### general options

| key(s)	| description																|
| :--------	| :------------------------------------------------------------------------	|   
| 2/"		| change colormapping														|			
| 4			| toggle raytrace (may crash sometime...)									|
| 5			| screenshot																|
| 6			| reset camera																|
| 7			| save current state to properties.csv or overrides the loaded properties -p|
| j			| export current field to data.mat											|
| b/B		| amplification																|
| c			| cut-planes/full															|
| i/I		| show info																	|
| ESC		| Exit																		|
| ./:		| change calculation-interval												|
| -/_		| change calculation-timestepsize											| 
| u/U		| reverse Animation															|
| SPACE		| Stop/Start Animation														|			
| m/M		| brightness																|
| o/O		| speed of autorotate around y-axis											|
| o/O + ctrl| speed of autorotate around x-axis											|
| o/O + alt	| speed of autorotate around z-axis											|
| mousewheel + left Ctrl		 | Opacity						|
| mousewheel + left Ctrl + umsch | Opacity of cutted area		|

### raytrace options

| key(s)			| description					|
| :----------------	| :----------------------------	|   
| y/Y				| threshold						|			
| </>				| alpha premultiplier			|
| x/X				| quality (very expensive)		|

 ### options with matlabfiles

  mouse: left=rotation (x|y), right:  zoom + rotation (z), wheel:  move z-cut-plane

 #### textureblend options:
       
| key(s)			| description							|
| :----------------	| :------------------------------------	|   
| wheel + left umsch| move x-cut-plane						|			
| wheel + left Ctrl | move y-cut-plane						|
| r/R				| change number of textureslices		|
              
                               
### options with GPU calculation
              
mouse: left=rotation (x|y), right:  zoom + rotation (z),wheel=frequency
 
 #### calculation options
 
| key(s)						 | description					|
| :----------------------------- | :---------------------------	|   
| +/*							 | add/remove source			|			
| Arrowkeys						 | move virtual source			|
| p/P							 | size scaling in m			|
| mousewheel + left umsch		 | Change Phase					|

            

        



       