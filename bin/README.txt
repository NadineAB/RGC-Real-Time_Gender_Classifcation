Executable with the required data to run are provided in this package. 

How to run it:

0. Open the executable
  0.1 is it does not open, try installing Microsoft Visual C++ 2010 Redistributable Package (x86). you can find it here: http://www.microsoft.com/downloads/en/details.aspx?FamilyID=a7b7a05e-6de6-4d3a-a423-37bf0912db84

1. put one face in the webcam feed, and press "capture training image"
  1.1 give it a unique name (your face will be stored in the images folder).

2. repeat step 1 for at least 2 different faces, preferably more.

3. press the button "train recognizer", you should get a pop-up that shows you if it has finished successfully.

4. put one of the faces you used for training in the webcam feed, and press recognize, the name of the closest image should pop-up in the log window at the bottom.

(yes, this app needs some work, but it's a proof of concept for the cognition framework)
