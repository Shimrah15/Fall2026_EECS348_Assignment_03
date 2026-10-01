/*
Program: EECS 348 Assignment 3
Description: This program prioritizes CEO emails using an object-oriented MaxHeap priority queue.
Inputs: The program reads EMAIL, NEXT, READ, and COUNT commands from standard input.
Output: The program displays the next email for the CEO and the current number of unread emails.
Collaborators: None.
Sources: ChatGPT, Gemini
Author: Shimrah
Creation Date: October 1, 2026
Revision Date: October 1, 2026
Revisions: Improved the generated code by fixing the output format, READ behavior, date comparison, and class organization.
*/

#include <cctype>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*
The Email class stores one email and knows how to compare itself against another email.
The MaxHeap uses this comparison to decide which email should move to the top.
*/
class Email {
private:
    string senderCategory;
    string subjectLine;
    string dateText;
    int arrivalOrder;

public:
    /*
    Default constructor used when an empty Email object is needed.
    */
    Email() {
        senderCategory = "";
        subjectLine = "";
        dateText = "";
        arrivalOrder = 0;
    }

    /*
    Constructor used when the program creates a real email from an EMAIL command.
    */
    Email(string sender, string subject, string date, int order) {
        senderCategory = sender;
        subjectLine = subject;
        dateText = date;
        arrivalOrder = order;
    }

    /*
    Returns the sender category so it can be printed in the required output format.
    */
    string getSenderCategory() const {
        return senderCategory;
    }

    /*
    Returns the subject line so it can be printed in the required output format.
    */
    string getSubjectLine() const {
        return subjectLine;
    }

    /*
    Returns the original date string so the output keeps the MM-DD-YYYY format.
    */
    string getDateText() const {
        return dateText;
    }

    /*
    Converts the sender category into a numeric priority.
    A larger number means the email should be read earlier.
    */
    int getSenderPriority() const {
        if (senderCategory == "Boss") {
            return 5;
        }
        if (senderCategory == "Subordinate") {
            return 4;
        }
        if (senderCategory == "Peer") {
            return 3;
        }
        if (senderCategory == "ImportantPerson") {
            return 2;
        }
        return 1;
    }

    /*
    Converts MM-DD-YYYY into YYYYMMDD so normal integer comparison can decide which date is newer.
    */
    int getDateValue() const {
