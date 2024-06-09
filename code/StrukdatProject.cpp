#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>
#include <algorithm>
#include <iomanip>

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
    LaundryItem(int id, const string& customerName, const string& serviceType,
                const string& deliveryType, float weight, float price, int deliveryDays)
        : id(id), customerName(customerName), serviceType(serviceType),
          deliveryType(deliveryType), weight(weight), price(price), deliveryDays(deliveryDays) {}

    int getId() const { return id; }
    string getCustomerName() const { return customerName; }
    void setCustomerName(const string& name) { customerName = name; }

    string getServiceType() const { return serviceType; }
    void setServiceType(const string& service) { serviceType = service; }

    string getDeliveryType() const { return deliveryType; }
    void setDeliveryType(const string& delivery) { deliveryType = delivery; }

    float getWeight() const { return weight; }
    void setWeight(float w) { weight = w; }

    float getPrice() const { return price; }
    void setPrice(float p) { price = p; }

    int getDeliveryDays() const { return deliveryDays; }
    void setDeliveryDays(int days) { deliveryDays = days; }
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
    int nextId;
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
    LaundrySystem() : nextId(1) {}

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
        cout << setw(5) << left << "ID"
             << setw(20) << left << "Customer"
             << setw(15) << left << "Service"
             << setw(15) << left << "Delivery"
             << setw(15) << left << "Weight(kg)"
             << setw(10) << left << "Price"
             << setw(15) << left << "Delivery Days"
             << endl;

        cout << string(90, '-') << endl;

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
            items[index].setCustomerName(newCustomerName);
            items[index].setServiceType(typeid(*newService).name());
            items[index].setDeliveryType(newDeliveryType);
            items[index].setWeight(newWeight);
            items[index].setPrice(calculatePrice(newService, newWeight, newDeliveryType));
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
            cout << setw(5) << left << "ID"
                 << setw(20) << left << "Customer"
                 << setw(15) << left << "Service"
                 << setw(15) << left << "Delivery"
                 << setw(15) << left << "Weight(kg)"
                 << setw(10) << left << "Price"
                 << setw(15) << left << "Delivery Days"
                 << endl;
            cout << string(90, '-') << endl;
            cout << setw(5) << left << items[index].getId()
                 << setw(20) << left << items[index].getCustomerName()
                 << setw(15) << left << getServiceCode(items[index].getServiceType())
                 << setw(15) << left << items[index].getDeliveryType()
                 << setw(15) << left << items[index].getWeight()
                 << setw(10) << left << items[index].getPrice()
                 << setw(15) << left << items[index].getDeliveryDays()
                 << endl;
        } else {
            cout << "Item with ID " << id << " not found." << endl;
        }
    }

    void findItemByName(const string& name) const {
        string lowerName = toLowerCase(name);

        vector<int> indices;
        for (size_t i = 0; i < items.size(); ++i) {
            if (toLowerCase(items[i].getCustomerName()) == lowerName) {
                indices.push_back(i);
            }
        }

        if (indices.empty()) {
            cout << "No items found for customer name \"" << name << "\"." << endl;
        } else {
            cout << setw(5) << left << "ID"
                 << setw(20) << left << "Customer"
                 << setw(15) << left << "Service"
                 << setw(15) << left << "Delivery"
                 << setw(15) << left << "Weight(kg)"
                 << setw(10) << left << "Price"
                 << setw(15) << left << "Delivery Days"
                 << endl;
            cout << string(90, '-') << endl;

            for (int index : indices) {
                cout << setw(5) << left << items[index].getId()
                     << setw(20) << left << items[index].getCustomerName()
                     << setw(15) << left << getServiceCode(items[index].getServiceType())
                     << setw(15) << left << items[index].getDeliveryType()
                     << setw(15) << left << items[index].getWeight()
                     << setw(10) << left << items[index].getPrice()
                     << setw(15) << left << items[index].getDeliveryDays()
                     << endl;
            }
        }
    }

    void findItemByService(const string& serviceType) const {
        string lowerServiceType = toLowerCase(serviceType);

        vector<int> indices;
        for (size_t i = 0; i < items.size(); ++i) {
            if (toLowerCase(getServiceCode(items[i].getServiceType())) == lowerServiceType) {
                indices.push_back(i);
            }
        }

        if (indices.empty()) {
            cout << "No items found for service type \"" << serviceType << "\"." << endl;
        } else {
            cout << setw(5) << left << "ID"
                 << setw(20) << left << "Customer"
                 << setw(15) << left << "Service"
                 << setw(15) << left << "Delivery"
                 << setw(15) << left << "Weight(kg)"
                 << setw(10) << left << "Price"
                 << setw(15) << left << "Delivery Days"
                 << endl;
            cout << string(90, '-') << endl;

            for (int index : indices) {
                cout << setw(5) << left << items[index].getId()
                     << setw(20) << left << items[index].getCustomerName()
                     << setw(15) << left << getServiceCode(items[index].getServiceType())
                     << setw(15) << left << items[index].getDeliveryType()
                     << setw(15) << left << items[index].getWeight()
                     << setw(10) << left << items[index].getPrice()
                     << setw(15) << left << items[index].getDeliveryDays()
                     << endl;
            }
        }
    }

    void findItemByDelivery(const string& deliveryType) const {
        string lowerDeliveryType = toLowerCase(deliveryType);

        vector<int> indices;
        for (size_t i = 0; i < items.size(); ++i) {
            if (toLowerCase(items[i].getDeliveryType()) == lowerDeliveryType) {
                indices.push_back(i);
            }
        }

        if (indices.empty()) {
            cout << "No items found for delivery type \"" << deliveryType << "\"." << endl;
        } else {
            cout << setw(5) << left << "ID"
                 << setw(20) << left << "Customer"
                 << setw(15) << left << "Service"
                 << setw(15) << left << "Delivery"
                 << setw(15) << left << "Weight(kg)"
                 << setw(10) << left << "Price"
                 << setw(15) << left << "Delivery Days"
                 << endl;
            cout << string(90, '-') << endl;

            for (int index : indices) {
                cout << setw(5) << left << items[index].getId()
                     << setw(20) << left << items[index].getCustomerName()
                     << setw(15) << left << getServiceCode(items[index].getServiceType())
                     << setw(15) << left << items[index].getDeliveryType()
                     << setw(15) << left << items[index].getWeight()
                     << setw(10) << left << items[index].getPrice()
                     << setw(15) << left << items[index].getDeliveryDays()
                     << endl;
            }
        }
    }

    void sortByItemId() {
        sort(items.begin(), items.end(), compareById);
    }

    void sortByCustomerName() {
        sort(items.begin(), items.end(), compareByName);
    }

    bool authenticateStaff(const string& username, const string& password) const {
        return username == staffUsername && password == staffPassword;
    }

    float calculateTotalIncome() const {
        float totalIncome = 0;
        for (const auto& item : items) {
            totalIncome += item.getPrice();
        }
        return totalIncome;
    }

    void generateIncomeReport(const string& username, const string& password) const {
        if (authenticateStaff(username, password)) {
            cout << "\nIncome Report:\n";
            cout << "-----------------------------\n";
            cout << "Total Income: $" << calculateTotalIncome() << endl;
            cout << "-----------------------------\n";
        } else {
            cout << "Invalid username or password. Access denied." << endl;
        }
    }
};

