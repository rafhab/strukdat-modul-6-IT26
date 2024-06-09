#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <fstream>

using namespace std;

class LaundryItem {
private:
    int id;
    string customerName;
    string serviceType;
    string deliveryType;
    float weight;
    float price;
    int deliveryDays;

public:
    LaundryItem() : id(0), customerName(""), serviceType(""), deliveryType(""), weight(0), price(0), deliveryDays(0) {}
    LaundryItem(int id, const string& customerName, const string& serviceType,
                const string& deliveryType, float weight, float price, int deliveryDays)
        : id(id), customerName(customerName), serviceType(serviceType),
          deliveryType(deliveryType), weight(weight), price(price), deliveryDays(deliveryDays) {}

    int getId() const { return id; }
    const string& getCustomerName() const { return customerName; }
    const string& getServiceType() const { return serviceType; }
    const string& getDeliveryType() const { return deliveryType; }
    float getWeight() const { return weight; }
    float getPrice() const { return price; }
    int getDeliveryDays() const { return deliveryDays; }

    void setId(int newId) { id = newId; }
    void setCustomerName(const string& newName) { customerName = newName; }
    void setServiceType(const string& newServiceType) { serviceType = newServiceType; }
    void setDeliveryType(const string& newDeliveryType) { deliveryType = newDeliveryType; }
    void setWeight(float newWeight) { weight = newWeight; }
    void setPrice(float newPrice) { price = newPrice; }
    void setDeliveryDays(int newDeliveryDays) { deliveryDays = newDeliveryDays; }
};

class LaundryService {
public:
    virtual float calculatePrice(float weight, const string& deliveryType) = 0;
};

class Cuci_Kering : public LaundryService {
public:
    float calculatePrice(float weight, const string& deliveryType) override {
        float price = 4000 * weight;
        if (deliveryType == "Kilat") {
            price += 1000;
        } else if (deliveryType == "Super Kilat") {
            price += 2000;
        }
        return price;
    }
};

class Cuci_Setrika : public LaundryService {
public:
    float calculatePrice(float weight, const string& deliveryType) override {
        float price = 5000 * weight;
        if (deliveryType == "Kilat") {
            price += 1000;
        } else if (deliveryType == "Super Kilat") {
            price += 2000;
        }
        return price;
    }
};

class Setrika : public LaundryService {
public:
    float calculatePrice(float weight, const string& deliveryType) override {
        float price = 3000 * weight;
        if (deliveryType == "Kilat") {
            price += 1000;
        } else if (deliveryType == "Super Kilat") {
            price += 2000;
        }
        return price;
    }
};

class LaundrySystem {
private:
    vector<LaundryItem> items;
    int nextId = 1; 
    struct Customer {
        string name;
        int totalOrders;
    };

    vector<Customer> customers;
    const string staffUsername = "staff";
    const string staffPassword = "password";

    string toLowerCase(const string& str) const {
        string lowerStr = str;
        transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
    }

    void displayHeader() const {
    cout << string(95, '-') << endl;
    cout << setw(5) << left << "ID"
         << setw(20) << left << "Customer"
         << setw(15) << left << "Service"
         << setw(15) << left << "Delivery"
         << setw(15) << left << "Weight(kg)"
         << setw(10) << left << "Price"
         << setw(15) << left << "Delivery Days"
         << endl;
    cout << string(95, '-') << endl;
}

    int findItemIndexById(int id) const {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i].getId() == id) {
                return i;
            }
        }
        return -1;
    }

    static bool compareById(const LaundryItem& a, const LaundryItem& b) {
        return a.getId() < b.getId();
    }

    static bool compareByName(const LaundryItem& a, const LaundryItem& b) {
        return a.getCustomerName() < b.getCustomerName();
    }

    float calculatePrice(LaundryService* service, float weight, const string& deliveryType) {
        return service->calculatePrice(weight, deliveryType);
    }

    int calculateDeliveryDays(const string& deliveryType) {
        if (deliveryType == "Reguler") {
            return 2;
        } else if (deliveryType == "Kilat") {
            return 1;
        } else if (deliveryType == "Super Kilat") {
            return 0;
        }
        return -1;
    }

    string getServiceCode(const string& serviceType) const {
        if (serviceType == typeid(Cuci_Kering).name()) {
            return "Cuci Kering";
        } else if (serviceType == typeid(Cuci_Setrika).name()) {
            return "Cuci Setrika";
        } else if (serviceType == typeid(Setrika).name()) {
            return "Setrika";
        }
        return "Unknown";
    }

