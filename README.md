# COP4520_pa2
Code for Programming Assignment 2 of class COP4520

To change the sample number of guests, change the numGuests variable in either program.

Problem 1 is located under labyrinth_birthday.cpp
Problem 1 uses the solution of having the first guest (thread) always be the designated checker of the cupcakes and who has entered the labyrinth or not. Every time the first thread notices that the cupcake has been eaten, it updates the counter by 1. When the counter reaches the number of guests, the program returns as every guest is confirmed to have entered the labyrinth at least once. The other threads only eat the cupcake once, to ensure the first thread marks only one time having entered the labyrinth. 

Problem 2 is located under crystal_vase.cpp.
Problem 2 uses the second solution given, where the guest inside the vase showroom marks that they are in the showroom, preventing other guests from entering. Having visited the showroom, they are inserted into an array to track who has visited the showroom. When the size of the array is the size of the number of guests, the program returns. This solution may not be as efficient as using a queue, but it benefits from being simple to implement and understand. 