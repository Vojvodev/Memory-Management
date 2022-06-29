#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <list>
#include <iterator>


#define MAX 30


using namespace std;


int fifo(int[], int, int);
int lru(int[], int, int);
int second_chance(int[], int, int);
int lfu(int[], int, int);
int pageFaults(int[], int, int);
int optimal_algorithm(int[], int, int);

void printQueue(int, queue<int>, string);
void printList(int, list<int>, string);
void printVector(int, vector<int>, string);
int predict(int[], vector<int>&, int, int);
bool search(int, vector<int>&);




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
            optimal_algorithm(reference_array, reference_number, frame_number);
            break;
        default:

            abort();
        }
    } while (1);

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
    cout << "Efficiency: " << page_faults << " / " << reference_number << " = " << 100 * page_faults / (double)reference_number << "%" << "\n\n\n\n";

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
    cout << "Efficiency: " << page_faults << " / " << reference_number << " = " << 100 * page_faults / (double)reference_number << "%" << "\n\n\n\n";

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
    cout << "Efficiency: " << page_faults << " / " << reference_number << " = " << 100 * page_faults / (double)reference_number << "%" << "\n\n\n\n";

    return page_faults;

}


// Aging mechanism. When not used, a page's frequency gets decreased by one, else it gets increased by one
int lfu(int pages[], int reference_number, int frame_number) {

    vector<int> v;
    unordered_map<int, int> mp;     // To store frequency of pages

    int page_faults = 0;
    

    for (int i = 0; i < reference_number; i++) {

        auto it = find(v.begin(), v.end(), pages[i]);

        
        if (it == v.end()) {                     // If element is not present
            if (v.size() == frame_number) {     // If memory is full
                mp[v[0]]--;                    // Decrease the frequency
                    
                v.erase(v.begin());           // Remove the first element as it is least frequently used
                
            }

            v.push_back(pages[i]);             // Add the element at the end of memory
                
            mp[pages[i]]++;                   // Increase its frequency
            

            cout << pages[i] << " - PF  - |";       // First part of printing out

            page_faults++;
        }
        else {                              // If element is present, remove the element, add it at the end and increase its frequency
            mp[pages[i]]++;
            v.erase(it);
            v.push_back(pages[i]);
            
            cout << pages[i] << " - NPF - |";       // First part of printing out

        }

        // Compare frequency with other pages
        // starting from the 2nd last page                 
        int k = v.size() - 2;

        // Sort the pages based on their frequency 
        // And time at which they arrive
        // if frequency is same
        // then, the page arriving first must be placed first
        while (k > -1 && mp[v[k]] > mp[v[k + 1]]) {
            swap(v[k + 1], v[k]);
            k--;
        }


        for (int i = 0; i < v.size(); i++)          // Second part of printing out 
            cout << v[i] << "|";

        cout << " <- " << endl;
    
    }


    cout << "\nLFU\nNumber of page faults: " << page_faults << "\n";
    cout << "Efficiency: " << page_faults << " / " << reference_number << " = " << 100 * page_faults / (double)reference_number << "%" << "\n\n\n\n";

    
    return page_faults;
}


// Throws out page that will appear last, in the future
int optimal_algorithm(int pages[], int reference_number, int frame_number) {

    vector<int> fr;
    int page_faults = 0;
    int hits = 0;

    for (int i = 0; i < reference_number; i++) {
        if (search(pages[i], fr)) {
            hits++;

            printVector(pages[i], fr, " - NPF - |");
            continue;
        }

        if (fr.size() < frame_number)
        {
            fr.push_back(pages[i]);
        }
        else {
            int j = predict(pages, fr, reference_number, i + 1);        // Find the page to be replaced.
            fr[j] = pages[i];
        }



        printVector(pages[i], fr, " - PF  - |");
    }

    page_faults = reference_number - hits;

    cout << "\nOptimal algorithm\nNumber of page faults: " << page_faults << "\n";
    cout << "Efficiency: " << page_faults << " / " << reference_number << " = " << 100 * page_faults / (double)reference_number << "%" << "\n\n\n\n";

    return page_faults;

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


// Helping function for optimal algorithm
void printVector(int page, vector<int> fr, string PForNPF)
{
    cout << page << PForNPF;

    for (int i = 0; i < fr.size(); i++)
        cout << fr[i] << "|";

    cout << " <- " << endl;
}


// Helping function for optiomal algorithm, determines which page to kick out
int predict(int page[], vector<int>& fr, int reference_number, int index) {

    int res = -1, farthest = index;
    for (int i = 0; i < fr.size(); i++) {
        int j;
        for (j = index; j < reference_number; j++) {
            if (fr[i] == page[j]) {
                if (j > farthest) {
                    farthest = j;
                    res = i;
                }
                break;
            }
        }
        if (j == reference_number)
            return i;
    }

    return (res == -1) ? 0 : res;
}


// Helping function for optiomal algorithm
bool search(int key, vector<int>& fr) {
    for (int i = 0; i < fr.size(); i++)
        if (fr[i] == key)
            return true;
    return false;
}

