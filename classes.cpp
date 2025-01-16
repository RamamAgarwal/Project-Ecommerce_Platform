#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <ctime>

using namespace std;

class Category {
private:
    string name;
    string description;

public:
    Category(string name) : name(name) {}

    string getName() {
        return name;
    }
};

class Product {
private:
    static int idCounter;
    int id;
    string name;
    string description;
    double price;
    double ratings;
    Category category;
    int availableCount;
    Seller* seller;

public:

    Product(string name, string description, double price, Category category, int availableCount)
            : name(name), description(description), price(price), category(category), availableCount(availableCount) {
        idCounter++;
        id = idCounter;
    }

    int getId() {
        return id;
    }

    string getName() {
        return name;
    }

    string getDescription() {
        return description;
    }

    double getPrice() {
        return price;
    }

    Seller* getSeller() {
        return seller;
    }

    double getRatings() {
        return ratings;
    }

    Category getCategory() {
        return category;
    }

    int getAvailableCount() {
        return availableCount;
    }

    void setSeller(Seller* seller) {
        this->seller = seller;
    }

    void setAvailableCount(int availableCount) {
        this->availableCount = availableCount;
    }
};

int Product::idCounter = 0;

class User {
protected:
    int id;
    string name;
    string address;
    string username;
    string password;
    string phone;

public:
    User(string name, string phone) : name(name), phone(phone) {}

    string getName() {
        return name;
    }

    string getAddress() {
        return address;
    }

    string getPhone() {
        return phone;
    }
};

class Seller : public User {
private:
    vector<Product*> products;

public:
    Seller(string name, string phone) : User(name, phone) {}

    void registerProduct(ProductsCatalog* productsCatalog, Product* product) {
        product->setSeller(this);
        productsCatalog->addProduct(product);
        products.push_back(product);
    }

    void updateProductQuantity(ProductsCatalog* productsCatalog, Product* product, int newQuantity) {
        productsCatalog->updateProductQuantity(product, newQuantity);
    }

    void removeProduct(ProductsCatalog* productsCatalog, Product* product) {
        productsCatalog->removeProduct(product);
        products.erase(remove(products.begin(), products.end(), product), products.end());
    }
};

class Customer : public User {
private:
    Cart* cart;
    Order* currentOrder;
    vector<Order*> orderHistory;

public:
    Customer(string name, string phone) : User(name, phone) {
        cart = new Cart();
        orderHistory = vector<Order*>();
    }

    void addItemToCart(Item* item) {
        cart->addItem(item);
    }

    void removeItemFromCart(Item* item) {
        cart->removeItem(item);
    }

    void printCartItems() {
        cart->printItems();
    }

    void updateItemCount(Item* item, int newQuantity) {
        cart->updateItemCount(item, newQuantity);
    }

    void placeOrder() {
        currentOrder = new Order();
        currentOrder->setOrderStatus(OrderStatus::UNSHIPPED);
        currentOrder->setOrderDate(time(0));
        vector<Item*> orderedItems = cart->getItems();
        double orderAmount = 0;
        for (Item* item : orderedItems) {
            orderAmount += item->getPrice();
        }
        currentOrder->setItems(orderedItems);
        currentOrder->setAmount(orderAmount);
        currentOrder->setShippingAddress(getAddress());
        cart->checkout();
        orderHistory.push_back(currentOrder);
    }

    vector<Order*> getOrderHistory() {
        return orderHistory;
    }

    Order* getCurrentOrder() {
        return currentOrder;
    }
};

class Item {
private:
    int productId;
    int count;
    double price;

public:
    Item(Product* product, int count) : productId(product->getId()), count(count), price(product->getPrice() * count) {}

    double getPrice() {
        return price;
    }

    void updateCount(int newCount) {
        price = (price / count) * newCount;
        count = newCount;
    }
};

class Cart {
private:
    vector<Item*> itemList;

public:
    void addItem(Item* item) {
        itemList.push_back(item);
    }

    void removeItem(Item* item) {
        itemList.erase(remove(itemList.begin(), itemList.end(), item), itemList.end());
    }

    void updateItemCount(Item* item, int newCount) {
        for (Item* i : itemList) {
            if (i == item) {
                i->updateCount(newCount);
                return;
            }
        }
    }

    vector<Item*> getItems() {
        return itemList;
    }

    void printItems() {
        for (Item* item : itemList) {
            cout << "Item{" << item->getPrice() << "}" << endl;
        }
    }

    void checkout() {
        itemList.clear();
    }
};

enum OrderStatus {
    CREATED, PENDING, UNSHIPPED, SHIPPED, CANCELLED, COMPLETED
};

class OrderLog {
private:
    time_t creationTimestamp;
    OrderStatus status;

public:
    OrderLog(time_t creationTimestamp, OrderStatus status) : creationTimestamp(creationTimestamp), status(status) {}
};

class Order {
private:
    static int numberCounter;
    int orderNumber;
    OrderStatus orderStatus;
    time_t orderDate;
    vector<Item*> items;
    double amount;
    string shippingAddress;
    vector<OrderLog*> orderLogs;

public:
    Order() : orderNumber(numberCounter++) {
        orderLogs = vector<OrderLog*>();
        addOrderLog(new OrderLog(time(0), OrderStatus::CREATED));
    }

