#include <random>
#include <functional>
#include <iostream>

using namespace std;

template <typename T>
class Chain;
template <typename T>
class Histogram;

template <typename T>
class ChainNode {
    friend Chain<T>;
    friend Histogram<T>;
private:
    T dataL;
    ChainNode<T> *linkL;
};

template <typename T>
class HistogramNode {
    friend Histogram<T>;
private:
    T dataH;
    T countH;
    HistogramNode<T> *linkH;
};

template <typename T>
class Histogram {
public:
    Histogram() { // Histogram constructor
        firstH = nullptr;
    }
    ~Histogram() { // Histogram destructor
        HistogramNode<T> *next;
        while (firstH) {
            next = firstH->linkH;
            delete firstH;
            firstH = next;
        }
    }
    Histogram<T>& Insert(const Chain<T> *Chain) { 	// Insert element of Chain L if it does not already exist in Histogram, else increase the right counter
        ChainNode<T> *currentL = Chain->firstL;
        HistogramNode<T> *currentH = firstH;
        while (currentL) { // For each element of Chain L, check if it exists inside Histogram
            int i = 0;
            currentH = firstH;
            if (!currentH) {
                auto *y = new HistogramNode<T>;
                y->dataH = currentL->dataL;
                y->countH = 1;
                firstH = y;
                currentH = firstH;
            }
            else {
                while (currentH) {
                    if (currentL->dataL == currentH->dataH) { // If element exists in Histogram, increase its counter
                        currentH->countH++;
                    }
                    else { // If element does not exist, increase i
                        i++;
                    }
                    currentH = currentH->linkH;
                }
                int size = this->Length();
                if (i == size) { // Check if element existed in Histogram and if it didn't, do the following
                    auto *y = new HistogramNode<T>;
                    y->dataH = currentL->dataL;
                    y->countH = 1; // Element appears for the 1st time in Histogram
                    currentH = firstH;
                    int c = 1;
                    while (c <= size) {
                        if (c == size) { // Add new element right after the last element to already exist
                            currentH->linkH = y;
                        }
                        currentH = currentH->linkH; // Follow the link to the next element on Histogram
                        c++;
                    }
                }
            }
            currentL = currentL->linkL; // After we've finished with one element oh Chain L, current follows the link to next element until none are left
        }
        return *this;
    }
    int Length() const { // Return the number of elements in Histogram
        HistogramNode<T> *current = firstH;
        int len = 0;
        while (current) {
            len++;
            current = current->linkH;
        }
        return len;
    }
    Histogram<T>& bubbleSort() { // Function to Sort the linked list using Bubble Sort
        HistogramNode<T> *temp1;
        HistogramNode<T> *temp2;
        for(temp1 = firstH; temp1; temp1=temp1->linkH)
        {
            for(temp2 = temp1->linkH; temp2; temp2=temp2->linkH)
            {
                if(temp1->dataH > temp2->dataH)
                {
                    T temp3 = temp1->dataH;
                    T temp4 = temp1->countH;
                    temp1->dataH = temp2->dataH;
                    temp1->countH = temp2->countH;
                    temp2->dataH = temp3;
                    temp2->countH = temp4;
                }
            }
        }
        return *this;
    }
    void Output() const { // Function to print elements in Histogram, along with their counters
        HistogramNode<T> *current = firstH;
        int i = 1;
        while (current) {
            cout << "Element number " << i << " in Histogram is: " << current->dataH << " and appears a total of: " << current->countH << " time(s)." << endl;
            current = current->linkH;
            i++;
        }
    }
private:
    HistogramNode<T> *firstH;
    ChainNode<T> *firstL;
};

template <typename T>
class Chain {
    friend Histogram<T>; // Class Histogram is declared as a friend to class Chain, so we can access its first element to be able to create Histogram
public:
    Chain() { // Chain constructor
        firstL = nullptr;
    }
    ~Chain() { // Chain destructor
        ChainNode<T> *next;
        while (firstL) {
            next = firstL->linkL;
            delete firstL;
            firstL = next;
        }
    }
    Chain<T>& Insert(int k, const T& x) { // Insert x after the k'th element
        if (k < 0) { // Throw OutOfBounds exception if position to insert new element is negative or zero
            throw "Out of bounds.";
        }
        ChainNode<T> *current = firstL;
        for (int i = 1; i < k && current; i++) { // Lead current to k'th element
            current = current->linkL;
        }
        if (k > 0 && !current) { // Throw OutOfBounds exception if chain L has less than k elements
            throw "Out of bounds.";
        }
        auto *y = new ChainNode<T>; // If position k is valid, create new element
        y->dataL = x;
        // Insert new element after k'th element, represented by current
        if (k) { // Insert as interstitial element
            y->linkL = current->linkL;
            current->linkL = y;
        }
        else { // Insert as first element
            y->linkL = firstL;
            firstL = y;
        }
        return *this;
    }
    void Output() const { // Function to print elements in Chain L
        int i = 1;
        ChainNode<T> *current = firstL;
        while (current) {
            cout << "Element number " << i << " in Chain L is: " << current->dataL << endl;
            current = current->linkL;
            i++;
        }
    }
private:
    ChainNode<T> *firstL;
};

int main() {
    try {
        default_random_engine generator;
        uniform_int_distribution<int> data_element_distribution(0, 100); // Change the limits of randomness to see difference in how many times each element appears
        auto random_element= bind(data_element_distribution, generator);
        int n;
        do { // Check for valid number of elements
            cout << "Give the size of Chain L." << endl;
            cin >> n;
        } while (n<=0);
        auto *L = new Chain<int>;
        for (int i = 0; i < n; i++) {
            int data_element = random_element();
            L->Insert(i, data_element);
        }
        auto *Hist = new Histogram<int>;
        Hist->Insert(L);
        Hist->Output(); // Histogram before being sorted
        Hist->bubbleSort();
        L->Output();
        Hist->Output(); // Histogram after being sorted
        delete L;
        delete Hist;
        return 0;
    }
    catch (const char* msg) {
        cerr << msg << endl;
        return 1;
    }
}