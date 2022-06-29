#include <iostream>
#include <unordered_set>
#include <queue>
#include <list>
#include <iterator>


#define MAX 30


using namespace std;


int fifo(int[], int, int);
int lru(int[], int, int);
int second_chance(int[], int, int);
int lfu(int[], int, int);
int optimal_algorithm(int[], int, int);

void printQueue(int, queue<int>, string);
void printList(int, list<int>, string);




int main() {

    // Reference = Page

    int frame_number = 0;
    int reference_number = 0;
    int reference_array[MAX]{};
    int algorithm_number = 0;       // Multiple choice variable



    do {
        cout << "Enter number of frames: ";
        cin >> frame_number;
    } while (frame_number < 1);

    do {
        cout << "Enter number of references: ";
        cin >> reference_number;
    } while (reference_number < 1);



    for (int i = 0; i < reference_number; i++) {
        cout << "Enter reference[" << i + 1 << "]: ";
        cin >> reference_array[i];
    }
    cout << std::endl;
    cout << std::endl;
    
    
    
    do {
        cout << "Pick an algorithm \n"
            "1) FIFO \n"
            "2) LRU \n"
            "3) Second Chance \n"
            "4) LFU \n"
            "5) Optimal algorithm \n\n"
            "Enter number: ";
        cin >> algorithm_number;
    } while ((algorithm_number < 1) || (algorithm_number > 5));





    switch (algorithm_number) {
    case 1:
        fifo(reference_array, reference_number, frame_number);
        break;
    case 2:
        lru(reference_array, reference_number, frame_number);
        break;
    case 3:
        second_chance(reference_array, reference_number, frame_number);
        break;
    case 4:
        lfu(reference_array, reference_number, frame_number);
        break;
    case 5:
        //optimal_algorithm(reference_array, reference_number, frame_number);
        break;
    default:

        abort();
    }


    return 0;
}




// First In First Out
int fifo(int pages[], int reference_number, int frame_number) {

    unordered_set<int> s;
    queue<int> indexes;

    int page_faults = 0;


    for (int i = 0; i < reference_number; i++) {

        if (s.size() < frame_number) {      // There is room for another frame (nothing needs to be pushed out)
            if (s.find(pages[i]) == s.end()) {      // If the page is not found in set (insert into set and increase page fault counter)
                
                s.insert(pages[i]);     // Insert the current page into the set
                page_faults++;          // Page Fault occurred

                indexes.push(pages[i]);

                printQueue(pages[i], indexes, " - PF  - |");
            }
            else {      // Page already exists in queue, nothing happens (pages just get written out) 
                printQueue(pages[i], indexes, " - NPF - |");
            }

        }
        else {      // There is no room for another frame (first in queue needs to go)

            if (s.find(pages[i]) == s.end()) {      // If the page is not found in set (insert into set and increase page fault counter)

                int val = indexes.front();          // One frame is removed
                indexes.pop();
                s.erase(val);

                s.insert(pages[i]);                 // New one is added
                indexes.push(pages[i]);

                page_faults++;                      // Page Fault occurred

                printQueue(pages[i], indexes, " - PF  - |");

            }
            else {      // Page already exists in queue, nothing happens (pages just get written out)
                printQueue(pages[i], indexes, " - NPF - |");
            }

        }


    }

    cout << "\nFIFO\nNumber of page faults: " << page_faults << "\n";
    cout << "Efficiency: " << page_faults << " / " << reference_number << " = " << 100 * page_faults / (double)reference_number << "%" << "\n\n";

    return page_faults;
}


