/*
Program: EECS 348 Assignment 3
Description: This program prioritizes CEO emails using an object-oriented MaxHeap priority queue.
Inputs: The program reads commands from standard input: EMAIL, NEXT, READ, and COUNT.
Output: The program displays the next email for the CEO to read and the number of unread emails.
Collaborators: None.
Sources: ChatGPT, Gemini
Author: Shimrah
Creation Date: October 1, 2026
Revision Date: October 1, 2026
Revisions: Created an object-oriented MaxHeap solution and improved input handling, priority comparison, and comments.
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;

/*
The Email class stores one email's sender category, subject, date, and arrival order.
It also contains the comparison logic needed by the MaxHeap.
*/
class Email {
private:
    string senderCategory;
    string subjectLine;
    string date;
    int arrivalOrder;

public:
    Email() {
        senderCategory = "";
        subjectLine = "";
        date = "";
        arrivalOrder = 0;
    }

    Email(string sender, string subject, string emailDate, int order) {
        senderCategory = sender;
        subjectLine = subject;
        date = emailDate;
        arrivalOrder = order;
    }

    string getSenderCategory() const {
        return senderCategory;
    }

    string getSubjectLine() const {
        return subjectLine;
    }

    string getDate() const {
        return date;
    }

    int getSenderPriority() const {
        if (senderCategory == "Boss") {
            return 5;
        }
        else if (senderCategory == "Subordinate") {
            return 4;
        }
        else if (senderCategory == "Peer") {
            return 3;
        }
        else if (senderCategory == "ImportantPerson") {
            return 2;
        }
        else {
            return 1;
        }
    }

    int getDateValue() const {
        int month = stoi(date.substr(0, 2));
        int day = stoi(date.substr(3, 2));
        int year = stoi(date.substr(6, 4));
        return (year * 10000) + (month * 100) + day;
    }

    bool hasHigherPriorityThan(const Email& other) const {
        if (getSenderPriority() != other.getSenderPriority()) {
            return getSenderPriority() > other.getSenderPriority();
        }

        if (getDateValue() != other.getDateValue()) {
            return getDateValue() > other.getDateValue();
        }

        return arrivalOrder < other.arrivalOrder;
    }

    void printEmail() const {
        cout << "Next email:" << endl;
        cout << "Sender: " << senderCategory << endl;
        cout << "Subject: " << subjectLine << endl;
        cout << "Date: " << date << endl;
    }
};

/*
The MaxHeap class manually implements a priority queue.
It uses a vector as the list-based heap storage.
No built-in heap module or priority_queue is used.
*/
class MaxHeap {
private:
    vector<Email> heapList;

    int getParentIndex(int index) const {
        return (index - 1) / 2;
    }

    int getLeftChildIndex(int index) const {
        return (2 * index) + 1;
    }

    int getRightChildIndex(int index) const {
        return (2 * index) + 2;
    }

    void heapifyUp(int index) {
        while (index > 0 && heapList[index].hasHigherPriorityThan(heapList[getParentIndex(index)])) {
            swap(heapList[index], heapList[getParentIndex(index)]);
            index = getParentIndex(index);
        }
    }

    void heapifyDown(int index) {
        int largestIndex = index;
        int leftIndex = getLeftChildIndex(index);
        int rightIndex = getRightChildIndex(index);

        if (leftIndex < heapList.size() && heapList[leftIndex].hasHigherPriorityThan(heapList[largestIndex])) {
            largestIndex = leftIndex;
        }

        if (rightIndex < heapList.size() && heapList[rightIndex].hasHigherPriorityThan(heapList[largestIndex])) {
            largestIndex = rightIndex;
        }

        if (largestIndex != index) {
            swap(heapList[index], heapList[largestIndex]);
            heapifyDown(largestIndex);
        }
    }

public:
    void insertEmail(const Email& email) {
        heapList.push_back(email);
        heapifyUp(heapList.size() - 1);
    }

    bool isEmpty() const {
        return heapList.empty();
    }

    int getCount() const {
        return heapList.size();
    }

    Email peekMaxEmail() const {
        return heapList[0];
    }

    void removeMaxEmail() {
        if (heapList.empty()) {
            return;
        }

        heapList[0] = heapList[heapList.size() - 1];
        heapList.pop_back();

        if (!heapList.empty()) {
            heapifyDown(0);
        }
    }
};

/*
The EmailProgram class controls the whole program.
It reads commands, creates Email objects, and uses the MaxHeap to prioritize them.
*/
class EmailProgram {
private:
    MaxHeap inbox;
    int arrivalCounter;

    string trim(string text) {
        int start = 0;
        int end = text.length() - 1;

        while (start < text.length() && isspace(text[start])) {
            start++;
        }

        while (end >= start && isspace(text[end])) {
            end--;
        }

        return text.substr(start, end - start + 1);
    }

    void processEmailCommand(string line) {
        string emailData = line.substr(6);

        int firstComma = emailData.find(",");
        int secondComma = emailData.find(",", firstComma + 1);

        string sender = trim(emailData.substr(0, firstComma));
        string subject = trim(emailData.substr(firstComma + 1, secondComma - firstComma - 1));
        string date = trim(emailData.substr(secondComma + 1));

        Email newEmail(sender, subject, date, arrivalCounter);
        arrivalCounter++;

        inbox.insertEmail(newEmail);
    }

    void processNextCommand() {
        if (inbox.isEmpty()) {
            cout << "There are 0 emails to read." << endl;
        }
        else {
            inbox.peekMaxEmail().printEmail();
        }
    }

    void processReadCommand() {
        if (!inbox.isEmpty()) {
            inbox.removeMaxEmail();
        }
    }

    void processCountCommand() {
        cout << "There are " << inbox.getCount() << " emails to read." << endl;
    }

public:
    EmailProgram() {
        arrivalCounter = 0;
    }

    void run() {
        string line;

        while (getline(cin, line)) {
            if (line.substr(0, 5) == "EMAIL") {
                processEmailCommand(line);
            }
            else if (line == "NEXT") {
                processNextCommand();
            }
            else if (line == "READ") {
                processReadCommand();
            }
            else if (line == "COUNT") {
                processCountCommand();
            }
        }
    }
};

int main() {
    EmailProgram program;
    program.run();
    return 0;
}
