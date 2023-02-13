#include <random>
#include <functional>
#include <iostream>

using namespace std;

class MaxHeap {
public:
    MaxHeap(int MaxHeapSize) { // Max heap constructor
        MaxSize = MaxHeapSize;
        heap = new float[MaxSize+1];
        CurrentSize = 0;
    }
    ~MaxHeap() { // Max heap destructor
        delete[] heap;
    }
    MaxHeap& Insert(const float x) {
        // Insert x into max heap
        if (CurrentSize == MaxSize) {
            throw "There is no free space in max heap.";
        }
        // Find place for x
        // Counter i starts at new leaf and moves up the tree
        int i = ++CurrentSize;
        while (i != 1 && x > heap[i/2]) {
            // Cannot put x in heap
            heap[i] = heap[i/2]; // Move element down
            i /= 2; // Move to parent
        }
        heap[i] = x;
        return *this;
    }
    MaxHeap& DeleteMax(float x) {
        // Set x to max element and delete max element from heap
        if (CurrentSize == 0) {
            throw "Heap is empty.";
        }
        x = heap[1];
        // Restructure heap
        float y = heap[CurrentSize--]; // Last element
        // Find place for y starting at root
        int i = 1; // Current node of heap
        int ci = 2; // Child of i
        while (ci <= CurrentSize) {
            // Heap[ci] should be larger child of i
            if (ci < CurrentSize && heap[ci] < heap[ci+1]){
                ci++;
            }
            // Can we put y in heap[ci]?
            if (y >= heap[ci]) { // Yes
                break;
            }
            // No
            heap[i] = heap[ci]; // Move child up
            i = ci; // Move down a level
            ci *= 2;
        }
        heap[i] = y;
        return *this;
    }
    void Initialize(float a[], int size, int ArraySize) {
        // Initialize max heap to array a
        delete[] heap;
        heap = a;
        CurrentSize = size;
        MaxSize = ArraySize;
        // Make into a max heap
        for (int i = CurrentSize/2; i >= 1; i--) {
            float y = heap[i]; // Root of subtree
            // Find place to put y
            int c = 2 * i; // Parent of c is target location for y
            while (c <= CurrentSize) {
                // Heap[c] should be larger sibling
                if (c < CurrentSize && heap[c] < heap[c+1]) {
                    c++;
                }
                // Can we put y in heap[c]?
                if (y >= heap[c]) { // Yes
                    break;
                }
                // No
                heap[c/2] = heap[c]; // Move child up
                c *= 2; // Move down a level
            }
            heap[c/2] = y;
        }
    }
    MaxHeap& Contraction() {
        for (int s = CurrentSize; s > 1; s--) {
            float temp = heap[1];
            DeleteMax(heap[1]);
            temp += heap[1]; // Add old max element with new max element
            DeleteMax(heap[1]);
            Insert(temp);
        }
        return *this;
    }
    float Output() const {
        cout << "The final element of Max Heap is: " << heap[1] << endl;
        return heap[1];
    }
private:
    int CurrentSize, MaxSize;
    float *heap; // Element array
};

class MinHeap {
public:
    MinHeap(int MinHeapSize) { // Min heap constructor
        MaxSize = MinHeapSize;
        heap = new float[MaxSize+1];
        CurrentSize = 0;
    }
    ~MinHeap() { // Min heap destructor
        delete[] heap;
    }
    MinHeap& Insert(const float x) {
        // Insert x into min heap
        if (CurrentSize == MaxSize) {
            throw "There is no free space in min heap.";
        }
        // Find place for x
        // Counter i starts at new leaf and moves up the tree
        int i = ++CurrentSize;
        while (i != 1 && x < heap[i/2]) {
            // Cannot put x in heap
            heap[i] = heap[i/2]; // Move element down
            i /= 2; // Move to parent
        }
        heap[i] = x;
        return *this;
    }
    MinHeap& DeleteMin(float x) {
        // Set x to min element and delete min element from heap
        if (CurrentSize == 0) {
            throw "Heap is empty.";
        }
        x = heap[1];
        // Restructure heap
        float y = heap[CurrentSize--]; // Last element
        // Find place for y starting at root
        int i = 1; // Current node of heap
        int ci = 2; // Child of i
        while (ci <= CurrentSize) {
            // Heap[ci] should be smaller child of i
            if (ci < CurrentSize && heap[ci] > heap[ci+1]){
                ci++;
            }
            // Can we put y in heap[ci]?
            if (y <= heap[ci]) { // Yes
                break;
            }
            // No
            heap[i] = heap[ci]; // Move child up
            i = ci; // Move down a level
            ci *= 2;
        }
        heap[i] = y;
        return *this;
    }
    void Initialize(float a[], int size, int ArraySize) {
        // Initialize min heap to array a
        delete[] heap;
        heap = a;
        CurrentSize = size;
        MaxSize = ArraySize;
        // Make into a min heap
        for (int i = CurrentSize/2; i >= 1; i--) {
            float y = heap[i]; // Root of subtree
            // Find place to put y
            int c = 2 * i; // Parent of c is target location for y
            while (c <= CurrentSize) {
                // Heap[c] should be smaller sibling
                if (c < CurrentSize && heap[c] > heap[c+1]) {
                    c++;
                }
                // Can we put y in heap[c]?
                if (y <= heap[c]) { // Yes
                    break;
                }
                // No
                heap[c/2] = heap[c]; // Move child up
                c *= 2; // Move down a level
            }
            heap[c/2] = y;
        }
    }
    MinHeap& Contraction() {
        for (int s = CurrentSize; s > 1; s--) {
            float temp = heap[1];
            DeleteMin(heap[1]);
            temp += heap[1]; // Add old min element with new min element
            DeleteMin(heap[1]);
            Insert(temp);
        }
        return *this;
    }
    float Output() const {
        cout << "The final element of Min Heap is: " << heap[1] << endl;
        return heap[1];
    }
private:
    int CurrentSize, MaxSize;
    float *heap; // Element array
};

int main() {
    try {
        default_random_engine generator;
        chi_squared_distribution<float> my_distribution(0.5); // Change the limits of randomness to see difference in the decimal point precision
        auto random_num= bind(my_distribution, generator);
        int N;
        do { // Check for valid number of elements
            cout << "Give the number of floating point numbers to be inserted in the heaps." << endl;
            cin >> N; // Execution time for N = 10.000.000 is 20 seconds
        } while (N<=0);
        auto *heap_min = new float[N]; // Both arrays are created dynamically, as described in requirements
        auto *heap_max = new float[N];
        for (int i = 0; i < N; i++) {
            heap_min[i] = random_num() * 10000.0;
        }
        for (int i = 0; i < N; i++) { // Copy heap_min to heap_max, as described in requirements
            heap_max[i] = heap_min[i];
        }
        auto *MinH = new MinHeap(N);
        auto *MaxH = new MaxHeap(N);
        MinH->Initialize(heap_min, N, N+1);
        MaxH->Initialize(heap_max, N, N+1);
        MinH->Contraction();
        MaxH->Contraction();
        float temp1 = MinH->Output();
        float temp2 = MaxH->Output();
        if (temp1 > temp2) {
            cout << "Final Min element is bigger than final Max element." << endl;
        }
        else if (temp1 < temp2) {
            cout << "Final Max element is bigger than final Min element." << endl;
        }
        else {
            cout << "Final Min element and Final Max element are equal to each other." << endl;
        }
        delete[] heap_min; // We need not delete heap_max, since it points to heap_min (throws message of double free), thus the memory it points to is already free
        return 0;
    }
    catch (const char* msg) {
        cerr << msg << endl;
        return 1;
    }
}