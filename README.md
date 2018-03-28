# 389GraphicsProject  
End result: art museum  
End lines with 2 spaces for formatting  

1/17  
Possible sculpture/model: "Big Red" by Alexander Calder  
  https://exhibitioninquisition.files.wordpress.com/2010/10/big-red.jpg  

1/19  
Check files for simple, base picture-frame model (for art)  
  Frame includes raised edges and small decorations  
  40 vertices; 27 faces  

1/20  
Check files for simple Big Red mobile mesh  
  All curves have been turned to mesh for this version  
  Panels are either circles or "boomarangs"   
  Pieces have been rotated about the center for 3D effect  
  1766 vertices; 14 faces  

1/28  
Decided on museum theme: H. R. Giger  

2/1  
Included simple museum model under Models  

![alt tag](https://github.com/Chimera0x9424A3/389GraphicsProject/blob/master/Readme1.png)

![alt tag](https://github.com/Chimera0x9424A3/389GraphicsProject/blob/master/Readme2.png)


2/2  
Questions to answer for this week:  
a. What is the theme of your museum:  
    H.R. Geiger (for wall art) and BigRed for the model  
    Maybe an Alien egg pod or something instead if we have the time  
b. How many rooms will your museum have:  
    one main one with subdivisions/internal walls (check museum model)  
c. Have you started looking for textures for your museum walls/floors? If so, what are your plans:  
    Have a couple possiblities for floor and ceiling: stone/metal mix  
    Check textures folder for more info  
d. Will your museum have a ceiling:   
    Yes. Flat for part of it and dome for the middle (check museum model)  
e. You must have at least ONE “sculpture” in your museum. What are you planning to use for
 your sculpture(s):  
    BigRed model from the cover of the algorithms book  
    We currently have a simplified version in our Models folder
    Might switch to Xenomorph model if we have time  
f. You must have at least FOUR “paintings” in your museum. What were you planning to use for  
 your pictures? Have you already started looking for textures that can be applied as “pictures”?  
 Will your pictures be hung on the wall? Will they be murals? Do you have a completely different  
 idea of how these paintings will be added to your museum:
    All paintings/art of H. R. Geiger  
    Some hung on walls, possibly some murals as well, depending on size  
    Check wall art folder for more details  
    

2/13  
Check here for our museum model (subdivided into subpanels for easier texturing):  
https://github.com/Chimera0x9424A3/389GraphicsProject/blob/master/Models/MuseumSubpanels.blend  


2/22  
Some models are loading!!!  
Cube and other simple object  
Can toggle between two different models and display modes (wireframe/filled in)  
  Check ToggleTime.cpp  
  Next step(s): deal with normals; normalize incoming verts  
Next next step: maybe change the viewpoint?  

3/5  
The museum can load into the window!  
Looks a bit funny though: check into that  
Dealt with normalizing incoming verts (individually as they are read in, with normalize function)  
Maybe this is what messed up the look of the museum?  
Still tyring to figure out how to rotate around it  
Also have cube loading in as well--should remove that soon  
Check here for current code:  
https://github.com/Chimera0x9424A3/389GraphicsProject/blob/master/OpenGL/ProjectCode/MuseumFirstLoad.cpp  
Next step: clean up old code, create (and use!) .h file for functions  

3/27
OMG!!!!  
The museum is still there (after normalizing with blender)  
It has colors!  
It can rotate!  
It can translate!  
You can "move" toward it!  
All through the magic of matrices!!!  
Next steps: move the camera, fix "clipping" out of frame when zoomed in too far, add textures  
https://github.com/Chimera0x9424A3/389GraphicsProject/blob/master/OpenGL/ProjectCode/GettingThere
![alt tag](https://github.com/Chimera0x9424A3/389GraphicsProject/blob/master/Readme3.png)
