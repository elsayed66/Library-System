#include <iostream>
#include <cstdlib>          // cls
#include <windows.h>        // Sleep
#include <cstring>          // string, to_string
#include <windows.h>        // SetConsoleTextAttribute
#include <algorithm>        // sort
#include <conio.h>          // _getch()


using namespace std;
HANDLE cout_handle = GetStdHandle(STD_OUTPUT_HANDLE);



/** CONSTANTS **/
const int MAX_BOOKS_NUM = 100;
const int MAX_USERS_NUM = 100;

/** DEFINED STRUCTS **/
struct Book{
    int id{-1};
    int totalQuantity{0};
    int totalBorrowed{0};
    string name;

    string borrowingList[MAX_USERS_NUM];
    int borrowingCount{0};

    void inputData(int i_id,string & i_name,int i_totalQuantity){
        id = i_id;
        name = i_name;
        totalQuantity = i_totalQuantity;
    }
    void print(){
        printf("id = %-4d name = %-8s total Quantity : %-4d total Borrowed : %-4d ",id,name.c_str(),totalQuantity,totalBorrowed);
        // 4d -> means each integer will take for cells
    }
    bool checkByPrefix(string& prefix, int indx= 0){
        //recursively check if the name contains specifix prefix
        if(name.size()<prefix.size()) return false;
        if(indx >= prefix.size()) return true; // base case
        if(name[indx] != prefix[indx]) return false; // check first char
        return checkByPrefix(prefix, indx + 1); // check other chars
    }
    bool borrowBook(string& name){
        // check if all books are borrowed
        if(totalQuantity <=0 ) return false;
        ++totalBorrowed;
        --totalQuantity;
        borrowingList[borrowingCount++] = name;
        return true;
    }
    void printWhoBorrowed(){
        cout<<endl;
        for (int i = 0; i < borrowingCount; ++i){
            cout<<borrowingList[i]<<'\n';
        }
    }
    void returnBook(string& i_Username){
        ++totalQuantity;
        --totalBorrowed;

        // search for the user's name to return book

        for (int i = 0; i < borrowingCount; ++i) {
            if(borrowingList[i] == i_Username){
                // shifting cells left to delete data of this cell
                for (int j = i; j < borrowingCount-1; ++j) {
                    borrowingList[j] = borrowingList[j+1];
                }
                --borrowingCount;
                break;
            }
        }
    }

};

struct User{
    string name;
    int nationalId=0;
    int borrowedBooksIds[100];
    int borrowedCount{0};

    void borrowBook(Book& book){
        borrowedBooksIds[borrowedCount++] = book.id;
    }
    void inputData(string& i_name,int i_nationalId){
        name = i_name;
        nationalId = i_nationalId;
    }
    void returnBook(Book& book){
        for (int i = 0; i < borrowedCount; ++i) {
            if(book.id == borrowedBooksIds[i]){
                // shift left to delete this cell
                for (int j = i; j < borrowedCount-1 ; ++j) {
                    borrowedBooksIds[j] = borrowedBooksIds[j+1];
                }
                --borrowedCount;
                break;
            }
        }
    }

    void print(){
        printf("user: %-8s id: %-5d borrowed ids: ",name.c_str(),nationalId);
        // print borrowed ids
        for (int i = 0; i < borrowedCount; ++i) {
            cout<<borrowedBooksIds[i]<<' ';
        }
    }
    bool checkByPrefix(string prefix, int indx= 0){
        //recursively check if the name contains specifix prefix

        if(indx >= prefix.size()) return true; // base case
        if(name[indx] != prefix[indx]) return false; // check first char
        return checkByPrefix(prefix, indx + 1); // check other chars
    }
};

/** GLOBAL VARIABLES **/
int booksCount;
Book booksList[MAX_BOOKS_NUM]{};
int usersCount;
User usersList[MAX_USERS_NUM]{};

/** ASSISTANT INLINE FUNCTIONS **/
inline void wait_or_clear(unsigned int sec, bool clear_screen = false)
{
    Sleep(sec*1000);
    if (clear_screen) system("cls");
}
inline void printLine(string str, bool el = false, int colorCode = 15) {
    SetConsoleTextAttribute(cout_handle,colorCode);
    cout << str << (el ? '\n' : '\t');
}

/** FUNCTIONS DECLARATIONS **/
int menu();
void librarySystem();
void app_intro();
void app_outro();
void add_book();
void search_book_by_prefix();
void print_library_by_id_or_name(bool);
void add_user();
void print_users();
void user_borrow_book();
void print_who_borrowed_book_by_name();
void user_return_book();



int main() {
    librarySystem();
    return 0;
}

/** FUNCTIONS DEFINITIONS **/
void librarySystem(){
    app_intro();
    while(true){
        wait_or_clear(0,1); // clear the screen to print the menu
        int choice {menu()};
        switch (choice) {
            case 1: add_book(); break;
            case 2: search_book_by_prefix(); break;
            case 3: print_who_borrowed_book_by_name();break;
            case 4: print_library_by_id_or_name(0);break;
            case 5: print_library_by_id_or_name(1);break;
            case 6: add_user();break;
            case 7: user_borrow_book(); break;
            case 8: user_return_book(); break;
            case 9: print_users(); break;
            default: app_outro();return;
        }
    }
}

void app_intro(){
    printLine("\n\n\n\n\n\n\t\t\t\t===========================", 1);
    printLine("\t\t\t\t      Library System", 1,1);
    printLine("\t\t\t\t===========================", 1);
    wait_or_clear(1);
}

void app_outro(){
    wait_or_clear(0,1);
    printLine("\n\n\n\t\t\tThank you for using our software :: ",1,120);
}

