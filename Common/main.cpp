#include <iostream>
#include "String.h"
#include "List.h"
#include "Vector.h"
#include "Queue.h"
#include "HashMap.h"
#include "PriorityQueue.h"
using namespace std;

int main() {
    struct Test{
        int value;
        int priority;
        Test(int value = 0, int priority = 0) : value(value), priority(priority) {}
        bool operator>(Test &other) const{
            return priority > other.priority;
        }
    };
    PriorityQueue<Test> queue(false);
    queue.push(Test(3, 3));
    queue.push(Test(1, 1));
    queue.push(Test(2, 2));
    cout << queue.pop().value << endl;
}
