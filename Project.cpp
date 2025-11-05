#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <typeinfo>
#include <cctype>
#include <algorithm>
#include <limits>


using namespace std;


void listRooms();
void listAvailable();
void bookRoom();
void cancelBook();
void viewBooked();


int main () {
   int choice;
 cout<<"Group:"<<right<<setw(9)<<"4"<<endl<<left<<setw(39)<<"Members: Villacorta, Joshua James"<<endl<<setw(9)<<""<<"Cabigao, Miguel"<<endl<<setw(9)<<""<<"Escarnuela, Vincent"<<endl<<setw(9)<<""<<"Medalla, Vincent"<<endl<<setw(9)<<""<<"Reponcion, Eric James"<<endl;
   cout<<"\nWelcome to Lumina Lodging"<<endl;
   while (true) {
       cout<<"Enter (1) if you want to display the catalog\n";
       cout<<"Enter (2) if you want to display available rooms\n";
       cout<<"Enter (3) if you want to book a room\n";
       cout<<"Enter (4) if you want to cancel your booking\n";
       cout<<"Enter (5) if you want to view your booked room/s\n";
       cout<<"Enter (6) if you want to exit the program\n";
     
      cout<<"Choice: ";
       cin>>choice;
       switch (choice) {
           case 1: listRooms();
               break;
           case 2:
               listAvailable();
               break;
           case 3: bookRoom();
               break;
           case 4: cancelBook();
               break;
           case 5: viewBooked();
               break;
           case 6: cout << "Thank you for choosing Lumina Lodging!" << endl;
           return 0;
         default: cout<<"Invalid input";
             return 1;
       }
   }
}


void listRooms(){
 string roomNum, roomType, bedNum, dailyRate, status;
 ifstream RoomsFile;
 RoomsFile.open("rooms_file.txt", ios::out);
 if (!RoomsFile) {
   cout<<"No file was read";
 }
 else {
   string line;
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
 cout<<endl;
 return;
}


void listAvailable(){
 string roomNum, roomType, bedNum, dailyRate, status;
 ifstream RoomsFile;
 RoomsFile.open("rooms_file.txt", ios::out);
 if (!RoomsFile) {
   cout<<"No file was read"<<endl;
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
 cout<<endl;
 return;


}


void bookRoom(){
 int a=0;
       string get_roomNum, roomNum, roomType, bedNum, dailyRate, status;
       cin.ignore();
       cout<<"Enter the room number you would like to book: ";
       getline(cin,get_roomNum);
   
       ifstream roomsFile;
       roomsFile.open("rooms_file.txt", ios::in);
       ofstream bookRoomFile;
       ofstream roomsFileCopy;
       roomsFileCopy.open("roomsFileCopy.txt", ios::app);
       bookRoomFile.open("usrBookedRooms.txt", ios::app);
   
       string line;
       while(getline(roomsFile, line)) {
           stringstream ss(line);
           getline(ss, roomNum, ',');
           getline(ss, roomType, ',');
           getline(ss, bedNum, ',');
           getline(ss, dailyRate, ',');
           getline(ss, status);
           if (roomNum==get_roomNum) {
               if (status!="Occupied") {
                   bookRoomFile <<roomNum <<"," <<roomType <<"," <<bedNum <<"," <<dailyRate <<endl;   
                   roomsFileCopy <<roomNum <<"," <<roomType <<"," <<bedNum <<"," <<dailyRate <<',' << "Occupied"  <<endl;
               } else {
                   cout<<"The room is already occupied."<<endl<<endl;
                   return;
               }
           
           } else {
               roomsFileCopy <<roomNum <<"," <<roomType <<"," <<bedNum <<"," <<dailyRate <<',' << status  <<endl;
           }
       
       }
       
       cout<<"Booked room successfully"<<endl;
       roomsFile.close();
       roomsFileCopy.close();
       remove("rooms_file.txt");
       rename("roomsFileCopy.txt", "rooms_file.txt");
 return;
}


void cancelBook(){
 string roomNum, roomType, bedNum, dailyRate, get_roomNum, line, status;
   ifstream usrBookedIn;
   ifstream roomsFile;
   roomsFile.open("rooms_file.txt", ios::in);
   usrBookedIn.open("borrowed_books.txt", ios::in);
   ofstream usrBookedOut;
   ofstream copyRoomsFile;
   usrBookedOut.open("usrBookedOut.txt", ios::app);
   copyRoomsFile.open("copyRoomsFile.txt", ios::app);
  
   cout<<"Enter the room number you would like to cancel: ";
   cin>>get_roomNum;
  
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
       copyRoomsFile.close();
       usrBookedOut.close();
       usrBookedIn.close();
       roomsFile.close();
       remove("rooms_file.txt");
       remove("usrBookedRooms.txt");
       rename("usrBookedOut.txt", "usrBookedRooms.txt");
       rename("copyRoomsFile.txt", "rooms_file.txt");
 return;
}


void viewBooked(){
 string roomNum, roomType, bedNum, dailyRate, status;
 ifstream usrBooked;
 usrBooked.open("usrBookedRooms.txt", ios::out);
 if (!usrBooked) {
   cout<<"No file was read";
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