public:

    void createItem(const string& customerName, LaundryService* service, const string& deliveryType, float weight) {
        float price = calculatePrice(service, weight, deliveryType);
        int deliveryDays = calculateDeliveryDays(deliveryType);

        auto it = find_if(customers.begin(), customers.end(), [&customerName](const Customer& c) {
            return c.name == customerName;
        });

        if (it != customers.end()) {
            it->totalOrders++;
            if (it->totalOrders % 3 == 0) {
                price *= 0.9;
                cout << "Congratulations! You've received a loyalty discount." << endl;
            }
        } else {
            customers.push_back({customerName, 1});
        }

        items.push_back(LaundryItem(nextId++, customerName, typeid(*service).name(), deliveryType, weight, price, deliveryDays));
    }

    void readItems() const {
        if (items.empty()) {
            cout << "No items in the system." << endl;
        } else {
            displayHeader(); 
            for (const auto& item : items) {
                cout << setw(5) << left << item.getId()
                     << setw(20) << left << item.getCustomerName()
                     << setw(15) << left << getServiceCode(item.getServiceType())
                     << setw(15) << left << item.getDeliveryType()
                     << setw(15) << left << item.getWeight()
                     << setw(10) << left << item.getPrice()
                     << setw(15) << left << item.getDeliveryDays()
                     << endl;
            }
        }
    }

    void updateItem(int id, const string& newCustomerName, LaundryService* newService, const string& newDeliveryType, float newWeight) {
        int index = findItemIndexById(id);
        if (index != -1) {
            if (!newCustomerName.empty())
                items[index].setCustomerName(newCustomerName);
            if (newService != nullptr)
                items[index].setServiceType(typeid(*newService).name());
            if (!newDeliveryType.empty())
                items[index].setDeliveryType(newDeliveryType);
            if (newWeight > 0)
                items[index].setWeight(newWeight);

            items[index].setPrice(calculatePrice(newService != nullptr ? newService : new Cuci_Kering(), newWeight, newDeliveryType));
            items[index].setDeliveryDays(calculateDeliveryDays(newDeliveryType));
        } else {
            cout << "Item with ID " << id << " not found." << endl;
        }
    }

    void deleteItem(int id) {
        int index = findItemIndexById(id);
        if (index != -1) {
            items.erase(items.begin() + index);
        } else {
            cout << "Item with ID " << id << " not found." << endl;
        }
    }

    void findItemById(int id) const {
        int index = findItemIndexById(id);
        if (index != -1) {
            displayHeader();
            const auto& item = items[index];
            cout << setw(5) << left << item.getId()
                 << setw(20) << left << item.getCustomerName()
                 << setw(15) << left << getServiceCode(item.getServiceType())
                 << setw(15) << left << item.getDeliveryType()
                 << setw(15) << left << item.getWeight()
                 << setw(10) << left << item.getPrice()
                 << setw(15) << left << item.getDeliveryDays()
                 << endl;
        } else {
            cout << "Item with ID " << id << " not found." << endl;
        }
    }

    void findItemByName(const string& name) const {
        string lowerName = toLowerCase(name);
        bool found = false;
        for (const auto& item : items) {
            if (toLowerCase(item.getCustomerName()).find(lowerName) != string::npos) {
                if (!found) {
                    displayHeader();
                    found = true;
                }
                cout << setw(5) << left << item.getId()
                     << setw(20) << left << item.getCustomerName()
                     << setw(15) << left << getServiceCode(item.getServiceType())
                     << setw(15) << left << item.getDeliveryType()
                     << setw(15) << left << item.getWeight()
                     << setw(10) << left << item.getPrice()
                     << setw(15) << left << item.getDeliveryDays()
                     << endl;
            }
        }
        if (!found) {
            cout << "No items found for customer name: " << name << endl;
        }
    }

    void findItemByService(const string& serviceType) const {
        bool found = false;
        for (const auto& item : items) {
            if (getServiceCode(item.getServiceType()) == serviceType) {
                if (!found) {
                    displayHeader();
                    found = true;
                }
                cout << setw(5) << left << item.getId()
                     << setw(20) << left << item.getCustomerName()
                     << setw(15) << left << getServiceCode(item.getServiceType())
                     << setw(15) << left << item.getDeliveryType()
                     << setw(15) << left << item.getWeight()
                     << setw(10) << left << item.getPrice()
                     << setw(15) << left << item.getDeliveryDays()
                     << endl;
            }
        }
        if (!found) {
            cout << "No items found for service type: " << serviceType << endl;
        }
    }

    void findItemByDelivery(const string& deliveryType) const {
        bool found = false;
        for (const auto& item : items) {
            if (item.getDeliveryType() == deliveryType) {
                if (!found) {
                    displayHeader();
                    found = true;
                }
                cout << setw(5) << left << item.getId()
                     << setw(20) << left << item.getCustomerName()
                     << setw(15) << left << getServiceCode(item.getServiceType())
                     << setw(15) << left << item.getDeliveryType()
                     << setw(15) << left << item.getWeight()
                     << setw(10) << left << item.getPrice()
                     << setw(15) << left << item.getDeliveryDays()
                     << endl;
            }
        }
        if (!found) {
            cout << "No items found for delivery type: " << deliveryType << endl;
        }
    }

    void sortByItemId() {
        sort(items.begin(), items.end(), compareById);
    }

    void sortByCustomerName() {
        sort(items.begin(), items.end(), compareByName);
    }

    void generateIncomeReport(const string& username, const string& password) const {
        if (username == staffUsername && password == staffPassword) {
            if (items.empty()) {
                cout << "No items in the system." << endl;
                return;
            }

            cout << string(90, '-') << endl;
            cout << "Income Report:" << endl;
            cout << string(90, '-') << endl;
            float totalIncome = 0;

            for (const auto& item : items) {
                totalIncome += item.getPrice();
            }

            cout << "Total Income: " << totalIncome << endl;
        } else {
            cout << "Invalid username or password. Access denied." << endl;
        }
    }