int menu(){
    int choice;
    printLine("\t\t\tMain Menu",1);
    printLine("\t\t    -----------------\n\n",1);
    printLine("1) add Book", 1);
    printLine("2) Search books by prefix", 1);
    printLine("3) Print who borrowed book by name", 1);
    printLine("4) Print library by Id", 1);
    printLine("5) Print library by name", 1);
    printLine("6) Add user", 1);
    printLine("7) user Borrow book", 1);
    printLine("8) user return book", 1);
    printLine("9) Print users", 1);
    printLine("10) Exit", 1);

    printLine("\tEnter your choice [1 - 10] :"),cin >> choice;
    return choice;
}
void add_book(){
    int id,totQuantity;
    string name;
    printLine("Enter book info: id & name & total Quantity : ");
    cin>>id>>name>>totQuantity;
    booksList[booksCount++].inputData(id, name, totQuantity);

    printLine("\n\t\tnew book was Added Successfully :) ", 1);
    wait_or_clear(1);
}

void search_book_by_prefix(){
    string key;
    printLine("Enter book name prefix : ");
    cin>>key;
    bool oneFound = false;
    for (int i = 0; i < booksCount; ++i) {
        if(booksList[i].checkByPrefix(key)) {
            printLine(booksList[i].name, 1);
            if(!oneFound) oneFound = true;
        }
    }
    if(!oneFound) cout<<"\a\t\tNo books with such a prefix!!!";
    wait_or_clear(2);
}
bool compareBooksIds(Book& b1 ,Book& b2){
    return (b1.id<b2.id);
}
bool compareBooksName(Book& b1 ,Book& b2){
    return (b1.name <= b2.name);
}

void print_library_by_id_or_name(bool sortType = 0){
    // sort based on ids
    if(sortType) // 0 -> sort by Id , 1-> sort by name
        sort(booksList,booksList+booksCount, compareBooksName);
    else sort(booksList,booksList+booksCount, compareBooksIds);

    // print sorted list
    for (int i = 0; i < booksCount; ++i) {
        booksList[i].print();
        cout<<'\n';
    }
    printLine("\t\t enter any key to proceed :");
    _getch();       // stop the cursor till any char entered
}

void add_user(){
    // validate on inputs can be done here
    string name; int nationalId;
    printLine("Enter user name & national id: ");

    cin>>name>>nationalId;
    usersList[usersCount].inputData(name,nationalId);
    ++usersCount;
    printLine("\n\t\tnew User was Added Successfully :) ", 1);
    wait_or_clear(1);
}
void print_users(){
    cout<<endl;

    if(usersCount <= 0) {
        cout<<"\a\t\t\tNo Users found!!!\n";
        wait_or_clear(1,1);
        return;
    }
    // print data if users found
    for (int i = 0; i < usersCount; ++i) {
        usersList[i].print();
        cout<<'\n';
    }
    cout<<endl;
    printLine("\n\t\t enter any key to proceed :");
    _getch();       // stop the cursor till any char entered
}
void user_borrow_book() {

    string usName, bkName;
    printLine("Enter user name and book's name : ");
    cin >> usName >> bkName;
    bool foundUser{false}, foundBook{false};
    int bkIndx, usrIndx;
    //Search for the user with name
    for (int i = 0; i < usersCount; ++i) {
        if (usersList[i].checkByPrefix(usName)) {
            usrIndx = i;
            foundUser = true;
            break;
        }
    }
    //Search for the Book with name
    for (int i = 0; i < booksCount; ++i) {
        if (booksList[i].checkByPrefix(bkName)) {
            bkIndx = i;
            foundBook = true;
            break;
        }
    }
    // validate the data
    if (!(foundUser && foundBook)) {
        cout << "\n\a\aError in input Data!!\n";
        wait_or_clear(1);
        return;
    }

    // use a bool to check if borrow book succeeded
    bool borrowingDone = booksList[bkIndx].borrowBook(usName);
    if(borrowingDone) {
        usersList[usrIndx].borrowBook(booksList[bkIndx]);
        printLine("\n\a\t\tprocess Done Successfully",1);
    }
    else printLine("\n\a\t\tNo Available books!",1);

    wait_or_clear(1);
}
void print_who_borrowed_book_by_name(){
    string name;
    printLine("Enter book name : ");
    cin>>name;

    bool foundBook;
    for (int i = 0; i < booksCount; ++i) {
        foundBook = false;
        foundBook = booksList[i].checkByPrefix(name);
        if(foundBook){
            booksList[i].printWhoBorrowed();
            break;
        }
    }
    if(!foundBook) printLine("\aInvalid book Name!!",1);

    printLine("\n\t\t enter any key to proceed :");
    _getch();       // stop the cursor till any char entered
}

void user_return_book(){
    string usName, bkName;
    printLine("Enter user name and book's name : ");
    cin >> usName >> bkName;
    bool foundUser{false}, foundBook{false};
    int bkIndx, usrIndx;
    //Search for the user with name
    for (int i = 0; i < usersCount; ++i) {
        if (usersList[i].checkByPrefix(usName)) {
            usrIndx = i;
            foundUser = true;
            break;
        }
    }
    //Search for the Book with name
    for (int i = 0; i < booksCount; ++i) {
        if (booksList[i].checkByPrefix(bkName)) {
            bkIndx = i;
            foundBook = true;
            break;
        }
    }
    // validate the data
    if (!(foundUser && foundBook)) {
        cout << "\n\a\aError in input Data!!\n";
        wait_or_clear(1);
        return;
    }
    booksList[bkIndx].returnBook(usName);
    usersList[usrIndx].returnBook(booksList[bkIndx]);
    printLine("Process Done",1);
    wait_or_clear(1);
}