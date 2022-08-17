# c_bmp_processing
A simple console program to proccess BMP images with C language

# Table of Contents
1. [Getting Started](#start-description)
2. [Building/Testing the Project](#build-description)
3. [Using the Project](#use-description)

<a name="start-description"></a>
## Getting Started

The current project is a mini interactive **cli** to modify a bmp image:

You can use the following commands after running the program:

* **save** "path" - to save the bmp image into the specified path
* **edit** "path" - to load a bmp image into the program and to modify it
* **insert** "path" y x - to overlap the image specified at **path** over the loaded one. The overlapping start from the left-down point [y, x]
* **set draw_color** R G B - to set a new color for the brush
* **set line_width** x - to set a new size for the brush, the new size must be odd
* **draw line** y1 x1 y2 x2 - to draw a line on loaded image between specified points
* **draw rectange** y1 x1 width height - to draw a rectangle of specified width and height where [y1, x1] is the left-down corner
* **draw triangle** y1 x1 y2 x2 y3 x3 - to draw a triangle on loaded image between the three specified points
* **fill** y x - to fill a region with the current brush color starting from [y, x] point
* **quit** - to free the loaded memory and to quit the program

<a name="build-description"></a>
## Building/Testing the Project

In order to build the project make sure that you have the **gcc compiler** installed, if not you will have to build the files by yourself.

That make sure that you cloned the project and changed the directory into it:

```BASH
    cd build
    make
```

After above commands you will get 1 executable:

* bmp

To clean every junk files after working with files run:

```BASH
    make clean
```

To test how the program works you must run:

```BASH
    ./bmp_run.sh
```

I encourage you to look on the input and reference folders to see how the input data is represented and how the output is printed.

<a name="use-description"></a>
## Using the Project

In order to work with project make sure that you build the project.

Now let's see how to write an instruction:

1. First of all you have to load a image into the program:

```BASH
    ./bmp
    edit path/to/image/my_image.bmp
```

2. When you are done with modifing your image you have to save it and to quit the program

```BASH
    ./bmp
    edit path/to/image/my_image.bmp
    ..... Modify ......
    save path/to/image/another_name.bmp # or save path/to/image/my_image.bmp
    quit
```

3. In order to modify the current loaded image you should look on input examples and output examples
from the build folder to see how to write **correctly** the instructions and what to expect as output
## Hope you will enjoy your mini cli program to process bmp images
