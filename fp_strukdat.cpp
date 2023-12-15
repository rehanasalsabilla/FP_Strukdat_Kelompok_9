#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <limits>
#include <unordered_map>

class Item {
public:
    virtual ~Item() = default;

    virtual std::string GetName() const = 0;
    virtual void DisplayInfo() const = 0;
};

class Product;

class Customer : public Item {
public:
    Customer(std::string name, std::string destinationCity)
        : name(name), destinationCity(destinationCity) {}

    void AddToCart(Product* product);

    void Checkout(const std::map<std::string, Product>& products);

    void DisplayPersonalStatus() const;

    void DisplayInfo() const override;

    std::string GetName() const override {
        return name;
    }

    std::vector<Product*>& GetCart() {
        return cart;
    }

    Customer() = default;

    ~Customer() override = default;

private:
    std::string name;
    std::string destinationCity; // New member variable
    std::vector<Product*> cart;
    std::vector<std::vector<Product*>> orderHistory;
    std::vector<std::string> status;
};

class Product : public Item {
public:
    Product(std::string name, double price, int stock) : name(name), price(price), stock(stock) {}

    // Add a default constructor
    Product() = default;

    ~Product() override = default;

    void DisplayInfo() const override {
        std::cout << "Product: " << GetName() << "\n- Price: $" << GetPrice() << "\n- Stock: " << GetStock() << "\n";
    }

    std::string GetName() const override {
        return name;
    }

    double GetPrice() const {
        return price;
    }

    int GetStock() const {
        return stock;
    }

    void ReduceStock() {
        stock--;
    }

    void AddCustomer(Customer* customer) {
        customers.push_back(customer);
    }

    std::vector<Customer*>& GetCustomers() {
        return customers;
    }

private:
    std::string name;
    double price;
    int stock;
    std::vector<Customer*> customers;
};

class DistanceCalculator {
public:
 DistanceCalculator() {
        // Initialize the adjacency list for the shipping distance and cost
        shippingAdjacencyList["surabaya"] = {{"mojokerto", 5}, {"sidoarjo", 3}, {"lamongan", 7}};
        shippingAdjacencyList["mojokerto"] = {{"surabaya", 5}, {"nganjuk", 15}, {"kediri", 10}};
        shippingAdjacencyList["sidoarjo"] = {{"surabaya", 3}, {"malang", 13}, {"pasuruan", 10}};
        shippingAdjacencyList["lamongan"] = {{"surabaya", 7}, {"tuban", 5}};
        // Add more locations as needed

    }

int CalculateDistance(const std::string& source, const std::string& destination) {
    std::unordered_map<std::string, int> distance;
    std::queue<std::string> bfsQueue;

    bfsQueue.push(source);
    distance[source] = 0;

    while (!bfsQueue.empty()) {
        std::string currentLocation = bfsQueue.front();
        bfsQueue.pop();

        for (const auto& neighborPair : shippingAdjacencyList[currentLocation]) {
            const std::string& neighbor = neighborPair.first;
            if (distance.find(neighbor) == distance.end()) {
                distance[neighbor] = distance[currentLocation] + 1;
                bfsQueue.push(neighbor);
            }
        }
    }

    if (distance.find(destination) != distance.end()) {
        return distance[destination];
    } else {
        return std::numeric_limits<int>::max();
    }
}

std::vector<std::string> GetShippingRoute(const std::string& source, const std::string& destination) {
    std::vector<std::string> route;
    std::unordered_map<std::string, std::string> previousLocation;
    std::queue<std::string> bfsQueue;

    bfsQueue.push(source);
    previousLocation[source] = "";

    while (!bfsQueue.empty()) {
        std::string currentLocation = bfsQueue.front();
        bfsQueue.pop();

        for (const auto& neighborPair : shippingAdjacencyList[currentLocation]) {
            const std::string& neighbor = neighborPair.first;
            if (previousLocation.find(neighbor) == previousLocation.end()) {
                previousLocation[neighbor] = currentLocation;
                bfsQueue.push(neighbor);
            }
        }
    }

    // Reconstruct the route
    std::string current = destination;
    while (!current.empty()) {
        route.insert(route.begin(), current);
        current = previousLocation[current];
    }

    return route;
}

        const std::map<std::string, std::vector<std::pair<std::string, int>>>& GetShippingAdjacencyList() const {
        return shippingAdjacencyList;
    }

private:
    std::map<std::string, std::vector<std::pair<std::string, int>>> shippingAdjacencyList;
};

void Customer::AddToCart(Product* product) {
    cart.push_back(product);
    status.push_back("Added " + product->GetName() + " to cart.");
}

void Customer::Checkout(const std::map<std::string, Product>&) {
    double total = 0;
    std::cout << "\n=== Checkout ===\n";

    for (auto& item : cart) {
        item->DisplayInfo();
        total += item->GetPrice();
        item->ReduceStock(); // reduce stock
    }

    std::cout << "\nTotal price: $" << total << "\nOrder shipped to: " << destinationCity << "\n";

    
    DistanceCalculator distanceCalculator;
    std::string sourceLocation = "surabaya";  
    int shippingDistance = distanceCalculator.CalculateDistance(sourceLocation, destinationCity);

    std::cout << "Shipping distance from " << sourceLocation << " to " << destinationCity << ": " << shippingDistance << " locations\n";


    orderHistory.push_back(cart);
    cart.clear(); 
    status.push_back("Checked out. Total: $" + std::to_string(total));
}