string itemToString(const LaundryItem& item) const {
    stringstream ss;
    ss << item.getId() << "," << item.getCustomerName() << "," << item.getServiceType() << ","
       << item.getDeliveryType() << "," << item.getWeight() << "," << item.getPrice() << ","
       << item.getDeliveryDays();
    return ss.str();
}

void saveToFile(const string& filename) const {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& item : items) {
            file << itemToString(item) << endl;
        }
        file.close();
        cout << "Data saved successfully to " << filename << endl;
    } else {
        cout << "Unable to open file " << filename << endl;
    }
}

void loadFromFile(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        items.clear(); // Menghapus data yang ada sebelumnya

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            int id, deliveryDays;
            string customerName, serviceType, deliveryType;
            float weight, price;

            // Parsing setiap bagian dari string
            string token;
            getline(ss, token, ',');
            id = stoi(token);
            getline(ss, customerName, ',');
            getline(ss, serviceType, ',');
            getline(ss, deliveryType, ',');
            getline(ss, token, ',');
            weight = stof(token);
            getline(ss, token, ',');
            price = stof(token);
            getline(ss, token);
            deliveryDays = stoi(token);

            items.push_back(LaundryItem(id, customerName, serviceType, deliveryType, weight, price, deliveryDays));
        }
        file.close();
        cout << "Data loaded successfully from " << filename << endl;
    } else {
        cout << "Unable to open file " << filename << endl;
    }
}


};

