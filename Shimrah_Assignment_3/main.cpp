/*
 * Program: EECS 348 Assignment 3 - CEO Email Priority Queue
 * Author: Shimrah
 * Creation Date: 10/01/2026
 * Revision Date: 10/01/2026
 *
 * Description:
 * This C++ program manages a CEO's unread emails by using a MaxHeap as a
 * priority queue. Emails are ordered first by sender category, then by newest
 * date, then by arrival order if the category and date are the same.
 *
 * Inputs:
 * The program reads commands from standard input until the end of the file.
 * Valid commands are:
 * EMAIL <sender category>,<subject line>,<date>
 * NEXT
 * READ
 * COUNT
 *
 * Outputs:
 * The program displays the next email, removes the next email, or displays the
 * number of unread emails depending on the command.
 *
 * Collaborators: None
 *
 * Other Sources:
 * OpenAI ChatGPT/Codex was used to help draft and explain this program.
 * Google Gemini
 *
 */

#include <iostream>  // Allows the program to read input and print output.
#include <string>    // Allows the program to use string objects.
#include <vector>    // Allows the heap to be stored in a dynamic list.

using namespace std; // Allows standard library names to be used directly.

/*
 * StringHelper contains small text-cleaning behavior used by the other classes.
 * This keeps the program object-oriented instead of relying on loose helper code.
 */
class StringHelper {
public:
    /*
     * trim removes spaces, tabs, and line endings from the front and back of text.
     * This helps the program handle input files with small formatting differences.
     */
    static string trim(const string& text) {
        size_t start = text.find_first_not_of(" \t\r\n"); // Finds the first real character.

        if (start == string::npos) { // Checks whether the string only contains whitespace.
            return "";              // Returns an empty string when there is no real text.
        }

        size_t end = text.find_last_not_of(" \t\r\n"); // Finds the last real character.
        return text.substr(start, end - start + 1);       // Returns the cleaned text.
    }
};

/*
 * Date stores the original date text and a sortable integer version of the date.
 * The sortable value makes it easy to decide which email is newer.
 */
class Date {
private:
    string originalText; // Stores the date exactly as it should be displayed.
    int month;           // Stores the month part of the date.
    int day;             // Stores the day part of the date.
    int year;            // Stores the year part of the date.
    int sortableValue;   // Stores the date as YYYYMMDD for easy comparison.

public:
    /*
     * Default constructor creates an empty date object.
     */
    Date() {
        originalText = ""; // Starts with no display text.
        month = 0;         // Starts month at 0.
        day = 0;           // Starts day at 0.
        year = 0;          // Starts year at 0.
        sortableValue = 0; // Starts sortable value at 0.
    }

    /*
     * Constructor accepts a date in MM-DD-YYYY format.
     */
    Date(const string& dateText) {
        originalText = StringHelper::trim(dateText);          // Saves the cleaned original date.
        month = stoi(originalText.substr(0, 2));              // Reads the MM part.
        day = stoi(originalText.substr(3, 2));                // Reads the DD part.
        year = stoi(originalText.substr(6, 4));               // Reads the YYYY part.
        sortableValue = (year * 10000) + (month * 100) + day; // Converts to YYYYMMDD.
    }

    /*
     * getSortableValue returns the integer used for date comparisons.
     */
    int getSortableValue() const {
        return sortableValue; // Returns YYYYMMDD.
    }

    /*
     * toString returns the original date text for output.
     */
    string toString() const {
        return originalText; // Returns the display version of the date.
    }
};

/*
 * Email stores all information about one email and knows how to compare itself
 * against another email using the assignment's priority rules.
 */
class Email {
private:
    string senderCategory; // Stores the sender category.
    string subjectLine;    // Stores the email subject line.
    Date emailDate;        // Stores the email date.
    int arrivalNumber;     // Stores when the email arrived compared to others.
    int categoryPriority;  // Stores the numeric priority of the sender category.

    /*
     * calculateCategoryPriority converts the required category order into numbers.
     * Higher numbers mean higher priority in the MaxHeap.
     */
    int calculateCategoryPriority(const string& category) const {
        if (category == "Boss") {            // Boss is highest priority.
            return 5;                         // Returns the highest category value.
        } else if (category == "Subordinate") { // Subordinate is second.
            return 4;                            // Returns the second-highest category value.
        } else if (category == "Peer") {      // Peer is third.
            return 3;                          // Returns the middle category value.
        } else if (category == "ImportantPerson") { // ImportantPerson is fourth.
            return 2;                               // Returns the fourth category value.
        } else {                              // OtherPerson is last.
            return 1;                         // Returns the lowest category value.
        }
    }

public:
    /*
     * Default constructor creates an empty email object.
     */
    Email() {
        senderCategory = ""; // Starts sender category as empty.
        subjectLine = "";    // Starts subject line as empty.
        emailDate = Date();   // Starts with the default date.
        arrivalNumber = 0;    // Starts arrival number at 0.
        categoryPriority = 0; // Starts category priority at 0.
    }