void Customer::DisplayPersonalStatus() const {
    std::cout << "\n=== Personal Status ===\n";
    for (const auto& s : status) {
        std::cout << "- " << s << "\n";
    }

    double cartTotal = 0;
    for (const auto& item : cart) {
        cartTotal += item->GetPrice();
    }
    std::cout << "Cart Total: $" << cartTotal << "\n";
}

void Customer::DisplayInfo() const {
    std::cout << "\n=== Customer Information ===\n";
    std::cout << "Customer: " << GetName() << "\n- Destination City: " << destinationCity << "\n";

    std::cout << "- Order History:\n";
    for (size_t i = 0; i < orderHistory.size(); ++i) {
        std::cout << "  - Order #" << i + 1 << ": ";
        for (const auto& item : orderHistory[i]) {
            std::cout << item->GetName() << ", ";
        }
        std::cout << "\n";
    }
}

void DisplayProductList(const std::map<std::string, Product>& products) {
    std::cout << "\n=== Product List ===\n";
    for (const auto& product : products) {
        product.second.DisplayInfo();
    }
}

int main() {
    std::map<std::string, Product> products;
    std::map<std::string, Customer> customers;

    // Creating instances of Product
    products.emplace("Laptop", Product("Laptop", 1000, 10));
    products.emplace("Handphone", Product("Handphone", 500, 20));
    products.emplace("T-Shirt", Product("T-Shirt", 20, 50));
    products.emplace("Celana", Product("Celana", 30, 40));
    products.emplace("Topi", Product("Topi", 10, 30));


    std::string customerName, destinationCity;


    while (true) {
        std::cout << "Enter your name: ";
        std::getline(std::cin, customerName);
        std::cout << "Enter destination city for shipping: ";
        std::getline(std::cin, destinationCity);

       
        std::vector<std::string> validCities = {"mojokerto", "sidoarjo", "lamongan", "nganjuk", "kediri", "malang", "pasuruan", "tuban"};
        if (std::find(validCities.begin(), validCities.end(), destinationCity) != validCities.end()) {
            break; 
        } else {
            std::cout << "Invalid destination city. Please choose from the following cities: mojokerto, sidoarjo, lamongan, nganjuk, kediri, malang, pasuruan, tuban\n";
        }
    }

    customers.emplace(customerName, Customer(customerName, destinationCity));

    for (auto& product : products) {
        product.second.AddCustomer(&customers[customerName]);
    }

    // Main menu
    while (true) {
        std::cout << "\n=== Main Menu ===\n";
        std::cout << "1. Display Product\n";
        std::cout << "2. Add to Cart\n";
        std::cout << "3. Checkout\n";
        std::cout << "4. Cart Data\n";
        std::cout << "5. Order History\n";
        std::cout << "6. Display Shipping Route\n";
        std::cout << "7. Exit\n";
        int choice;
        std::cout << "Enter your choice (1-7): ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                DisplayProductList(products);
                break;
            case 2: {
                std::string productName;
                std::cout << "Enter product name to add to cart: ";
                std::getline(std::cin, productName);
                if (products.find(productName) != products.end()) {
                    customers[customerName].AddToCart(&products[productName]);
                } else {
                    std::cout << "Invalid product name. Please try again.\n";
                }
                break;
            }
            case 3:
                customers[customerName].Checkout(products);
                break;
            case 4:
                customers[customerName].DisplayPersonalStatus();
                break;
            case 5:
                customers[customerName].DisplayInfo();
                break;
           
            case 6:
            {
                DistanceCalculator distanceCalculator;
                std::string sourceLocation = "surabaya";  
                std::vector<std::string> shippingRoute = distanceCalculator.GetShippingRoute(sourceLocation, destinationCity);
                std::cout << "\n=== Shipping Route ===\n";
                std::cout << "From " << sourceLocation << " to " << destinationCity << ":\n";

                double totalCost = 0;
                std::unordered_map<std::string, int> edgeCost;
                for (size_t i = 0; i < shippingRoute.size(); ++i) {
                    std::string currentLocation = shippingRoute[i];
                    std::string nextLocation = (i + 1 < shippingRoute.size()) ? shippingRoute[i + 1] : "";
                    int cost = 0;

                    for (const auto& neighborPair : distanceCalculator.GetShippingAdjacencyList().at(currentLocation)) {
                        if (neighborPair.first == nextLocation) {
                            cost = neighborPair.second;
                            break;
                        }
                    }

                    std::cout << (i > 0 ? " -> " : "") << currentLocation;
                    if (!nextLocation.empty()) {
                        totalCost += cost;
                        std::cout << "(" << cost << "k)";
                    }
                }
                std::cout << "\nTotal Cost: " << totalCost << "k" << "\n";
            }
            break;
                case 7:
                    std::cout << "Exiting program...\n";
                    return 0;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
                    break;
            }
        }
    return 0;
}