int main() {
    LaundrySystem system;
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
                cout << "Select delivery type:\n1. Reguler\n2. Kilat\n3. Super Kilat\n";
                int deliveryChoice;
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
                cout << "Enter weight in kg: ";
                cin >> weight;
                system.createItem(customerName, service, deliveryType, weight);
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
                int editOption;
                cin >> editOption;
            
                switch (editOption) {
                    case 1:
                        cout << "Enter new customer name: ";
                        cin.ignore();
                        getline(cin, customerName);
                        system.updateItem(id, customerName, nullptr, "", 0);
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
                        break;
                    case 4:
                        cout << "Enter new weight in kg: ";
                        cin >> weight;
                        system.updateItem(id, "", nullptr, "", weight); 
                        break;
                    default:
                        cout << "Invalid option. Please choose again." << endl;
                        continue;
                }
                break;
            case 4:
                cout << "Enter item ID to delete: ";
                cin >> id;
                system.deleteItem(id);
                break;
            case 5:
                cout << "Enter item ID to find: ";
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
                cout << "Laundry items sorted by ID." << endl;
                break;
            case 10:
                system.sortByCustomerName();
                cout << "Laundry items sorted by Customer Name." << endl;
                break;
            case 11:
                cout << "Enter staff username: ";
                cin >> usernameInput;
                cout << "Enter staff password: ";
                cin >> passwordInput;
                system.generateIncomeReport(usernameInput, passwordInput);
                break;
            case 0:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please choose again." << endl;
                break;
        }

    } while (choice != 0);

    // Clean up dynamically allocated memory
    delete service;

    return 0;
}
