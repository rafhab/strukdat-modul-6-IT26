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

    int newId = (items.empty() ? 1 : items.back().getId() + 1); 
    items.push_back(LaundryItem(newId, customerName, typeid(*service).name(), deliveryType, weight, price, deliveryDays));
}
