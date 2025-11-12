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

// Guest booking module (original function names preserved)
void listRooms();
void listAvailable();
void bookRoom();
void cancelBook();
void viewBooked();

// Another booking module (the other large booking block)
RoomBooking getRoomInfo_v2(int roomNum);
void bookingLoop_v2(); // the second big booking main logic (adapted to single DB)

// Customer Management and billing/invoice (function names preserved)
void customerManagement_v1(); // first small customer management snippet
void customerManagement_v2(); // improved V2
void billingSystem();
void invoiceSystem();
void printInvoice(const string &name, int age, const string &roomNumber, const string &roomType,
                  const string &checkIn, const string &checkOut, int nights, int rate,
                  const string &paymentMethod, const string &paymentStatus, const string &specialRequest);

// Staff features
void staffMenu(); // integrated staff menu (unchanged layout)
void analyticsReport();
void realTimeAvailability();
void reservationCancellation(); // wrapper to cancelBook()

// Database helpers (new)
void initializeDatabase();
vector<string> splitFields(const string &line, char delim='|');
bool findRecordByRoom(const string &roomNum, vector<string> &outFields, string &outLine);
bool updateRecordLine(const string &oldLine, const string &newLine);
void writeDefaultDatabaseIfMissing();
string trim(const string &s);

// Misc helpers
void pauseReturn();

// ---------------------- IMPLEMENTATION ----------------------

string username; // small variable used by introduction / staff greeting
const string DB_FILENAME = "hotel_database.txt";

// ---------- DATABASE HELPERS ----------
string trim(const string &s) {
    size_t a = 0;
    while (a < s.size() && isspace((unsigned char)s[a])) ++a;
    size_t b = s.size();
    while (b > a && isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b-a);
}

vector<string> splitFields(const string &line, char delim) {
    vector<string> out;
    string field;
    stringstream ss(line);
    while (getline(ss, field, delim)) {
        out.push_back(field);
    }
    return out;
}

// Replace a single line in DB file (oldLine -> newLine). Returns true on success.
bool updateRecordLine(const string &oldLine, const string &newLine) {
    ifstream fin(DB_FILENAME);
    if (!fin) return false;
    ofstream fout("db_tmp.txt");
    string line;
    bool replaced = false;
    while (getline(fin, line)) {
        if (!replaced && line == oldLine) {
            fout << newLine << '\n';
            replaced = true;
        } else {
            fout << line << '\n';
        }
    }
    fin.close();
    fout.close();
    if (!replaced) {
        // no change
        remove("db_tmp.txt");
        return false;
    }
    remove(DB_FILENAME.c_str());
    rename("db_tmp.txt", DB_FILENAME.c_str());
    return true;
}