    Shipment* moveToShipment() {
        Shipment* shipment = new Shipment(this);
        return shipment;
    }

    void addOrderLog(OrderLog* orderLog) {
        orderLogs.push_back(orderLog);
    }

    void setOrderStatus(OrderStatus orderStatus) {
        this->orderStatus = orderStatus;
        addOrderLog(new OrderLog(time(0), orderStatus));
    }

    void setOrderDate(time_t orderDate) {
        this->orderDate = orderDate;
    }

    void setItems(vector<Item*> items) {
        this->items = items;
    }

    void setAmount(double amount) {
        this->amount = amount;
    }

    void setShippingAddress(string shippingAddress) {
        this->shippingAddress = shippingAddress;
    }
};

int Order::numberCounter = 0;

class Shipment {
private:
    static int numberCounter;
    int shipmentNumber;
    time_t date;
    time_t estimatedArrival;
    Order* orderDetails;

public:
    Shipment(Order* orderDetails) : shipmentNumber(numberCounter++) {
        date = time(0);
        estimatedArrival = date + 3 * 24 * 60 * 60; // 3 days
        this->orderDetails = orderDetails;
        orderDetails->setOrderStatus(OrderStatus::SHIPPED);
    }
};

int Shipment::numberCounter = 0;

class ProductsCatalog {
private:
    vector<Product*> products;
    vector<Category*> categories;
    unordered_map<string, vector<Product*>> categoryProductMap;
    unordered_map<string, vector<Seller*>> productSellerMap;
    unordered_map<string, vector<Product*>> similarProducts;

public:
    ProductsCatalog() : products(vector<Product*>()), categories(vector<Category*>()), categoryProductMap(unordered_map<string, vector<Product*>>()), productSellerMap(unordered_map<string, vector<Seller*>>()), similarProducts(unordered_map<string, vector<Product*>>()) {
        similarProducts["Dummy Product"] = vector<Product*>();
    }

    void updateSimilarProductsMap(Product* newProduct) {
        string productName = newProduct->getName();
        for (auto& pair : similarProducts) {
            if (pair.first.find(productName)!= string::npos || productName.find(pair.first)!= string::npos) {
                pair.second.push_back(newProduct);
                return;
            }
        }
        similarProducts[productName] = vector<Product*>({newProduct});
    }

    void updateCategoryProductMap() {
        for (Category* category : categories) {
            categoryProductMap[category->getName()] = vector<Product*>();
        }
    }

    void updateCategoryProductMap(Product* product) {
        if (categoryProductMap.find(product->getCategory().getName())!= categoryProductMap.end()) {
            categoryProductMap[product->getCategory().getName()].push_back(product);
        } else {
            categoryProductMap[product->getCategory().getName()] = vector<Product*>({product});
        }
    }

    void updateProductSellerMap(Product* newProduct) {
        if (productSellerMap.find(newProduct->getName())!= productSellerMap.end()) {
            productSellerMap[newProduct->getName()].push_back(newProduct->getSeller());
        } else {
            productSellerMap[newProduct->getName()] = vector<Seller*>({newProduct->getSeller()});
        }
    }

    void addCategory(Category* newCategory) {
        categories.push_back(newCategory);
        updateCategoryProductMap();
    }

    void addProduct(Product* product) {
        products.push_back(product);
        updateProductSellerMap(product);
        updateCategoryProductMap(product);
        updateSimilarProductsMap(product);
    }

    vector<Product*> searchProduct(string productName) {
        return similarProducts[productName];
    }
    vector<Product*> searchCategory(string categoryName) {
        return categoryProductMap[categoryName];
    }

    void updateProductQuantity(Product* product, int newQuantity) {
        product->setAvailableCount(newQuantity);
    }

    void removeProduct(Product* product) {
        products.erase(remove(products.begin(), products.end(), product), products.end());
    }
};

int main() {

    Category* electronics = new Category("Electronics");
    Category* clothing = new Category("Clothing");

    ProductsCatalog* productsCatalog = new ProductsCatalog();
    productsCatalog->addCategory(electronics);
    productsCatalog->addCategory(clothing);

    Seller* seller1 = new Seller("Seller 1", "1234567890");
    Seller* seller2 = new Seller("Seller 2", "9876543210");

    Product* product1 = new Product("TV", "LED TV", 50000.0, *electronics, 10);
    Product* product2 = new Product("Shirt", "Cotton Shirt", 500.0, *clothing, 20);

    productsCatalog->addProduct(product1);
    productsCatalog->addProduct(product2);

    seller1->registerProduct(productsCatalog, product1);
    seller2->registerProduct(productsCatalog, product2);

    Customer* customer = new Customer("Customer 1", "1234567890");
    customer->addItemToCart(new Item(product1, 2));
    customer->addItemToCart(new Item(product2, 3));
    customer->printCartItems();
    customer->placeOrder();

    return 0;
}