int main() {
    LaundrySystem system;
    system.loadFromFile("laundry_data.txt");

    int choice;
    string customerName, deliveryType, serviceType, usernameInput, passwordInput;
    float weight;
    int id;
    LaundryService* service = nullptr;
    do {
        cout << "\nLaundry System Menu:\n";
        cout << "1. Add Laundry Item\n";
        cout << "2. View Laundry Items\n";
        cout << "3. Edit Laundry Item\n";
        cout << "4. Delete Laundry Item\n";
        cout << "5. Find Laundry Item by ID\n";
        cout << "6. Find Laundry Item by Customer Name\n";
        cout << "7. Find Laundry Item by Service Type\n";
        cout << "8. Find Laundry Item by Delivery Type\n";
        cout << "9. Sort Laundry Items by ID\n";
        cout << "10. Sort Laundry Items by Customer Name\n";
        cout << "11. Generate Income Report\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter customer name: ";
                cin.ignore();
                getline(cin, customerName);
                cout << "Select service type:\n1. Cuci Kering\n2. Cuci Setrika\n3. Setrika\n";
                int serviceChoice;
                cout << "Enter service type (1/2/3): ";
                cin >> serviceChoice;
                switch(serviceChoice) {
                    case 1:
                        service = new Cuci_Kering();
                        break;
                    case 2:
                        service = new Cuci_Setrika();
                        break;
                    case 3:
                        service = new Setrika();
                        break;
                    default:
                        cout << "Invalid choice." << endl;
                        break;
                }
                cout << "Select delivery type:\n1. Reguler\n2. Kilat\n3. Super Kilat\n";
                int deliveryChoice;
                cout << "Enter delivery type (1/2/3): ";
                cin >> deliveryChoice;
                switch(deliveryChoice) {
                    case 1:
                        deliveryType = "Reguler";
                        break;
                    case 2:
                        deliveryType = "Kilat";
                        break;
                    case 3:
                        deliveryType = "Super Kilat";
                        break;
                    default:
                        cout << "Invalid delivery type. Please choose again." << endl;
                        continue;
                }
                cout << "Enter weight (kg): ";
                cin >> weight;
                system.createItem(customerName, service, deliveryType, weight);
                system.saveToFile("laundry_data.txt");
                break;
            case 2:
                system.readItems();
                break;
        case 3:
            cout << "Enter item ID to edit: ";
            cin >> id;
            cout << "Select item to edit:\n";
            cout << "1. Customer Name\n";
            cout << "2. Service Type\n";
            cout << "3. Delivery Type\n";
            cout << "4. Weight\n";
            cout << "Enter item to edit:\n";
            int editOption;
            cin >> editOption;

            switch (editOption) {
                case 1:
                    cout << "Enter new customer name: ";
                    cin.ignore();
                    getline(cin, customerName);
                    system.updateItem(id, customerName, nullptr, "", 0); 
                    system.saveToFile("laundry_data.txt");
                    break;
                case 2:
                    cout << "Select new service type:\n1. Cuci Kering\n2. Cuci Setrika\n3. Setrika\n";
                    cin >> serviceChoice;
                    switch(serviceChoice) {
                        case 1:
                            service = new Cuci_Kering();
                            break;
                        case 2:
                            service = new Cuci_Setrika();
                            break;
                        case 3:
                            service = new Setrika();
                            break;
                        default:
                            cout << "Invalid service type. Please choose again." << endl;
                            continue;
                    }
                    system.updateItem(id, "", service, "", 0); 
                    system.saveToFile("laundry_data.txt");
                    break;
                case 3:
                    cout << "Select new delivery type:\n1. Reguler\n2. Kilat\n3. Super Kilat\n";
                    cin >> deliveryChoice;
                    switch(deliveryChoice) {
                        case 1:
                            deliveryType = "Reguler";
                            break;
                        case 2:
                            deliveryType = "Kilat";
                            break;
                        case 3:
                            deliveryType = "Super Kilat";
                            break;
                        default:
                            cout << "Invalid delivery type. Please choose again." << endl;
                            continue;
                    }
                    system.updateItem(id, "", nullptr, deliveryType, 0);
                    system.saveToFile("laundry_data.txt");
                    break;
                case 4:
                    cout << "Enter new weight in kg: ";
                    cin >> weight;
                    system.updateItem(id, "", nullptr, "", weight);
                    system.saveToFile("laundry_data.txt");
                    break;
                default:
                    cout << "Invalid option. Please choose again." << endl;
                    continue;
            }
            break;
            case 4:
                cout << "Enter ID of the item to delete: ";
                cin >> id;
                system.deleteItem(id);
                system.saveToFile("laundry_data.txt");
                break;
            case 5:
                cout << "Enter ID of the item to find: ";
                cin >> id;
                system.findItemById(id);
                break;
            case 6:
                cout << "Enter customer name to find: ";
                cin.ignore();
                getline(cin, customerName);
                system.findItemByName(customerName);
                break;
            case 7:
                cout << "Enter service type to find (Cuci Kering/Cuci Setrika/Setrika): ";
                cin.ignore();
                getline(cin, serviceType);
                system.findItemByService(serviceType);
                break;
            case 8:
                cout << "Enter delivery type to find (Reguler/Kilat/Super Kilat): ";
                cin.ignore();
                getline(cin, deliveryType);
                system.findItemByDelivery(deliveryType);
                break;
            case 9:
                system.sortByItemId();
                break;
            case 10:
                system.sortByCustomerName();
                break;
            case 11:
                cout << "Enter staff username: ";
                cin >> usernameInput;
                cout << "Enter staff password: ";
                cin >> passwordInput;
                system.generateIncomeReport(usernameInput, passwordInput);
                break;
            case 0:
                cout << "Exiting system." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 0);
    return 0;
}
