/*
Name: EECS 348 Assignment 3
Description: C program that takes in instructions, and performs things typical of an email service
Inputs: Text file for instructions
Outputs: Results of instructions from text file
Collaborators: None
Sources: ChatGPT
Author: Max Kuhlmann
Creation Date: 10/1/26
Revision Date: 10/1/26
Revisions: Fixed compiler warnings
*/

// Following code is from ChatGPT, with edits and comments added

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

// ------------------------------------------------------------
// Email Object
// ------------------------------------------------------------
class Email
{
// Defines private variables
private:
    string sender;
    string subject;
    string date;
    int priority;
    int dateValue;

// Defines public functions
public:
    // Initial values for new Email object without starting values
    Email()
    {
        sender = "";
        subject = "";
        date = "";
        priority = 0;
        dateValue = 0;
    }

    // Initial values for new Email object with starting values
    Email(string s, string sub, string d)
    {
        sender = s;
        subject = sub;
        date = d;

        // Assign priority based on sender category.
        if (sender == "Boss")
            priority = 5;
        else if (sender == "Subordinate")
            priority = 4;
        else if (sender == "Peer")
            priority = 3;
        else if (sender == "ImportantPerson")
            priority = 2;
        else
            priority = 1;

        // Convert MM-DD-YYYY into a number that can be compared.
        // YYYY is most significant, then MM, then DD.
        int month = stoi(date.substr(0, 2));
        int day = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));

        dateValue = year * 10000 + month * 100 + day;
    }

    // Get functions to return values of emails
    string getSender()
    {
        return sender;
    }

    string getSubject()
    {
        return subject;
    }

    string getDate()
    {
        return date;
    }

    int getPriority()
    {
        return priority;
    }

    int getDateValue()
    {
        return dateValue;
    }
};


// ------------------------------------------------------------
// MaxHeap Object
// List-based implementation
// ------------------------------------------------------------
class MaxHeap
{
// Defines private functions
private:
    vector<Email> heap;

    // Determines whether email a has higher priority than email b.
    bool higherPriority(Email a, Email b)
    {
        // Sender category has primary importance.
        if (a.getPriority() != b.getPriority())
            return a.getPriority() > b.getPriority();

        // If sender categories are the same,
        // the newest email comes first.
        return a.getDateValue() > b.getDateValue();
    }

    // Returns parents/children
    int parent(int index)
    {
        return (index - 1) / 2;
    }

    int leftChild(int index)
    {
        return 2 * index + 1;
    }

    int rightChild(int index)
    {
        return 2 * index + 2;
    }

    // Swaps two emails by index
    void swapEmails(int first, int second)
    {
        Email temp = heap[first];
        heap[first] = heap[second];
        heap[second] = temp;
    }

    // Move an email upward after insertion.
    void heapifyUp(int index)
    {
        while (index > 0)
        {
            int p = parent(index);

            if (higherPriority(heap[index], heap[p]))
            {
                swapEmails(index, p);
                index = p;
            }
            else
            {
                break;
            }
        }
    }

    // Move an email downward after removal.
    void heapifyDown(int index)
    {
        while (true)
        {
            size_t left = leftChild(index);
            size_t right = rightChild(index);
            int largest = index;

            if (left < heap.size() &&
                higherPriority(heap[left], heap[largest]))
            {
                largest = left;
            }

            if (right < heap.size() &&
                higherPriority(heap[right], heap[largest]))
            {
                largest = right;
            }

            if (largest != index)
            {
                swapEmails(index, largest);
                index = largest;
            }
            else
            {
                break;
            }
        }
    }

// Defines public functions
public:
    // Add an email to the MaxHeap.
    void insert(Email email)
    {
        heap.push_back(email);
        heapifyUp(heap.size() - 1);
    }

    // Return the highest-priority email without removing it.
    Email peek()
    {
        return heap[0];
    }

    // Remove and return the highest-priority email.
    Email removeMax()
    {
        Email result = heap[0];

        heap[0] = heap[heap.size() - 1];
        heap.pop_back();

        if (!heap.empty())
            heapifyDown(0);

        return result;
    }

    // Returns information about heap
    int size()
    {
        return heap.size();
    }

    bool empty()
    {
        return heap.empty();
    }
};


// ------------------------------------------------------------
// CEO Email System Object
// ------------------------------------------------------------
class CEOEmailSystem
{
// Defines private variables
private:
    MaxHeap emailQueue;

// Defines public functions
public:
    // Adds an email to the CEO's priority queue.
    void addEmail(string sender, string subject, string date)
    {
        Email email(sender, subject, date);
        emailQueue.insert(email);
    }

    // Displays the next email without marking it as read.
    void nextEmail()
    {
        if (emailQueue.empty())
            return;

        Email email = emailQueue.peek();

        cout << "Next email:" << endl;
        cout << "Sender: " << email.getSender() << endl;
        cout << "Subject: " << email.getSubject() << endl;
        cout << "Date: " << email.getDate() << endl;
        cout << endl;
    }

    // Marks the current highest-priority email as read.
    void readEmail()
    {
        if (!emailQueue.empty())
            emailQueue.removeMax();
    }

    // Displays the number of unread emails.
    void countEmails()
    {
        cout << "There are " << emailQueue.size()
             << " emails to read." << endl;
        cout << endl;
    }

    // Processes commands from the test file.
    void processCommand(string line)
    {
        if (line.substr(0, 5) == "EMAIL")
        {
            // Remove "EMAIL "
            string information = line.substr(6);

            // Find the first comma.
            size_t firstComma = information.find(',');

            // Find the second comma.
            size_t secondComma = information.find(',', firstComma + 1);

            string sender =
                information.substr(0, firstComma);

            string subject =
                information.substr(
                    firstComma + 1,
                    secondComma - firstComma - 1);

            string date =
                information.substr(secondComma + 1);

            addEmail(sender, subject, date);
        }
        else if (line == "NEXT")
        {
            nextEmail();
        }
        else if (line == "READ")
        {
            readEmail();
        }
        else if (line == "COUNT")
        {
            countEmails();
        }
    }
};


// ------------------------------------------------------------
// Main
// ------------------------------------------------------------
int main()
{
    // Initializes the CEO Email System
    CEOEmailSystem system;

    // Used for user input
    string line;

    // Constantly reads from terminal for input
    // ./chatgpt < test.txt can be used to input values faster
    while (getline(cin, line))
    {
        if (!line.empty())
        {
            system.processCommand(line);
        }
    }

    return 0;
}

