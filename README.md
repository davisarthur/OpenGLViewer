# OpenGLViewer
The main program of my project is ```render.cpp```. This program will render one of five models using Phong or
Gouraud shading. The user is able to choose the shading method and the model from the terminal window. Once the
program renders the model, the user can scale the model up and down by pressing U or D on the keyboard. The user
can also rotate the model using the X, Y, and Z keys. Finally, the user can save the z-buffer to a txt file by
pressing P. These save files will be stored in the ZBufferSaves folder by default. 

Use the following command to compile this program on Mac:
```
g++ -lglfw -lglew -framework OpenGL helperFunctions.cpp render.cpp -o render.o
```
I do not own a Windows or Linux machine, but I believe the following command can be used for compilation on those platforms:
```
g++ -lglfw -lglew helperFunctions.cpp render.cpp -o render.o
```
Once compiled, the program can be run using the following command: ```./render.o```

## Plot Z Buffer
After saving the contents of the z-buffer to a text file, you may want to plot the z-buffer values to further analyze
the rendering. My python script ```plotZBuffer.py``` can be used to generate this plot. When running this program,
the user will be prompted to enter the index of the z-buffer save file they would like to plot. For instance if the
user would like to read from a file stored at the path ```ZBufferSaves/save3.txt```, the user should input 
```3```.

In order to run my script, you must have the matplotlib library installed. This can be installed using the following
command: ```pip3 install matplotlib```

Once matplotlib is installed, my program can be run via the following command: ```python3 plotZBuffer.py```


