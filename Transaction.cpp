#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;


struct RoomBooking {
    int roomNum;
    string roomType;
    int roomPrice;
};


RoomBooking getRoomInfo(int roomNum) {
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


int main() {
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
        return 1;
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


            RoomBooking room = getRoomInfo(roomNum);


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
            cin.ignore();
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


    return 0;
}