    /*
     * Constructor creates a complete email object from parsed input fields.
     */
    Email(const string& sender, const string& subject, const string& dateText, int arrival) {
        senderCategory = StringHelper::trim(sender);              // Stores cleaned sender category.
        subjectLine = StringHelper::trim(subject);                // Stores cleaned subject line.
        emailDate = Date(StringHelper::trim(dateText));           // Stores parsed email date.
        arrivalNumber = arrival;                                  // Stores the arrival order.
        categoryPriority = calculateCategoryPriority(senderCategory); // Stores sender priority.
    }

    /*
     * hasHigherPriorityThan applies the MaxHeap comparison rules.
     */
    bool hasHigherPriorityThan(const Email& other) const {
        if (categoryPriority != other.categoryPriority) {           // First compares sender category.
            return categoryPriority > other.categoryPriority;       // Higher category number wins.
        }

        if (emailDate.getSortableValue() != other.emailDate.getSortableValue()) { // Then compares dates.
            return emailDate.getSortableValue() > other.emailDate.getSortableValue(); // Newer date wins.
        }

        return arrivalNumber < other.arrivalNumber; // Earlier arrival wins if category and date tie.
    }

    /*
     * display prints one email in the exact format required for NEXT.
     */
    void display() const {
        cout << "Sender: " << senderCategory << endl; // Prints sender category.
        cout << "Subject: " << subjectLine << endl;   // Prints subject line.
        cout << "Date: " << emailDate.toString() << endl; // Prints date.
    }
};

/*
 * MaxHeap stores Email objects in a vector-based list and maintains heap order.
 * It does not use std::priority_queue or any pre-existing heap module.
 */
class MaxHeap {
private:
    vector<Email> heapList; // Stores the heap as a list where index 0 is the root.

    /*
     * parentIndex returns the parent location for a child location.
     */
    int parentIndex(int childIndex) const {
        return (childIndex - 1) / 2; // Uses the standard heap parent formula.
    }

    /*
     * leftChildIndex returns the left child location for a parent location.
     */
    int leftChildIndex(int parent) const {
        return (2 * parent) + 1; // Uses the standard heap left child formula.
    }

    /*
     * rightChildIndex returns the right child location for a parent location.
     */
    int rightChildIndex(int parent) const {
        return (2 * parent) + 2; // Uses the standard heap right child formula.
    }

    /*
     * swapEmails swaps two emails inside the heap list.
     */
    void swapEmails(int firstIndex, int secondIndex) {
        Email temporary = heapList[firstIndex];       // Saves the first email temporarily.
        heapList[firstIndex] = heapList[secondIndex]; // Moves the second email into the first spot.
        heapList[secondIndex] = temporary;            // Moves the saved email into the second spot.
    }

    /*
     * moveUp restores heap order after inserting a new email.
     */
    void moveUp(int currentIndex) {
        while (currentIndex > 0) {                              // Continues until the root is reached.
            int parent = parentIndex(currentIndex);             // Finds the parent of the current email.

            if (heapList[currentIndex].hasHigherPriorityThan(heapList[parent])) { // Checks if child should rise.
                swapEmails(currentIndex, parent);               // Swaps child with parent.
                currentIndex = parent;                          // Continues checking from the parent spot.
            } else {                                            // Stops when heap order is correct.
                break;                                          // Exits the loop.
            }
        }
    }

    /*
     * moveDown restores heap order after removing the root email.
     */
    void moveDown(int currentIndex) {
        int heapSize = static_cast<int>(heapList.size()); // Stores the current number of heap items.

        while (true) {                                    // Continues until the email is in the right spot.
            int left = leftChildIndex(currentIndex);      // Finds the left child.
            int right = rightChildIndex(currentIndex);    // Finds the right child.
            int highest = currentIndex;                   // Assumes current email has highest priority.

            if (left < heapSize && heapList[left].hasHigherPriorityThan(heapList[highest])) { // Checks left child.
                highest = left; // Marks left child as highest so far.
            }

            if (right < heapSize && heapList[right].hasHigherPriorityThan(heapList[highest])) { // Checks right child.
                highest = right; // Marks right child as highest so far.
            }

            if (highest != currentIndex) {       // Checks whether a child should move up.
                swapEmails(currentIndex, highest); // Swaps current email with the higher-priority child.
                currentIndex = highest;            // Continues checking from the child spot.
            } else {                              // Stops when heap order is correct.
                break;                            // Exits the loop.
            }
        }
    }

public:
    /*
     * insert adds a new email to the heap.
     */
    void insert(const Email& newEmail) {
        heapList.push_back(newEmail);                      // Adds the email to the end of the list.
        moveUp(static_cast<int>(heapList.size()) - 1);     // Moves it up until heap order is restored.
    }

