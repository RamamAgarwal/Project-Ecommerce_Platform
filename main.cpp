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