// Least Recently Used meaning oldest one goes out
// Same as fifo, except that, when there is no PF, the referenced page goes to the back of the queue
int lru(int pages[], int reference_number, int frame_number)
{

    unordered_set<int> s;
    list<int> indexes;

    int page_faults = 0;


    for (int i = 0; i < reference_number; i++) {

        if (s.size() < frame_number) {      // There is room for another frame (nothing needs to be pushed out)
            if (s.find(pages[i]) == s.end()) {      // If the page is not found in set (insert into set and increase page fault counter)

                s.insert(pages[i]);     // Insert the current page into the set
                page_faults++;          // Page Fault occurred

                indexes.push_back(pages[i]);

                printList(pages[i], indexes, " - PF  - |");
            }
            else {      // Page already exists in queue, new one goes to the top
                
                list<int>::iterator iter = indexes.begin();       // Now the iterator points to the page I want to remove and then add again to the top
                while (*iter != pages[i]) { iter++; }

                indexes.erase(iter);
                indexes.push_back(pages[i]);

                printList(pages[i], indexes, " - NPF - |");
            }

        }
        else {      // There is no room for another frame (first in queue needs to go)

            if (s.find(pages[i]) == s.end()) {      // If the page is not found in set (insert into set and increase page fault counter)

                int val = indexes.front();          // One frame is removed
                indexes.pop_front();
                s.erase(val);

                s.insert(pages[i]);                 // New one is added
                indexes.push_back(pages[i]);

                page_faults++;                      // Page Fault occurred

                printList(pages[i], indexes, " - PF  - |");

            }
            else {      // Page already exists in queue, new one goes to the top
                
                list<int>::iterator iter = indexes.begin();
                while (*iter != pages[i]) { iter++; }

                indexes.erase(iter);
                indexes.push_back(pages[i]);
                
                printList(pages[i], indexes, " - NPF - |");
            }

        }


    }

    cout << "\nLRU\nNumber of page faults: " << page_faults << "\n";
    cout << "Efficiency: " << page_faults << " / " << reference_number << " = " << 100 * page_faults / (double)reference_number << "%" << "\n\n";

    return page_faults;
}


// One of the references has R - bit (priviledge)
int second_chance(int pages[], int reference_number, int frame_number) {
    
    int Rpage;
    bool priviledge = true;
    
    cout << "Which page has the R-bit: ";
    cin >> Rpage;

    unordered_set<int> s;
    queue<int> indexes;

    int page_faults = 0;


    for (int i = 0; i < reference_number; i++) {

        if (s.size() < frame_number) {      // There is room for another frame (nothing needs to be pushed out)
            if (s.find(pages[i]) == s.end()) {      // If the page is not found in set (insert into set and increase page fault counter)

                s.insert(pages[i]);     // Insert the current page into the set
                page_faults++;          // Page Fault occurred

                indexes.push(pages[i]);

                printQueue(pages[i], indexes, " - PF  - |");
            }
            else {      // Page already exists in queue, nothing happens (pages just get written out) 
                printQueue(pages[i], indexes, " - NPF - |");
            }

        }
        else {      // There is no room for another frame (first in queue needs to go, except if its the one with priviledge)

            if (s.find(pages[i]) == s.end()) {      // If the page is not found in set (insert into set and increase page fault counter)
                
                if (pages[i] == Rpage) { priviledge = true; }

                int val = indexes.front();          
                if (val == Rpage)                   // If its the Rpage that should've been kicked out
                {
                    if (priviledge == false)        // Priviledge == false, it just gets deleted
                    {
                        indexes.pop();
                        s.erase(val);
                        s.insert(pages[i]);                 
                        indexes.push(pages[i]);
                    }
                    else                            // Priviledge is there, first the Rpage goes to the top, then we insert the new page
                    {
                        priviledge = false;

                        indexes.pop();
                        s.erase(val);
                        s.insert(Rpage);                 
                        indexes.push(Rpage);

                        val = indexes.front();      
                        indexes.pop();
                        s.erase(val);
                        s.insert(pages[i]);
                        indexes.push(pages[i]);
                    }
                }
                else
                {
                    indexes.pop();
                    s.erase(val);
                    s.insert(pages[i]);
                    indexes.push(pages[i]);
                }

                page_faults++;                      // Page Fault occurred

                printQueue(pages[i], indexes, " - PF  - |");

            }
            else {      // Page already exists in queue, nothing happens (pages just get written out)
                
                if (pages[i] == Rpage && priviledge == false) { priviledge = true; }
                
                printQueue(pages[i], indexes, " - NPF - |");
            }

        }


    }

    cout << "\nSecond Chance\nNumber of page faults: " << page_faults << "\n";
    cout << "Efficiency: " << page_faults << " / " << reference_number << " = " << 100 * page_faults / (double)reference_number << "%" << "\n\n";

    return page_faults;

}


void lfu() {
    std::cout << "\nnot working yet :(";
    abort();
}


void optimal_algorithm() {
    std::cout << "\nnot working yet :(";
    abort();
}


// Helping function for fifo and second chance that prints out pages
void printQueue(int page, queue<int> indexes, string PForNPF)
{
    cout << page << PForNPF;

    queue<int> printing_queue(indexes);

    while (!printing_queue.empty()) {
        cout << printing_queue.front() << "|";
        printing_queue.pop();
    }

    cout << " <- " << endl;
}


// Helping function for LRU
void printList(int page, list<int> indexes, string PForNPF)
{
    cout << page << PForNPF;

    list<int>::iterator it;
    for (it = indexes.begin(); it != indexes.end(); ++it)
        cout << *it << "|";

    cout << " <- " << endl;
}

