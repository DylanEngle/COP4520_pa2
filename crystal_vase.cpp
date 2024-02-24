#include <array>
#include <chrono>
#include <deque>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>
#include <unordered_set>

#define numGuests 25

using namespace std;

// true = available, false = unavailable
bool showroomAvailable = true;
// keeps track of people who've seen the vase already
unordered_set<thread::id> guestsVisited{};
mutex mtx;

// generates random number 
int generateRandomNumber(int min, int max) {
    random_device seed;
    mt19937 rng(seed());
    uniform_int_distribution<mt19937::result_type> dist(min, max);
    return dist(rng);
}

void admireVase(int threadIndex) {
    thread::id threadId = this_thread::get_id();

    while (guestsVisited.size() < numGuests) {
        mtx.lock();

        if (showroomAvailable && guestsVisited.find(threadId) == guestsVisited.end()) {
            showroomAvailable = false;
            cout << "Guest #" << threadIndex + 1 << " is currently viewing the vase" << endl;
            this_thread::sleep_for(chrono::milliseconds(generateRandomNumber(10, 300)));
            showroomAvailable = true;
            guestsVisited.insert(threadId);
        }

        mtx.unlock();
    }
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    array<thread, numGuests> threads{};

    for (size_t i = 0; i < threads.size(); i++) {
        threads[i] = thread(admireVase, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration<double, milli>(end - start);

    std::cout << "All guests have viewed the vase." << endl;
    std::cout << "Finished in " << duration.count() / 1000 << "s" << endl;
}