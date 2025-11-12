// integrated_lumina.cpp
// Full integration of all code blocks provided by the user.
// All original text/logic preserved. Duplicate mains merged into one flow.

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

// ---------------------- COMMON STRUCTS ----------------------
struct RoomBooking {
    int roomNum;
    string roomType;
    int roomPrice;
};

struct GuestInfo {
    string name;
    int age;
    string dateStr;
};

// ---------------------- FORWARD DECLARATIONS ----------------------
// Introduction / flow
void introduction(GuestInfo &guest);
void separationSystem(GuestInfo &guest);
void shutdownSystem();

// Guest booking module (original functions)
void listRooms();
void listAvailable();
void bookRoom();
void cancelBook();
void viewBooked();

// Another booking module (the other large booking block)
RoomBooking getRoomInfo_v2(int roomNum);
void bookingLoop_v2(); // the second big booking main logic

// Customer Management and billing/invoice
void customerManagement_v1(); // first small customer management snippet
void customerManagement_v2(); // improved V2
void billingSystem();
void invoiceSystem();
void printInvoice(const string &name, int age, const string &roomNumber, const string &roomType,
                  const string &checkIn, const string &checkOut, int nights, int rate,
                  const string &paymentMethod, const string &paymentStatus, const string &specialRequest);

// Staff features
void staffMenu(); // integrated staff menu
void analyticsReport();
void realTimeAvailability();
void reservationCancellation(); // uses booking file cancellation

// Misc helpers
void pauseReturn();

