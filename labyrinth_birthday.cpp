#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>

#define numGuests 40
using namespace std;

//keeps track of whether guests in the labyrinth have eaten the cupcake or not
array<bool, numGuests> guestsCupcakeStatus;
mutex mtx;
bool isCupcakeReady = true;
int cupcakesEaten = 0;

// Keeps track of which guest is currently in the labyrinth
int runningThreadIndex;

// generates random guest to visit the labyrinth
int generateRandomNumber(int min, int max) {
    random_device seed;
    mt19937 rng(seed());
    uniform_int_distribution<mt19937::result_type> dist(min, max);
    return dist(rng);
}

//checks status of cupcake, and updates counter whenever cupcake eaten
void firstThreadRole() {
    while (cupcakesEaten < numGuests) {
        mtx.lock();

        if (runningThreadIndex == 0) {
            //cupcake has eaten by a guest, need to replace the cupcake and update counter
            if (!isCupcakeReady) {
                cupcakesEaten++;
                isCupcakeReady = true;
            }
            
            //first thread must eat the cupcake as well
            if (isCupcakeReady && !guestsCupcakeStatus[0]) {
                cupcakesEaten++;
                isCupcakeReady = true;
                guestsCupcakeStatus[0] = true;
            }
        }

        mtx.unlock();
    }
}

//all other threads besides the first one perform this function
void visitLabyrinth(unsigned int threadIndex) {
    while (cupcakesEaten < numGuests) {
        mtx.lock();

        if (runningThreadIndex == threadIndex && isCupcakeReady && !guestsCupcakeStatus[threadIndex]) {
            isCupcakeReady = false;
            //mark them in bool array so they don't eat another cupcake
            guestsCupcakeStatus[threadIndex] = true;
            cout << "Guest #" << threadIndex << " has eaten the cupcake." << endl;
        }

        mtx.unlock();
    }
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    array<thread, numGuests> threads{};

    //first thread will be keeping track of the others by checking cupcake status
    threads[0] = thread(firstThreadRole);

    //for the other threads, they will be doing visitLabyrinth whenever randomly selected
    for (size_t i = 1; i < threads.size(); i++) {
        threads[i] = thread(visitLabyrinth, i);
    }


    //keep picking guests at random until all guests have been counted as eating the cupcake
    while (cupcakesEaten < numGuests) {
        runningThreadIndex = generateRandomNumber(0, numGuests);
    }

    //rejoin all the threads
    for (auto& thread : threads) {
        thread.join();
    }

    //calculate total algorithm length
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration<double, milli>(end - start);

    cout << "All " << cupcakesEaten << " guests have entered the labyrinth." << endl;
    cout << "Finished in " << duration.count() / 1000 << "s" << endl;
}