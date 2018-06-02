# opengl_render_img_in_a_specific_view
According to the given camera intrinsic and extrinsic, use OpenGL to render image in such view. 

## Dependencies
OpenGL  
OpenCV  
You can modify following property files to your own opencv openggl include path, lib path, and used lib name  
"OPENCV_64_D.props\"   
"OPENCV_64_R.props\"  
"OPENGL_64_D.props\"  
"OPENGL_64_R.props\"  


## input 
1. E.txt:  
----8 cam intrinsics(only one is used in the example code)  
----format: r11 r21 r31 r12 r22 r32 r31 r32 r33 tx ty tz  
2. K.txt:  
----8 cam extrinsics(only one is used in the example code)  
----format: fx 0 0 0 fy 0 cx cy 1   
3. test.obj_:  
u should change the filename to test.obj to view the model in Meshlab  
the input model file


## output
the corresponding rendered image in such veiw.  
it will be saved to 'output' folder.


## author
if the code presented helps u, it literally is a delight to me.  