// ---------------------- IMPLEMENTATION ----------------------
string username;
// ---------- INTRODUCTION ----------
void introduction(GuestInfo &guest) {
    // Preserved original introduction prompts (adapted to avoid re-declaring main)
    cout << "***Welcome to Lumina Lodgings!***" << endl;
    cout << "****Please enjoy your stay.****" << endl;
    cout << "" << endl;
    cout << "Please input your name" << endl;
    cin >> username;
    cout << "" << endl;
    cout << "Welcome " << guest.name << endl;;
    cout << "" << endl;
    cout << "Please input your age" << endl;
    while (!(cin >> guest.age)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter numeric age: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "" << endl;
    cout << "Please input the date (Month, Day, Year)" << endl;
    getline(cin, guest.dateStr);
    cout << endl;
}

// ---------- GUEST BOOKING MODULE (original functions) ----------
void listRooms() {
    string roomNum, roomType, bedNum, dailyRate, status;
    ifstream RoomsFile;
    // original used ios::out incorrectly; corrected to ios::in but preserved behavior
    RoomsFile.open("rooms_file.txt", ios::in);
    if (!RoomsFile) {
        cout << "No file was read" << endl;
    }
    else {
        string line;
        cout << left << setw(15) << "Room Number" << setw(25) << "Room Type" << setw(15) << "Beds" << setw(15) << "Daily Rate" << setw(15) << "Status" << endl;
        while (getline(RoomsFile,line)) {
            stringstream ss(line);
            getline(ss, roomNum, ',');
            getline(ss, roomType, ',');
            getline(ss, bedNum, ',');
            getline(ss, dailyRate, ',');
            getline(ss, status);
            cout << left << setw(15) << roomNum << setw(25) << roomType << setw(15) << bedNum << setw(15) << dailyRate <<setw(15) <<status <<endl;
        }
    }
    cout << endl;
}

void listAvailable() {
    string roomNum, roomType, bedNum, dailyRate, status;
    ifstream RoomsFile;
    RoomsFile.open("rooms_file.txt", ios::in);
    if (!RoomsFile) {
        cout << "No file was read" << endl;
        return;
    }
    cout << setw(15) << "Room Number" << setw(25) << "Room Type" << setw(15) << "Beds" << setw(15) << "Daily Rate" <<setw(15) <<"Status" <<endl;
    string line;
    while (getline(RoomsFile,line)) {
        stringstream ss(line);
        getline(ss, roomNum, ',');
        getline(ss, roomType, ',');
        getline(ss, bedNum, ',');
        getline(ss, dailyRate, ',');
        getline(ss, status);
        if(status=="Available"){
            cout << setw(15) << roomNum << setw(25) << roomType << setw(15) << bedNum << setw(15) << dailyRate <<setw(15) <<status <<endl;
        }
    }
    cout << endl;
    return;
}

void bookRoom() {
    int a=0;
    string get_roomNum, roomNum, roomType, bedNum, dailyRate, status;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout<<"Enter the room number you would like to book: ";
    getline(cin,get_roomNum);

    ifstream roomsFile;
    roomsFile.open("rooms_file.txt", ios::in);
    ofstream bookRoomFile;
    ofstream roomsFileCopy;
    roomsFileCopy.open("roomsFileCopy.txt", ios::app);
    bookRoomFile.open("usrBookedRooms.txt", ios::app);

    if(!roomsFile) {
        cout << "rooms_file.txt not found." << endl;
        // close files if any opened
        if(roomsFileCopy.is_open()) roomsFileCopy.close();
        if(bookRoomFile.is_open()) bookRoomFile.close();
        return;
    }

    string line;
    bool found = false;
    while(getline(roomsFile, line)) {
        stringstream ss(line);
        getline(ss, roomNum, ',');
        getline(ss, roomType, ',');
        getline(ss, bedNum, ',');
        getline(ss, dailyRate, ',');
        getline(ss, status);
        if (roomNum==get_roomNum) {
            found = true;
            if (status!="Occupied") {
                bookRoomFile <<roomNum <<"," <<roomType <<"," <<bedNum <<"," <<dailyRate <<endl;   
                roomsFileCopy <<roomNum <<"," <<roomType <<"," <<bedNum <<"," <<dailyRate <<',' << "Occupied"  <<endl;
            } else {
                cout<<"The room is already occupied."<<endl<<endl;
                roomsFile.close();
                roomsFileCopy.close();
                bookRoomFile.close();
                // cleanup copy file
                remove("roomsFileCopy.txt");
                return;
            }

        } else {
            roomsFileCopy <<roomNum <<"," <<roomType <<"," <<bedNum <<"," <<dailyRate <<',' << status  <<endl;
        }

    }

    if(!found) {
        cout << "Room number not found." << endl;
        roomsFile.close();
        roomsFileCopy.close();
        bookRoomFile.close();
        remove("roomsFileCopy.txt");
        return;
    }

    cout<<"Booked room successfully"<<endl;
    roomsFile.close();
    roomsFileCopy.close();
    bookRoomFile.close();
    remove("rooms_file.txt");
    rename("roomsFileCopy.txt", "rooms_file.txt");
    return;
}

void cancelBook() {
    string roomNum, roomType, bedNum, dailyRate, get_roomNum, line, status;
    ifstream usrBookedIn;
    ifstream roomsFile;
    // original code had usrBookedIn.open("borrowed_books.txt", ios::in);
    // It appears they intended a user-booked-file; preserve original but add fallback to usrBookedRooms.txt
    roomsFile.open("rooms_file.txt", ios::in);
    usrBookedIn.open("usrBookedRooms.txt", ios::in);
    if(!usrBookedIn) { // try fallback filename used earlier in some snippets
        usrBookedIn.open("borrowed_books.txt", ios::in);
    }

    ofstream usrBookedOut;
    ofstream copyRoomsFile;
    usrBookedOut.open("usrBookedOut.txt", ios::app);
    copyRoomsFile.open("copyRoomsFile.txt", ios::app);

    cout<<"Enter the room number you would like to cancel: ";
    cin>>get_roomNum;

    if(usrBookedIn) {
        while (getline(usrBookedIn,line)) {
            stringstream ss(line);
            getline(ss, roomNum, ',');
            getline(ss, roomType, ',');
            getline(ss, bedNum, ',');
            getline(ss, dailyRate);

            if (roomNum!=get_roomNum) {
                usrBookedOut <<roomNum <<"," <<roomType <<"," <<bedNum <<"," <<dailyRate<<endl;
            }    
        }
    } else {
        // no booked file found
    }

    if(roomsFile) {
        while (getline(roomsFile, line)) {
            stringstream ss(line);
            getline(ss, roomNum, ',');
            getline(ss, roomType, ',');
            getline(ss, bedNum, ',');
            getline(ss, dailyRate, ',');
            getline(ss, status);

            if (roomNum==get_roomNum) {
                copyRoomsFile <<roomNum <<"," <<roomType <<"," <<bedNum <<"," <<dailyRate<<",Available" <<endl;
            } else {
                copyRoomsFile <<roomNum <<"," <<roomType <<"," <<bedNum <<"," <<dailyRate<<"," <<status <<endl;
            }
        }
    } else {
        cout << "rooms_file.txt not found while cancelling." << endl;
    }

    copyRoomsFile.close();
    usrBookedOut.close();
    if(usrBookedIn.is_open()) usrBookedIn.close();
    if(roomsFile.is_open()) roomsFile.close();

    remove("rooms_file.txt");
    remove("usrBookedRooms.txt");
    rename("usrBookedOut.txt", "usrBookedRooms.txt");
    rename("copyRoomsFile.txt", "rooms_file.txt");
    return;
}

void viewBooked() {
    string roomNum, roomType, bedNum, dailyRate, status;
    ifstream usrBooked;
    usrBooked.open("usrBookedRooms.txt", ios::in);
    if (!usrBooked) {
        cout<<"No file was read"<<endl;
    }
    else {
        cout << setw(15) << "Room Number" << setw(25) << "Room Type" << setw(15) << "Beds" << setw(15) << "Daily Rate" <<endl;
        string line;
        while (getline(usrBooked,line)) {
            stringstream ss(line);
            getline(ss, roomNum, ',');
            getline(ss, roomType, ',');
            getline(ss, bedNum, ',');
            getline(ss, dailyRate);
            cout << left << setw(15) << roomNum << setw(25) << roomType << setw(15) << bedNum << setw(15) << dailyRate <<endl;
        }   
    }
    cout<<endl;
    return;
}

// ---------- SECOND BOOKING MODULE (the larger booking logic) ----------
RoomBooking getRoomInfo_v2(int roomNum) {
    RoomBooking room{};
    room.roomNum = roomNum;

    switch (roomNum) {
        case 1001: case 1002: case 1003: case 1004:
            room.roomType = "Single";
            room.roomPrice = 1000;
            break;
        case 2001: case 2002: case 2003: case 2004:
            room.roomType = "Double";
            room.roomPrice = 2000;
            break;
        default:
            room.roomType = "Invalid";
            room.roomPrice = 0;
    }
    return room;
}

void bookingLoop_v2() {
    vector<RoomBooking> bookedRooms;
    int sched, totalPrice = 0;
    char rep, select, add;
    string guestName;

    cout << " \t\t\t\t\t*****************************************\n";
    cout << " \t\t\t\t\t*       Welcome to Lumina Lodging       *\n";
    cout << " \t\t\t\t\t*               By Group 4              *\n";
    cout << " \t\t\t\t\t*****************************************\n\n";

    fstream file("bookings.txt", ios::in | ios::out | ios::app);
    if (!file) {
        cout << "Error opening bookings.txt\n";
        return;
    }

    do {
        bookedRooms.clear();
        totalPrice = 0;

        cout <<"\n*************** ROOM LIST ****************\n";
        cout <<" Room Number \t Type \t\t Price (PHP)" << endl;
        cout <<"-------------------------------------------\n";
        cout<<" 1001 \t\t Single \t 1000\n";
        cout<<" 1002 \t\t Single \t 1000\n";
        cout<<" 1003 \t\t Single \t 1000\n";
        cout<<" 1004 \t\t Single \t 1000\n";
        cout<<" 2001 \t\t Double \t 2000\n";
        cout<<" 2002 \t\t Double \t 2000\n";
        cout<<" 2003 \t\t Double \t 2000\n";
        cout<<" 2004 \t\t Double \t 2000\n";
        cout << "*****************************************\n";

        do {
            int roomNum;
            cout<< "\nEnter Room Number: ";
            cin>> roomNum;

            RoomBooking room = getRoomInfo_v2(roomNum);

            if(room.roomPrice == 0) {
                cout<< "Invalid room number. Please try again.\n";
                continue;
            }

            cout<<"\n***********************\n";
            cout<<" Room Number: " << room.roomNum << endl;
            cout<<" Type: " << room.roomType << endl;
            cout<<" Price: PHP " << room.roomPrice << endl;
            cout<<"***********************\n";

            cout<<"\nDo you want to proceed with this room? [Y/N]: ";
            cin>> rep;

            if (rep == 'Y' || rep == 'y') {
                bookedRooms.push_back(room);
            }

            cout<<"Do you want to add another room? [Y/N]: ";
            cin>> add;
        } while(add == 'Y' || add == 'y');

        cout<<"\nHow many day/s will you stay?: ";
        cin>> sched;

        for(auto &room : bookedRooms) {
            totalPrice += room.roomPrice;
        }
        totalPrice *= sched;

        cout<<"\n\n*******************************\n";
        cout<<" Rooms booked: " << bookedRooms.size() << endl;
        cout<<" Days: " << sched << endl;
        cout<<" Total Price: PHP " << totalPrice << endl;
        cout<<"*******************************\n";

        cout<<" Confirm this transaction? [Y/N]: ";
        cin>> select;

        if(select == 'Y' || select == 'y') {
            cout<<"Enter guest name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, guestName);

            file<< left << setw(20) << guestName
                << setw(10) << sched
                << setw(10) << totalPrice
                << "Rooms: ";

            for(auto &room : bookedRooms)
                file << room.roomNum << " ";
            file << endl;

            cout<<"\nTransaction saved successfully!\n";
        }

        cout<<"\nDo you want to book again? [Y/N]: ";
        cin>> rep;

    } while (rep == 'Y' || rep == 'y');

    file.close();
    cout << "\nAll bookings have been saved in 'bookings.txt'.\n";
    return;
}

// ---------- CUSTOMER MANAGEMENT / BILLING / INVOICING ----------
void customerManagement_v1() {
    int guestchoice;
    char again;

    do {
        cout << "==============================" << endl;
        cout << "      CUSTOMER MANAGEMENT     " << endl;
        cout << "==============================" << endl;

        cout << "[1] SICKS SEVEN" << endl;
        cout << "[2] ZACKT CORTA" << endl;
        cout << "[3] -UNRESERVED-" << endl;
        cout << "[4] TATANG CRUZ" << endl;
        cout << "[5] -UNRESERVED-" << endl;
        cout << "[6] JOHN WICKED" << endl;
        cout << "[7] -UNRESERVED-" << endl;
        cout << "[8] GINOONG HALIMAW" << endl;
        cout << "==============================" << endl;

        cout << "Enter number (1-8): ";
        cin >> guestchoice;
        cout << endl;

        switch (guestchoice) {
            case 1: {
                int nights = 3;
                int rate = 1000;
                cout << "ROOM NUMBER: 1001\nROOM TYPE: Single Bed Room\nPRICE PER NIGHT: ₱" << rate << endl;
                cout << "NAME: SICKS SEVEN\nAGE: 67\nHOW MANY PEOPLE: 1" << endl;
                cout << "Check-in: 10/20/25\nCheck-out: 10/23/25" << endl;
                cout << "Length of stay: " << nights << " nights" << endl;
                cout << "Total Price: ₱" << nights * rate << endl;
                cout << "Special requests: Extra pillows and a cup of warm tea." << endl;
                break;
            }

            case 2: {
                int nights = 2;
                int rate = 1000;
                cout << "ROOM NUMBER: 1002\nROOM TYPE: Single Bed Room\nPRICE PER NIGHT: ₱" << rate << endl;
                cout << "NAME: ZACKT CORTA\nAGE: 18\nHOW MANY PEOPLE: 1" << endl;
                cout << "Check-in: 10/25/25\nCheck-out: 10/27/25" << endl;
                cout << "Length of stay: " << nights << " nights" << endl;
                cout << "Total Price: ₱" << nights * rate << endl;
                cout << "Special requests: Quiet room with good Wi-Fi signal." << endl;
                break;
            }

            case 3:
                cout << "ROOM NUMBER: 1003\nROOM TYPE: Single Bed Room\nPRICE PER NIGHT: ₱1,000" << endl;
                cout << "This room is available for reservations." << endl;
                break;

            case 4: {
                int nights = 5;
                int rate = 2000;
                cout << "ROOM NUMBER: 2001\nROOM TYPE: Double Bed Room\nPRICE PER NIGHT: ₱" << rate << endl;
                cout << "NAME: TATANG CRUZ\nAGE: 100\nHOW MANY PEOPLE: 1" << endl;
                cout << "Check-in: 10/10/25\nCheck-out: 10/15/25" << endl;
                cout << "Length of stay: " << nights << " nights" << endl;
                cout << "Total Price: ₱" << nights * rate << endl;
                cout << "Special requests: Room close to elevator." << endl;
                break;
            }

            case 5:
                cout << "ROOM NUMBER: 2002\nROOM TYPE: Double Bed Room\nPRICE PER NIGHT: ₱2,000" << endl;
                cout << "This room is available for reservations." << endl;
                break;

            case 6: {
                int nights = 4;
                int rate = 2000;
                cout << "ROOM NUMBER: 2003\nROOM TYPE: Double Bed Room\nPRICE PER NIGHT: ₱" << rate << endl;
                cout << "NAME: JOHN WICKED\nAGE: 31\nHOW MANY PEOPLE: 1" << endl;
                cout << "Check-in: 10/18/25\nCheck-out: 10/22/25" << endl;
                cout << "Length of stay: " << nights << " nights" << endl;
                cout << "Total Price: ₱" << nights * rate << endl;
                cout << "Special requests: Near parking area." << endl;
                break;
            }

            case 7:
                cout << "ROOM NUMBER: 2004\nROOM TYPE: Double Bed Room\nPRICE PER NIGHT: ₱2,000" << endl;
                cout << "This room is available for reservations." << endl;
                break;

            case 8: {
                int nights = 3;
                int rate = 2000;
                cout << "ROOM NUMBER: 2004\nROOM TYPE: Double Bed Room\nPRICE PER NIGHT: ₱" << rate << endl;
                cout << "NAME: GINOONG HALIMAW\nAGE: 22\nHOW MANY PEOPLE: 1" << endl;
                cout << "Check-in: 10/23/25\nCheck-out: 10/26/25" << endl;
                cout << "Length of stay: " << nights << " nights" << endl;
                cout << "Total Price: ₱" << nights * rate << endl;
                cout << "Special requests: Room with strong air conditiong." << endl;
                break;
            }

            default:
                cout << "Invalid choice. Please enter a number between 1 and 8." << endl;
                break;
        }

        cout << "\nWould you like to check another guest? (Y/N): ";
        cin >> again;
        cout << endl;

    } while (again == 'Y' || again == 'y');

    cout << "Thank you for using the Customer Management System!" << endl;
    // return to caller (no exit)
}

void customerManagement_v2() {
    int guestchoice;
    char again = 'Y';

    while (again == 'Y' || again == 'y') {
        cout << "==============================\n";
        cout << "       CUSTOMER MANAGEMENT     \n";
        cout << "==============================\n";
        cout << "[1] SICKS SEVEN\n";
        cout << "[2] ZACKT CORTA\n";
        cout << "[3] -UNRESERVED-\n";
        cout << "[4] TATANG CRUZ\n";
        cout << "[5] -UNRESERVED-\n";
        cout << "[6] JOHN WICKED\n";
        cout << "[7] -UNRESERVED-\n";
        cout << "[8] GINOONG HALIMAW\n";
        cout << "==============================\n";
        cout << "Enter number (1-8): ";

        while (!(cin >> guestchoice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number (1-8): ";
        }

        cout << endl;

        switch (guestchoice) {
            case 1: {
                int nights = 3, rate = 1000;
                cout << "ROOM NUMBER: 1001\nROOM TYPE: Single Bed Room\nPRICE PER NIGHT: ₱" << rate << endl;
                cout << "NAME: SICKS SEVEN\nAGE: 67\nHOW MANY PEOPLE: 1\n";
                cout << "Check-in: 10/20/25\nCheck-out: 10/23/25\n";
                cout << "Length of stay: " << nights << " nights\n";
                cout << "Total Price: ₱" << nights * rate << endl;
                cout << "Special requests: Extra pillows and a cup of warm tea.\n";
                break;
            }
            case 2: {
                int nights = 2, rate = 1000;
                cout << "ROOM NUMBER: 1002\nROOM TYPE: Single Bed Room\nPRICE PER NIGHT: ₱" << rate << endl;
                cout << "NAME: ZACKT CORTA\nAGE: 18\nHOW MANY PEOPLE: 1\n";
                cout << "Check-in: 10/25/25\nCheck-out: 10/27/25\n";
                cout << "Length of stay: " << nights << " nights\n";
                cout << "Total Price: ₱" << nights * rate << endl;
                cout << "Special requests: Quiet room with good Wi-Fi signal.\n";
                break;
            }
            case 3:
                cout << "ROOM NUMBER: 1003\nROOM TYPE: Single Bed Room\nPRICE PER NIGHT: ₱1,000\n";
                cout << "This room is available for reservations.\n";
                break;
            case 4: {
                int nights = 5, rate = 2000;
                cout << "ROOM NUMBER: 2001\nROOM TYPE: Double Bed Room\nPRICE PER NIGHT: ₱" << rate << endl;
                cout << "NAME: TATANG CRUZ\nAGE: 100\nHOW MANY PEOPLE: 1\n";
                cout << "Check-in: 10/10/25\nCheck-out: 10/15/25\n";
                cout << "Length of stay: " << nights << " nights\n";
                cout << "Total Price: ₱" << nights * rate << endl;
                cout << "Special requests: Room close to elevator.\n";
                break;
            }
            case 5:
                cout << "ROOM NUMBER: 2002\nROOM TYPE: Double Bed Room\nPRICE PER NIGHT: ₱2,000\n";
                cout << "This room is available for reservations.\n";
                break;
            case 6: {
                int nights = 4, rate = 2000;
                cout << "ROOM NUMBER: 2003\nROOM TYPE: Double Bed Room\nPRICE PER NIGHT: ₱" << rate << endl;
                cout << "NAME: JOHN WICKED\nAGE: 31\nHOW MANY PEOPLE: 1\n";
                cout << "Check-in: 10/18/25\nCheck-out: 10/22/25\n";
                cout << "Length of stay: " << nights << " nights\n";
                cout << "Total Price: ₱" << nights * rate << endl;
                cout << "Special requests: Near parking area.\n";
                break;
            }
            case 7:
                cout << "ROOM NUMBER: 2004\nROOM TYPE: Double Bed Room\nPRICE PER NIGHT: ₱2,000\n";
                cout << "This room is available for reservations.\n";
                break;
            case 8: {
                int nights = 3, rate = 2000;
                cout << "ROOM NUMBER: 2005\nROOM TYPE: Double Bed Room\nPRICE PER NIGHT: ₱" << rate << endl;
                cout << "NAME: GINOONG HALIMAW\nAGE: 22\nHOW MANY PEOPLE: 1\n";
                cout << "Check-in: 10/23/25\nCheck-out: 10/26/25\n";
                cout << "Length of stay: " << nights << " nights\n";
                cout << "Total Price: ₱" << nights * rate << endl;
                cout << "Special requests: Room with strong air conditioning.\n";
                break;
            }
            default:
                cout << "Invalid choice. Please enter a number between 1 and 8.\n";
                break;
        }

        cout << "\nWould you like to check another guest? (Y/N): ";
        cin >> again;
        cout << endl;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    cout << "Thank you for using the Customer Management System!" << endl;
    // returns to caller
}

void billingSystem() {
    int guestchoice;
    string email;
    char again;

    do {
        cout << "\n===== CUSTOMER MANAGEMENT SYSTEM =====\n";
        cout << "[1] SICKS SEVEN\n";
        cout << "[2] ZACKT CORTA\n";
        cout << "[3] -UNRESERVED-\n";
        cout << "[4] TATANG CRUZ\n";
        cout << "[5] -UNRESERVED-\n";
        cout << "[6] JOHN WICKED\n";
        cout << "[7] -UNRESERVED-\n";
        cout << "[8] GINOONG HALIMAW\n";
        cout << "======================================\n\n";

        cout << "Enter number: ";
        cin >> guestchoice;
        cout << endl;

        bool reserved = true; // flag to control billing system

        switch (guestchoice) {
            case 1:
                cout << "ROOM NUMBER: 1001\n";
                cout << "ROOM TYPE: Single Bed Room\n";
                cout << "PRICE PER NIGHT: ₱1,000\n";
                cout << "NAME: SICKS SEVEN\n";
                cout << "AGE: 67\n";
                cout << "HOW MANY PEOPLE: 1\n";
                cout << "Check-in: 10/22/25\n";
                cout << "Check-out: 10/23/25\n";
                cout << "Special requests: Extra pillows and a cup of warm tea.\n";
                cout << "Total Bill: ₱1,000\n";
                break;

            case 2:
                cout << "ROOM NUMBER: 1002\n";
                cout << "ROOM TYPE: Single Bed Room\n";
                cout << "PRICE PER NIGHT: ₱1,000\n";
                cout << "NAME: ZACKT CORTA\n";
                cout << "AGE: 18\n";
                cout << "HOW MANY PEOPLE: 1\n";
                cout << "Check-in: 10/25/25\n";
                cout << "Check-out: 10/27/25\n";
                cout << "Special requests: Quiet room with good Wi-Fi signal.\n";
                cout << "Total Bill: ₱2,000\n";
                break;

            case 3:
                cout << "The room is available for reservations.\n";
                reserved = false;
                break;

            case 4:
                cout << "ROOM NUMBER: 2001\n";
                cout << "ROOM TYPE: Double Bed Room\n";
                cout << "PRICE PER NIGHT: ₱2,000\n";
                cout << "NAME: TATANG CRUZ\n";
                cout << "AGE: 100\n";
                cout << "HOW MANY PEOPLE: 1\n";
                cout << "Check-in: 10/20/25\n";
                cout << "Check-out: 10/23/25\n";
                cout << "Special requests: Room close to elevator.\n";
                cout << "Total Bill: ₱6,000\n";
                break;

            case 5:
                cout << "The room is available for reservations.\n";
                reserved = false;
                break;

            case 6:
                cout << "ROOM NUMBER: 2002\n";
                cout << "ROOM TYPE: Double Bed Room\n";
                cout << "PRICE PER NIGHT: ₱2,000\n";
                cout << "NAME: JOHN WICKED\n";
                cout << "AGE: 31\n";
                cout << "HOW MANY PEOPLE: 1\n";
                cout << "Check-in: 10/24/25\n";
                cout << "Check-out: 10/26/25\n";
                cout << "Special requests: Near parking area.\n";
                cout << "Total Bill: ₱4,000\n";
                break;

            case 7:
                cout << "The room is available for reservations.\n";
                reserved = false;
                break;

            case 8:
                cout << "ROOM NUMBER: 2003\n";
                cout << "ROOM TYPE: Double Bed Room\n";
                cout << "PRICE PER NIGHT: ₱2,000\n";
                cout << "NAME: GINOONG HALIMAW\n";
                cout << "AGE: 22\n";
                cout << "HOW MANY PEOPLE: 1\n";
                cout << "Check-in: 10/21/25\n";
                cout << "Check-out: 10/24/25\n";
                cout << "Special requests: Room with strong air conditioning.\n";
                cout << "Total Bill: ₱6,000\n";
                break;

            default:
                cout << "Invalid choice.\n";
                reserved = false;
                break;
        }

        // Only reserved rooms will ask for email and send bill
        if (reserved) {
            cout << "\nEnter guest email to send the bill: ";
            cin >> email;

            cout << "\n--------------------------------------\n";
            cout << "Sending bill to: " << email << endl;
            cout << "Your Room Reservation bill has been sent successfully!\n";
            cout << "Thank you for choosing our hotel!\n";
            cout << "--------------------------------------\n";
        }

        cout << "\nDo you want to return to the system? (Y/N): ";
        cin >> again;

    } while (again == 'Y' || again == 'y');

    cout << "\nExiting system... Thank you!\n";
    // returns to caller
}

void printInvoice(const string &name, int age, const string &roomNumber, const string &roomType,
                  const string &checkIn, const string &checkOut, int nights, int rate,
                  const string &paymentMethod, const string &paymentStatus, const string &specialRequest) {
    int subtotal = nights * rate;
    int totalAmount = subtotal;

    cout << "------------------------------------------------------------\n";
    cout << "                    LUMINA LODGING                          \n";
    cout << "------------------------------------------------------------\n";
    cout << "INVOICE NO.: INV-" << roomNumber << setw(25) << "DATE: 10/27/25" << endl;
    cout << "\nGUEST NAME: " << name << setw(30 - (int)name.length()) << "AGE: " << age << endl;
    cout << "ROOM NUMBER: " << roomNumber << setw(25 - (int)roomNumber.length()) << "ROOM TYPE: " << roomType << endl;
    cout << "CHECK-IN: " << checkIn << setw(27 - (int)checkIn.length()) << "CHECK-OUT: " << checkOut << endl;
    cout << "NUMBER OF GUESTS: 1\n\n";

    cout << "------------------------------------------------------------\n";
    cout << left << setw(25) << "Description"
         << setw(10) << "Qty"
         << setw(10) << "Price"
         << setw(10) << "Total" << endl;
    cout << "------------------------------------------------------------\n";

    cout << left << setw(25) << ("Room Rate (₱" + to_string(rate) + "/night)")
         << setw(10) << nights
         << "₱" << setw(9) << rate
         << "₱" << subtotal << endl;

    cout << "------------------------------------------------------------\n";
    cout << right << setw(42) << "Subtotal: ₱" << subtotal << endl;
    cout << right << setw(42) << "TOTAL AMOUNT DUE: ₱" << totalAmount << endl;
    cout << "------------------------------------------------------------\n";
    cout << "Payment Method: " << paymentMethod << endl;
    cout << "Payment Status: " << paymentStatus << endl;
    cout << "Special Requests: " << specialRequest << endl;
    cout << "------------------------------------------------------------\n";
    cout << "               THANK YOU FOR STAYING WITH US!               \n";
    cout << "------------------------------------------------------------\n";
    cout << "Returning to system...\n\n";
}

void invoiceSystem() {
    int guestChoice;
    char again = 'Y';
    do {
        cout << "\n==============================\n";
        cout << "         HOTEL INVOICE        \n";
        cout << "==============================\n";
        cout << "[1] Room 1001 - SICKS SEVEN\n";
        cout << "[2] Room 1002 - ZACKT CORTA\n";
        cout << "[3] Room 1003 - (UNRESERVED)\n";
        cout << "[4] Room 2001 - TATANG CRUZ\n";
        cout << "[5] Room 2002 - (UNRESERVED)\n";
        cout << "[6] Room 2003 - JOHN WICKED\n";
        cout << "[7] Room 2004 - (UNRESERVED)\n";
        cout << "[8] Room 2005 - GINOONG HALIMAW\n";
        cout << "======================================\n\n";

        cout << "Enter number: ";
        cin >> guestChoice;
        cout << endl;

        switch (guestChoice) {
            case 1:
                printInvoice("SICKS SEVEN", 67, "1001", "Single Bed Room",
                             "10/20/25", "10/23/25", 3, 1000,
                             "Cash", "Paid", "Extra pillows and a cup of warm tea before bed.");
                break;
            case 2:
                printInvoice("ZACKT CORTA", 18, "1002", "Single Bed Room",
                             "10/25/25", "10/27/25", 2, 1000,
                             "Cash", "Paid", "Quiet room with good Wi-Fi signal for gaming.");
                break;
            case 3:
                cout << "ROOM NUMBER: 1003\nROOM TYPE: Single Bed Room\nPRICE PER NIGHT: ₱1,000" << endl;
                cout << "This room is available for reservations.\n";
                cout << "Returning to system...\n\n";
                break;
            case 4:
                printInvoice("TATANG CRUZ", 100, "2001", "Double Bed Room",
                             "10/10/25", "10/15/25", 5, 2000,
                             "Cash", "Paid", "Wheelchair access and soft lighting.");
                break;
            case 5:
                cout << "ROOM NUMBER: 2002\nROOM TYPE: Double Bed Room\nPRICE PER NIGHT: ₱2,000" << endl;
                cout << "This room is available for reservations.\n";
                cout << "Returning to system...\n\n";
                break;
            case 6:
                printInvoice("JOHN WICKED", 31, "2003", "Double Bed Room",
                             "10/18/25", "10/22/25", 4, 2000,
                             "Cash", "Paid", "Soundproof room and early breakfast delivery.");
                break;
            case 7:
                cout << "ROOM NUMBER: 2004\nROOM TYPE: Double Bed Room\nPRICE PER NIGHT: ₱2,000" << endl;
                cout << "This room is available for reservations.\n";
                cout << "Returning to system...\n\n";
                break;
            case 8:
                printInvoice("GINOONG HALIMAW", 22, "2005", "Double Bed Room",
                             "10/23/25", "10/26/25", 3, 2000,
                             "Cash", "Paid", "Room near gym with midnight snack delivery.");
                break;
            default:
                cout << "Invalid selection. Please choose 1–8.\n";
                cout << "Returning to system...\n\n";
                break;
        }

        cout << "\nDo you want to return to the system? (Y/N): ";
        cin >> again;

    } while (again == 'Y' || again == 'y');

    cout << "\nExiting invoice system... Thank you!\n";
}

// ---------- STAFF FEATURES ----------
void analyticsReport() {
    cout<<"Total guests this month: 67"<<endl;
    cout<<"Most common age (7-range): 26-33 (38.07%)"<<endl; // use age variable if available
    cout<<"Most Popular Room (%): Single (41.23%)"<<endl;
    cout<<"Highest peak (For 1 day): 27/28 Rooms Occupied"<<endl;
}

void realTimeAvailability() {
    cout << "Real-time availability display (sample):\n";
    cout << "Room 1001 - Available\nRoom 1002 - Occupied\nRoom 1003 - Available\nRoom 2001 - Occupied\n";
}

void reservationCancellation() {
    // Reuse cancelBook logic (but keep wrapper to match flow)
    cout << "Reservation Cancellation selected.\n";
    cancelBook();
}

// Staff menu that integrates Customer Management, Billing, Invoicing, Analytics, Real-time, Cancellation
void staffMenu() {
    int staffchoice;
    bool loop1=true;
    while (loop1){
        cout<<""<<endl;
        cout<<"[1] Customer Management"<<endl;
        cout<<"[2] Billing"<<endl;
        cout<<"[3] Invoicing"<<endl;
        cout<<"[4] Analytic Report"<<endl;
        cout<<"[5] Real-time Availability Display"<<endl;
        cout<<"[6] Reservation Cancellation"<<endl;
        cout<<"[7] Booking Module (list/book/view)"<<endl;
        cout<<"[8] Shutdown Program\n";

        cout<<"Input your choice: "<<endl;
        cin>>staffchoice;
        cout<<""<<endl;

        switch (staffchoice){
            case 1:
                customerManagement_v1();
                break;
            case 2:
                billingSystem();
                break;
            case 3:
                invoiceSystem();
                break;
            case 4:
                analyticsReport();
                break;
            case 5:
                realTimeAvailability();
                break;
            case 6:
                reservationCancellation();
                break;
            case 7:
                // Provide a small in-staff menu for guest-booking functions
                {
                    int sub;
                    cout << "Staff Booking Tools:\n[1] listRooms\n[2] listAvailable\n[3] bookRoom\n[4] viewBooked\nEnter choice: ";
                    cin >> sub;
                    switch(sub){
                        case 1: listRooms(); break;
                        case 2: listAvailable(); break;
                        case 3: { cin.ignore(numeric_limits<streamsize>::max(), '\n'); bookRoom(); break; }
                        case 4: viewBooked(); break;
                        default: cout << "Invalid.\n"; break;
                    }
                }
                break;
                
             case 8:
                cout<<"Shutting Down..."<<endl;
                loop1=false;
                break;
 
            default:
                cout<<"Unknown Command Detected, Please Try Again"<<endl;
                cout<<""<<endl;
                break;
        }
    }
}

// ---------- SEPARATION SYSTEM ----------
void separationSystem(GuestInfo &guest) {
    char auth;
    int pass, accNum;

    cout<<"=================================\n";
    cout<<"Enter Account Type (S for Staff, G for Guest): ";
    cin>>auth;
    cout<<"\n=================================";

    switch(auth)
    {
        case 'S': case 's':
            cout<< "\nEnter Staff ID: ";
            cin>>accNum;
            if(accNum == 678901) {
                cout << "\nEnter Password: ";
                cin>> pass;
                cout<<"\n=================================";
                if(pass == 123035) {
                    cout<<"\nWelcome Staff Member "<<username<<endl;
                    // call staff menu (integrated)
                    staffMenu();
                } else {
                    cout<<"Incorrect password.\n";
                }
            } else {
                cout<<"Invalid Staff ID.\n";
            }
            break;
        case 'G': case 'g':
            cout<<"\nWELCOME GUEST!\n";
            // Present guest options that use all guest booking code
            {
                int choice;
                bool guestLoop = true;
                while(guestLoop) {
                    cout << "\n==== GUEST MENU ====\n";
                    cout << "[1]. Display catalog\n";
                    cout << "[2]. Display available rooms\n";
                    cout << "[3]. Book a room\n";
                    cout << "[4]. Cancel booking\n";
                    cout << "[5]. View booked rooms\n";
                    cout << "[6]. Exit guest menu\n";
                    cout << "Choice: ";
                    cin >> choice;
                    switch(choice) {
                        case 1: listRooms(); break;
                        case 2: listAvailable(); break;
                        case 3: { cin.ignore(numeric_limits<streamsize>::max(), '\n'); bookRoom(); break; }
                        case 4: cancelBook(); break;
                        case 5: viewBooked(); break;
                        case 6: guestLoop = false; break;
                        default: cout << "Invalid.\n"; break;
                    }
                }
            }
            break;
        default:
            cout << "Invalid account type.\n";
            break;
    }
}

// ---------- SHUTDOWN ----------
void shutdownSystem() {
    cout << "\nSystem shutting down. Goodbye!\n";
    // flush files if needed
}

// ---------- MAIN ----------
int main() {
    GuestInfo guest;
    // run single integrated flow
    introduction(guest);
    separationSystem(guest);
    shutdownSystem();
    return 0;
}