// Find record by room number. If found, returns true and fills outFields (split by '|') and outLine (original).
bool findRecordByRoom(const string &roomNum, vector<string> &outFields, string &outLine) {
    ifstream fin(DB_FILENAME);
    if (!fin) return false;
    string line;
    while (getline(fin, line)) {
        vector<string> fields = splitFields(line, '|');
        if (!fields.empty() && trim(fields[0]) == trim(roomNum)) {
            outFields = fields;
            outLine = line;
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

// Create default DB if missing
void writeDefaultDatabaseIfMissing() {
    ifstream fin(DB_FILENAME);
    if (fin) { fin.close(); return; } // already exists
    ofstream fout(DB_FILENAME);
    if (!fout) {
        cout << "Warning: Cannot create " << DB_FILENAME << " in current directory.\n";
        return;
    }
    // Format per line:
    // roomNum|roomType|price|name|age|people|checkIn|checkOut|nights|note
    // For unreserved rooms, name = UNRESERVED and note = Available
    fout << "1001|Single Bed Room|1000|SICKS SEVEN|67|1|10/20/25|10/23/25|3|Extra pillows and a cup of warm tea.\n";
    fout << "1002|Single Bed Room|1000|ZACKT CORTA|18|1|10/25/25|10/27/25|2|Quiet room with good Wi-Fi signal.\n";
    fout << "1003|Single Bed Room|1000|UNRESERVED|-|-|-|-|-|Available\n";
    fout << "1004|Single Bed Room|1000|UNRESERVED|-|-|-|-|-|Available\n";
    fout << "2001|Double Bed Room|2000|TATANG CRUZ|100|1|10/10/25|10/15/25|5|Room close to elevator.\n";
    fout << "2002|Double Bed Room|2000|UNRESERVED|-|-|-|-|-|Available\n";
    fout << "2003|Double Bed Room|2000|JOHN WICKED|31|1|10/18/25|10/22/25|4|Near parking area.\n";
    fout << "2004|Double Bed Room|2000|UNRESERVED|-|-|-|-|-|Available\n";
    fout << "2005|Double Bed Room|2000|GINOONG HALIMAW|22|1|10/23/25|10/26/25|3|Room with strong air conditioning.\n";
    fout.close();
    cout << "Created default database file: " << DB_FILENAME << " (you can edit it manually if needed)\n";
}

void initializeDatabase() {
    writeDefaultDatabaseIfMissing();
}

// ---------- INTRODUCTION ----------
void introduction(GuestInfo &guest) {
    // Preserved original introduction prompts (adapted to avoid re-declaring main)
    cout << "***Welcome to Lumina Lodgings!***" << endl;
    cout << "****Please enjoy your stay.****" << endl;
    cout << "" << endl;
    cout << "Please input your name" << endl;
    // read single-word name like original code did (cin >> username)
    cin >> username;
    guest.name = username; // set guest struct name too
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

// ---------- GUEST BOOKING MODULE (reworked to single DB) ----------
void listRooms() {
    // prints all rooms from DB with columns similar to original listRooms
    ifstream fin(DB_FILENAME);
    if (!fin) {
        cout << "No file was read (database missing)." << endl;
        return;
    }
    cout << left << setw(15) << "Room Number" << setw(25) << "Room Type" << setw(10) << "Beds" << setw(15) << "Daily Rate" << setw(15) << "Status" << endl;
    string line;
    while (getline(fin, line)) {
        vector<string> f = splitFields(line, '|');
        if (f.size() >= 10) {
            string roomNum = trim(f[0]);
            string roomType = trim(f[1]);
            string price = trim(f[2]);
            string name = trim(f[3]);
            string people = trim(f[5]); // beds equivalence
            string note = trim(f[9]);
            string status = (name == "UNRESERVED" || name == "-") ? "Available" : "Occupied";
            cout << left << setw(15) << roomNum << setw(25) << roomType << setw(10) << people << setw(15) << price << setw(15) << status << endl;
        }
    }
    fin.close();
    cout << endl;
}

void listAvailable() {
    ifstream fin(DB_FILENAME);
    if (!fin) {
        cout << "No file was read (database missing)." << endl;
        return;
    }
    cout << setw(15) << "Room Number" << setw(25) << "Room Type" << setw(10) << "Beds" << setw(15) << "Daily Rate" << setw(15) << "Status" << endl;
    string line;
    while (getline(fin, line)) {
        vector<string> f = splitFields(line, '|');
        if (f.size() >= 10) {
            string roomNum = trim(f[0]);
            string roomType = trim(f[1]);
            string price = trim(f[2]);
            string name = trim(f[3]);
            string people = trim(f[5]);
            string status = (name == "UNRESERVED" || name == "-") ? "Available" : "Occupied";
            if (status == "Available") {
                cout << setw(15) << roomNum << setw(25) << roomType << setw(10) << people << setw(15) << price << setw(15) << status << endl;
            }
        }
    }
    fin.close();
    cout << endl;
}

void bookRoom() {
    // Adapted to single DB: mark an UNRESERVED room as occupied, writing guest info into the DB line
    cout << "Enter the room number you would like to book: ";
    string get_roomNum;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, get_roomNum);
    get_roomNum = trim(get_roomNum);

    vector<string> fields;
    string origLine;
    if (!findRecordByRoom(get_roomNum, fields, origLine)) {
        cout << "Room number not found.\n";
        return;
    }
    // fields layout: 0 roomNum|1 roomType|2 price|3 name|4 age|5 people|6 checkIn|7 checkOut|8 nights|9 note
    string currName = trim(fields[3]);
    if (currName != "UNRESERVED" && currName != "-") {
        cout << "The room is already occupied.\n";
        return;
    }

    // gather booking details from user
    string guestName;
    int guestAge;
    string numPeople;
    string checkIn, checkOut;
    int nights;

    cout << "Enter guest name: ";
    getline(cin, guestName);
    cout << "Enter guest age: ";
    while (!(cin >> guestAge)) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Enter numeric age: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "How many people: ";
    getline(cin, numPeople);
    cout << "Check-in (MM/DD/YY): ";
    getline(cin, checkIn);
    cout << "Check-out (MM/DD/YY): ";
    getline(cin, checkOut);
    cout << "Number of nights: ";
    while (!(cin >> nights)) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Enter numeric nights: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // build new line replacing fields 3..9
    // Keep price intact at fields[2]
    string newLine = fields[0] + "|" + fields[1] + "|" + fields[2] + "|"
                     + guestName + "|" + to_string(guestAge) + "|" + numPeople + "|"
                     + checkIn + "|" + checkOut + "|" + to_string(nights) + "|"
                     + "Booked via system";

    if (updateRecordLine(origLine, newLine)) {
        cout << "Booked room successfully\n";
    } else {
        cout << "Failed to update database. Try again.\n";
    }
}

void cancelBook() {
    // Adapted to single DB: set the record back to UNRESERVED
    cout << "Enter the room number you would like to cancel: ";
    string get_roomNum;
    cin >> get_roomNum;
    get_roomNum = trim(get_roomNum);
    vector<string> fields;
    string origLine;
    if (!findRecordByRoom(get_roomNum, fields, origLine)) {
        cout << "Room not found.\n";
        return;
    }
    // build new line setting fields 3..9 to UNRESERVED / dashes
    string newLine = fields[0] + "|" + fields[1] + "|" + fields[2] + "|UNRESERVED|-|-|-|-|-|Available";
    if (updateRecordLine(origLine, newLine)) {
        cout << "Reservation for room " << get_roomNum << " cancelled.\n";
    } else {
        cout << "Failed to cancel reservation (database error).\n";
    }
}

void viewBooked() {
    // Show bookings: all lines where name != UNRESERVED
    ifstream fin(DB_FILENAME);
    if (!fin) { cout << "No file was read (database missing).\n"; return; }
    cout << setw(15) << "Room Number" << setw(25) << "Room Type" << setw(20) << "Guest Name" << setw(8) << "Age" << setw(8) << "Nights" << setw(15) << "Daily Rate" << endl;
    string line;
    while (getline(fin, line)) {
        vector<string> f = splitFields(line, '|');
        if (f.size() >= 10) {
            string name = trim(f[3]);
            if (name != "UNRESERVED" && name != "-") {
                cout << left << setw(15) << trim(f[0]) << setw(25) << trim(f[1]) << setw(20) << trim(f[3]) << setw(8) << trim(f[4]) << setw(8) << trim(f[8]) << setw(15) << trim(f[2]) << endl;
            }
        }
    }
    fin.close();
    cout << endl;
}

// ---------- SECOND BOOKING MODULE (Group-4 booking loop) ----------
RoomBooking getRoomInfo_v2(int roomNum) {
    RoomBooking room{};
    room.roomNum = roomNum;

    switch (roomNum) {
        case 1001: case 1002: case 1003: case 1004:
            room.roomType = "Single";
            room.roomPrice = 1000;
            break;
        case 2001: case 2002: case 2003: case 2004: case 2005:
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
    // Adapted: when user confirms, we will mark each selected room in hotel_database.txt
    vector<RoomBooking> bookedRooms;
    int sched, totalPrice = 0;
    char rep, select, add;
    string guestName;

    cout << " \t\t\t\t\t*****************************************\n";
    cout << " \t\t\t\t\t*       Welcome to Lumina Lodging       *\n";
    cout << " \t\t\t\t\t*               By Group 4              *\n";
    cout << " \t\t\t\t\t*****************************************\n\n";

    // we won't open a separate bookings.txt now; we'll write into hotel_database.txt entries
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
        cout<<" 2005 \t\t Double \t 2000\n";
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

            // We'll set the first room's details (and all selected rooms if desired)
            // Ask for additional details:
            string ageStr, peopleStr, checkIn, checkOut;
            cout << "Enter guest age: ";
            int gAge;
            while(!(cin >> gAge)) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Enter numeric age: "; }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Enter number of people in room: ";
            getline(cin, peopleStr);
            cout << "Enter check-in date (MM/DD/YY): ";
            getline(cin, checkIn);
            cout << "Enter check-out date (MM/DD/YY): ";
            getline(cin, checkOut);

            // Update DB for each room in bookedRooms
            for (auto &room : bookedRooms) {
                string roomNumStr = to_string(room.roomNum);
                vector<string> f;
                string origLine;
                if (findRecordByRoom(roomNumStr, f, origLine)) {
                    string newLine = f[0] + "|" + f[1] + "|" + f[2] + "|" + guestName + "|" + to_string(gAge) + "|" + peopleStr + "|" + checkIn + "|" + checkOut + "|" + to_string(sched) + "|Booked via bookingLoop_v2";
                    if (!updateRecordLine(origLine, newLine)) {
                        cout << "Warning: could not update room " << roomNumStr << " in DB.\n";
                    }
                } else {
                    cout << "Warning: room " << roomNumStr << " not found in DB.\n";
                }
            }

            cout<<"\nTransaction saved successfully!\n";
        }

        cout<<"\nDo you want to book again? [Y/N]: ";
        cin>> rep;

    } while (rep == 'Y' || rep == 'y');

    cout << "\nBooking flow finished. (All bookings saved into " << DB_FILENAME << ")\n";
    return;
}

// ---------- CUSTOMER MANAGEMENT / BILLING / INVOICING (adapted to single DB) ----------
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

        // Map choices to room numbers:
        // 1->1001,2->1002,3->1003,4->2001,5->2002,6->2003,7->2004,8->2005
        vector<string> mapRooms = {"1001","1002","1003","2001","2002","2003","2004","2005"};
        string roomSel = mapRooms[(guestchoice>=1 && guestchoice<=8) ? (guestchoice-1) : 0];

        vector<string> f;
        string line;
        if (guestchoice < 1 || guestchoice > 8) {
            cout << "Invalid choice. Please enter a number between 1 and 8." << endl;
        } else {
            if (findRecordByRoom(roomSel, f, line) && f.size() >= 10) {
                string name = trim(f[3]);
                string age = trim(f[4]);
                string people = trim(f[5]);
                string checkIn = trim(f[6]);
                string checkOut = trim(f[7]);
                string nights = trim(f[8]);
                string priceStr = trim(f[2]);
                int rate = 0;
                try { rate = stoi(priceStr); } catch(...) { rate = 0; }
                int nightsInt = 0;
                try { nightsInt = stoi(nights); } catch(...) { nightsInt = 0; }

                if (name == "UNRESERVED" || name == "-") {
                    // keep similar message to original "This room is available for reservations."
                    cout << "ROOM NUMBER: " << roomSel << "\nROOM TYPE: " << trim(f[1]) << "\nPRICE PER NIGHT: ₱" << rate << endl;
                    cout << "This room is available for reservations." << endl;
                } else {
                    cout << "ROOM NUMBER: " << roomSel << "\nROOM TYPE: " << trim(f[1]) << "\nPRICE PER NIGHT: ₱" << rate << endl;
                    cout << "NAME: " << name << "\nAGE: " << age << "\nHOW MANY PEOPLE: " << people << endl;
                    cout << "Check-in: " << checkIn << "\nCheck-out: " << checkOut << endl;
                    cout << "Length of stay: " << nightsInt << " nights" << endl;
                    cout << "Total Price: ₱" << (nightsInt * rate) << endl;
                    cout << "Special requests: " << trim(f[9]) << endl;
                }
            } else {
                cout << "Data for room " << roomSel << " not found in the database.\n";
            }
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

        vector<string> mapRooms = {"1001","1002","1003","2001","2002","2003","2004","2005"};
        if (guestchoice < 1 || guestchoice > 8) {
            cout << "Invalid choice. Please enter a number between 1 and 8.\n";
        } else {
            string roomSel = mapRooms[guestchoice - 1];
            vector<string> f; string line;
            if (findRecordByRoom(roomSel, f, line) && f.size() >= 10) {
                string name = trim(f[3]);
                string age = trim(f[4]);
                string people = trim(f[5]);
                string checkIn = trim(f[6]);
                string checkOut = trim(f[7]);
                string nights = trim(f[8]);
                string price = trim(f[2]);
                int nightsInt=0, rate=0;
                try { nightsInt = stoi(nights); } catch(...) { nightsInt = 0; }
                try { rate = stoi(price); } catch(...) { rate = 0; }

                if (name == "UNRESERVED" || name == "-") {
                    cout << "ROOM NUMBER: " << roomSel << "\nROOM TYPE: " << trim(f[1]) << "\nPRICE PER NIGHT: ₱" << rate << endl;
                    cout << "This room is available for reservations.\n";
                } else {
                    cout << "ROOM NUMBER: " << roomSel << "\nROOM TYPE: " << trim(f[1]) << "\nPRICE PER NIGHT: ₱" << rate << endl;
                    cout << "NAME: " << name << "\nAGE: " << age << "\nHOW MANY PEOPLE: " << people << "\n";
                    cout << "Check-in: " << checkIn << "\nCheck-out: " << checkOut << "\n";
                    cout << "Length of stay: " << nightsInt << " nights\n";
                    cout << "Total Price: ₱" << nightsInt * rate << endl;
                    cout << "Special requests: " << trim(f[9]) << "\n";
                }
            } else {
                cout << "No data found for that room.\n";
            }
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

        bool reserved = true; // assume reserved unless DB says UNRESERVED

        vector<string> mapRooms = {"1001","1002","1003","2001","2002","2003","2004","2005"};
        if (guestchoice < 1 || guestchoice > 8) {
            cout << "Invalid choice.\n";
            reserved = false;
        } else {
            string roomSel = mapRooms[guestchoice - 1];
            vector<string> f; string line;
            if (findRecordByRoom(roomSel, f, line) && f.size() >= 10) {
                string name = trim(f[3]);
                int rate = 0;
                int nights = 0;
                try { rate = stoi(trim(f[2])); } catch(...) { rate = 0; }
                try { nights = stoi(trim(f[8])); } catch(...) { nights = 0; }

                if (name == "UNRESERVED" || name == "-") {
                    cout << "The room is available for reservations.\n";
                    reserved = false;
                } else {
                    cout << "ROOM NUMBER: " << roomSel << "\n";
                    cout << "ROOM TYPE: " << trim(f[1]) << "\n";
                    cout << "PRICE PER NIGHT: ₱" << rate << "\n";
                    cout << "NAME: " << name << "\n";
                    cout << "AGE: " << trim(f[4]) << "\n";
                    cout << "HOW MANY PEOPLE: " << trim(f[5]) << "\n";
                    cout << "Check-in: " << trim(f[6]) << "\n";
                    cout << "Check-out: " << trim(f[7]) << "\n";
                    cout << "Special requests: " << trim(f[9]) << "\n";
                    cout << "Total Bill: ₱" << (nights * rate) << "\n";
                    reserved = true;
                }
            } else {
                cout << "Invalid choice.\n";
                reserved = false;
            }
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

        vector<string> mapRooms = {"1001","1002","1003","2001","2002","2003","2004","2005"};
        if (guestChoice < 1 || guestChoice > 8) {
            cout << "Invalid selection. Please choose 1–8.\n";
        } else {
            string roomSel = mapRooms[guestChoice - 1];
            vector<string> f; string line;
            if (findRecordByRoom(roomSel, f, line) && f.size() >= 10) {
                string name = trim(f[3]);
                if (name == "UNRESERVED" || name == "-") {
                    cout << "ROOM NUMBER: " << roomSel << "\nROOM TYPE: " << trim(f[1]) << "\nSTATUS: Available for reservation.\n";
                    cout << "Returning to system...\n\n";
                } else {
                    int rate = 0, nights = 0;
                    try { rate = stoi(trim(f[2])); } catch(...) { rate = 0; }
                    try { nights = stoi(trim(f[8])); } catch(...) { nights = 0; }
                    printInvoice(trim(f[3]), stoi(trim(f[4])), roomSel, trim(f[1]), trim(f[6]), trim(f[7]), nights, rate, "Cash", "Paid", trim(f[9]));
                }
            } else {
                cout << "Data not found for that room.\n";
            }
        }

        cout << "\nDo you want to return to the system? (Y/N): ";
        cin >> again;

    } while (again == 'Y' || again == 'y');

    cout << "\nExiting invoice system... Thank you!\n";
}

// ---------- STAFF FEATURES ----------
void analyticsReport() {
    cout<<"Total guests this month: 67"<<endl;
    cout<<"Most common age (7-range): 26-33 (38.07%)"<<endl; // static text preserved
    cout<<"Most Popular Room (%): Single (41.23%)"<<endl;
    cout<<"Highest peak (For 1 day): 27/28 Rooms Occupied"<<endl;
}

void realTimeAvailability() {
    cout << "Real-time availability display (current DB snapshot):\n";
    ifstream fin(DB_FILENAME);
    if (!fin) { cout << "DB missing.\n"; return; }
    string line;
    while (getline(fin, line)) {
        vector<string> f = splitFields(line, '|');
        if (f.size() >= 10) {
            string room = trim(f[0]);
            string name = trim(f[3]);
            cout << "Room " << room << " - " << ((name=="UNRESERVED" || name=="-") ? "Available" : "Occupied by " + name) << "\n";
        }
    }
    fin.close();
}

void reservationCancellation() {
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
    // Initialize DB if missing
    initializeDatabase();

    GuestInfo guest;
    // run single integrated flow
    introduction(guest);
    separationSystem(guest);
    shutdownSystem();
    return 0;
}