    /*
     * isEmpty tells whether the heap has no emails.
     */
    bool isEmpty() const {
        return heapList.empty(); // Returns true when there are no emails.
    }

    /*
     * count returns the number of emails in the heap.
     */
    int count() const {
        return static_cast<int>(heapList.size()); // Returns the unread email count.
    }

    /*
     * getMax returns the highest-priority email without removing it.
     */
    const Email& getMax() const {
        return heapList[0]; // The root of a MaxHeap is always the highest-priority item.
    }

    /*
     * removeMax removes the highest-priority email from the heap.
     */
    void removeMax() {
        if (heapList.empty()) { // Checks whether there is nothing to remove.
            return;             // Leaves immediately when the heap is empty.
        }

        heapList[0] = heapList.back(); // Moves the last email to the root.
        heapList.pop_back();           // Removes the duplicate last email.

        if (!heapList.empty()) {       // Checks whether any email remains.
            moveDown(0);               // Restores heap order from the root.
        }
    }
};

/*
 * EmailPriorityQueue controls how commands affect the CEO's inbox.
 */
class EmailPriorityQueue {
private:
    MaxHeap inboxHeap;      // Stores unread emails in priority order.
    int nextArrivalNumber;  // Gives every email a unique arrival order.

public:
    /*
     * Constructor starts with an empty inbox.
     */
    EmailPriorityQueue() {
        nextArrivalNumber = 1; // Starts arrival order at 1.
    }

    /*
     * addEmailFromCommand parses an EMAIL command and inserts the email.
     */
    void addEmailFromCommand(const string& commandLine) {
        string emailFields = commandLine.substr(6);              // Removes the word "EMAIL " from the line.
        size_t firstComma = emailFields.find(',');               // Finds the comma after sender category.
        size_t secondComma = emailFields.find(',', firstComma + 1); // Finds the comma after subject line.

        string sender = emailFields.substr(0, firstComma);       // Extracts sender category.
        string subject = emailFields.substr(firstComma + 1, secondComma - firstComma - 1); // Extracts subject.
        string dateText = emailFields.substr(secondComma + 1);   // Extracts date.

        Email newEmail(sender, subject, dateText, nextArrivalNumber); // Creates the email object.
        inboxHeap.insert(newEmail);                              // Adds the email to the MaxHeap.
        nextArrivalNumber++;                                     // Updates arrival order for the next email.
    }

    /*
     * showNext displays the highest-priority email without deleting it.
     */
    void showNext() const {
        if (inboxHeap.isEmpty()) {                    // Checks whether there are no unread emails.
            cout << "There are no emails to read." << endl; // Handles empty NEXT safely.
            return;                                   // Leaves the function after printing the message.
        }

        cout << "Next email:" << endl; // Prints the required NEXT heading.
        inboxHeap.getMax().display();  // Displays the highest-priority email.
    }

    /*
     * readNext removes the highest-priority email.
     */
    void readNext() {
        if (inboxHeap.isEmpty()) {                    // Checks whether there are no unread emails.
            cout << "There are no emails to read." << endl; // Handles empty READ safely.
            return;                                   // Leaves the function after printing the message.
        }

        inboxHeap.removeMax(); // Removes the highest-priority email.
    }

    /*
     * showCount prints the current number of unread emails.
     */
    void showCount() const {
        cout << "There are " << inboxHeap.count() << " emails to read." << endl; // Prints count format.
    }
};

/*
 * main reads each command from the test file and sends it to the inbox object.
 */
int main() {
    EmailPriorityQueue ceoInbox; // Creates the CEO's email priority queue.
    string commandLine;          // Stores one command line at a time.

    while (getline(cin, commandLine)) {                  // Reads commands until the file ends.
        commandLine = StringHelper::trim(commandLine);   // Cleans extra whitespace from the command.

        if (commandLine.empty()) {                       // Ignores blank lines in the input file.
            continue;                                    // Moves to the next line.
        }

        if (commandLine.length() >= 6 && commandLine.substr(0, 6) == "EMAIL ") { // Checks for EMAIL command.
            ceoInbox.addEmailFromCommand(commandLine);   // Adds the email to the priority queue.
        } else if (commandLine == "NEXT") {              // Checks for NEXT command.
            ceoInbox.showNext();                         // Displays the next email without removing it.
        } else if (commandLine == "READ") {              // Checks for READ command.
            ceoInbox.readNext();                         // Removes the next email.
        } else if (commandLine == "COUNT") {             // Checks for COUNT command.
            ceoInbox.showCount();                        // Displays the unread email count.
        }
    }

    return 0; // Ends the program successfully.
}
