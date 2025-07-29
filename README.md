<b>Name:</b> Yanisa Srisa-ard

<b>UCID:</b> ******96

The source files have been uploaded as a zip file into the dropbox (only the main file was edited). 

<u>Fractal types included in the program:</u>
* Sierpinsky Triangle
* Pythagoras Tree
* Koch Snowflake
* Dragon Curve
* Hilbert Curve (bonus)

<u>Keyboard controls:</u>
* <b>RIGHT ARROW:</b> Go to the next fractal.
* <b>LEFT ARROW:</b> Go to the previous fractal.
* <b>UP ARROW:</b> Increase depth of the current fractal (max. 10).
* <b>DOWN ARROW:</b> Decrease depth of the current fractal (min. 0).

<b>Operating System:</b> macOS

<b>CMake version:</b> 3.30.3

<b>Note:</b> I wrote my original code using VScode on my mac laptop. However, I have edited it using the computers in the lab room, therefore it runs perfectly on the computers' <b>VScode</b> (in the lab room), not the regular Visual Studio.

Instructions to compile the program:
1. Open the folder (453-skeleton-a1-completed) using VScode.
2. Select the 'Unspecified' kit (if needed).
3. Build the program using CMake (exit code should be 0).
4. Open a new terminal.
5. Enter the following: 
```bash
cd build
./453-skeleton
```
6. Toggle between the different fractals and their depths using the arrow keys.
