#include <iostream>
#include <fstream>
#include <sstream>
#include<string>

using namespace std;

const int MAX_CARS = 100;
const int MAX_RENTERS = 100;
const int MAX_RESERVATIONS = 10;

struct Reservation {
    int reservationNumber; 
    string renterName;
    string carModel;
    string startDate; 
    int numDays; 
    double reservationCost; 
};

struct Car {
    string model;
    double rentalCost;
    int availableCars;
};
struct Renter {
    string name;
    Reservation reservations[MAX_RESERVATIONS];
    int numReservations = 0;
};

void listAvailableCars(const Car* inventory, int numCars) {
    cout << "Available Cars:" << endl;
    for (int i = 0; i < numCars; ++i) {
        cout << "Model: " << inventory[i].model << ", Rental Cost: " << inventory[i].rentalCost << ", Available: " << inventory[i].availableCars << endl;
    }
}

bool rentCar(Car* inventory, int numCars, const string& carModel) {
    for (int i = 0; i < numCars; ++i) {
        if (inventory[i].model == carModel && inventory[i].availableCars > 0) {
            inventory[i].availableCars--;
            return true;
        }
    }
    return false;
}

double calculateRentalCost(const Car* inventory, int numCars, const string& carModel, int numDays) {
    for (int i = 0; i < numCars; ++i) {
        if (inventory[i].model == carModel) {
            return inventory[i].rentalCost * numDays;
        }
    }
    return 0.0;
}


void addCar(Car* inventory, int& numCars, const string& newModel, double newRentalCost, int newNumAvailable) {
    if (numCars >= MAX_CARS) {
       cout << "Car inventory is full. Cannot add more cars." << endl;
        return;
    }

    Car newCar;
    newCar.model = newModel;
    newCar.rentalCost = newRentalCost;
    newCar.availableCars = newNumAvailable;

    inventory[numCars++] = newCar;
}


void removeCar(Car* inventory, int& numCars, const string& targetModel) {
    for (int i = 0; i < numCars; ++i) {
        if (inventory[i].model == targetModel) {
            // Move the last car to the position of the removed car
            inventory[i] = inventory[numCars - 1];
            numCars--;
            cout << "Car " << targetModel << " removed from inventory." << endl;
            return;
        }
    }

    cout << "Car " << targetModel << " not found in inventory." << endl;
}

void listCars(const Car* cars, int numCars) { //3rd
    for (int i = 0; i < numCars; ++i) {
       cout << "Model: " << cars[i].model << ", Rental Cost: $" << cars[i].rentalCost << ", Available Cars: " << cars[i].availableCars << endl;
    }
}

int loadCarsFromFile(const string& CarInv, Car* cars) {
    ifstream file(CarInv);
    if (!file.is_open()) { //error case
        cout << "Error: Could not open file " << CarInv << endl;
        return 0;
    }

    int numCars = 0;
    string line;
    while (getline(file, line) /* cond 1*/ && numCars < MAX_CARS /*cond 2 3omro ma hybooz */) {
        istringstream iss(line); //turns data to strings

        if (!(iss >> cars[numCars].model >> cars[numCars].rentalCost >> cars[numCars].availableCars)) { // invalide value given
            cout << "Error: Invalid line in file: " << line << endl;
            continue;
        }

        numCars++;
    }

    file.close();
    return numCars;
}


bool addRenter(Renter* renters, int& numRenters, const string& newName, const Reservation* newReservations, int numNewReservations) {
    if (numRenters >= MAX_RENTERS) {
        cout << "Renter database is full. Cannot add more renters." << endl;
        return false;
    }

    if (numNewReservations > MAX_RESERVATIONS) {
        cout << "Exceeded maximum number of reservations. Only " << MAX_RESERVATIONS << " reservations will be added." << endl;
        numNewReservations = MAX_RESERVATIONS;
    }

    Renter newRenter;
    newRenter.name = newName;
    newRenter.numReservations = numNewReservations;

    for (int i = 0; i < numNewReservations; ++i) {
        newRenter.reservations[i] = newReservations[i];
    }

    renters[numRenters++] = newRenter;
    return true;
}

void listRenterInfo(const Renter* renters, int numRenters) {
    for (int i = 0; i < numRenters; ++i) {
        cout << "Renter: " << renters[i].name << endl;
        cout << "Reservations:" << endl;
        for (int j = 0; j < renters[i].numReservations; ++j) {
            cout << "  Reservation Number: " << renters[i].reservations[j].reservationNumber << ", Car Model: " << renters[i].reservations[j].carModel << endl;
            
        }
    }
}

void saveRenterDataToFile(const Renter* renters, int numRenters, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    for (int i = 0; i < numRenters; ++i) {
        file << renters[i].name << endl;
        for (int j = 0; j < renters[i].numReservations; ++j) {
            file << renters[i].reservations[j].reservationNumber << " " << renters[i].reservations[j].carModel << endl;
            // Write other reservation details if available
        }
        file << endl; // Separator between renters
    }

    file.close();
}

void loadRenterDataFromFile(Renter* renters, int& numRenters, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    numRenters = 0;
    Renter currentRenter;
    string line;
    while (getline(file, line)) {
        if (line.empty()) {
            if (currentRenter.name != "") {
                renters[numRenters++] = currentRenter;
                currentRenter = Renter();
            }
        } else if (currentRenter.name == "") {
            currentRenter.name = line;
        } else {
            istringstream iss(line);
            Reservation reservation;
            iss >> reservation.reservationNumber >> reservation.carModel;
            currentRenter.reservations[currentRenter.numReservations++] = reservation;
        }
    }

    if (currentRenter.name != "") {
        renters[numRenters++] = currentRenter;
    }

    file.close();
}
int calculateTotalRentedCarsPerDay(const Reservation* reservations, int numReservations, const std::string& date) {
    int count = 0;
    for (int i = 0; i < numReservations; ++i) {
        if (reservations[i].startDate == date) {
            count++;
        }
    }
    return count;
}

string findFavoriteRentedCar(const Reservation* reservations, int numReservations) {
    // Implement logic to find the car model with the highest frequency in reservations
    // and return its model name.
}

double calculateAverageRentalTime(const Reservation* reservations, int numReservations) {
    // Implement logic to calculate the average rental time (number of days) across reservations.
}

void generateRenterSummary(const Renter* renters, int numRenters, const string& renterName) {
    for (int i = 0; i < numRenters; ++i) {
        if (renters[i].name == renterName) {
            cout << "Renter: " << renters[i].name << endl;
            for (int j = 0; j < renters[i].numReservations; ++j) {
                cout << "Reservation Number: " << renters[i].reservations[j].reservationNumber << ", Car Model: " << renters[i].reservations[j].carModel << ", Start Date: " << renters[i].reservations[j].startDate << ", Number of Days: " << renters[i].reservations[j].numDays << ", Reservation Cost: " << renters[i].reservations[j].reservationCost << std::endl;
            }
            return;
        }
    }
   cout << "Renter not found: " << renterName << endl;
}


int main() {
    Car inventory[MAX_CARS];
    Renter renters[MAX_RENTERS];
    int numRenters = 0;
    int numCars = loadCarsFromFile("CarInv.txt", inventory);

    listCars(inventory, numCars);
    loadRenterDataFromFile(renters, numRenters, "renter_data.txt");

    Reservation reservations[] = {{1, "Sedan"}, {2, "SUV"}};

    addRenter(renters, numRenters, "Alice Johnson", reservations, 2);

    saveRenterDataToFile(renters, numRenters, "renter_data.txt");

    listRenterInfo(renters, numRenters);
    return 0;
}
