## CG_Assignment_2_Q3

About
===
Aliasing
---
How to Use
===
1. Download zip piles.  
   
![download zip](https://github.com/user-attachments/assets/3e76e9d2-5325-42a3-ba52-2bb3064c0a58)

2. Unzip the folder  
3. open "OpenglViewer.sln"  
![leanch](https://github.com/user-attachments/assets/1ed43ef3-d812-4b75-809d-fe1077eabf9b)
---
Result of assignmet2- Q3

![result3](https://github.com/user-attachments/assets/bdf3c278-0c6a-44d8-8cd0-9c0d1bcf5dbc)  
---
Explanation
---
#include <random>: Adds a header file for random number generation.  
Create a random seed using std::random_device rd.  
Initialize the Mersenne Twitter random engine using std::mt19937 gen(rd()).  
Use std::uniform_real_distribution<> dis(0.0, 1.0); to create an even distribution between 0.0 and 1.0.  
offset_x, offset_y calculation: produces a random offset between -0.5 and 0.5 within the pixel.  
Modifying the getRay() call: When generating a ray from the camera, apply a random offset to the pixel coordinates to generate the ray at a random location within the pixel.  
Accumulate and average final_color: Accumulate the color of all rays generated for each pixel in final_color, and divide by num_samples to calculate the average color.  
![anti](https://github.com/user-attachments/assets/e1b4d552-7bf0-4de2-b0e5-f3789e9f1def)  
![anti concept](https://github.com/user-attachments/assets/c2c92251-93cc-40c9-a8b4-f305c07c14c8)
