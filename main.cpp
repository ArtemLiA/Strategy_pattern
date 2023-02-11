#include <iostream>
#include <vector>

class discount_type{
public:
    virtual double discount(double total, double num_of_sum) = 0;
};

class fixed_discount: discount_type{
    double disc_percentage;
public:
    explicit fixed_discount(double percent): disc_percentage(percent){};
    double discount(double total, double amount) final{
        return total * disc_percentage;
    }
};

class volume_discount: discount_type{
    double disc_percentage;
    double items_for_discount;
public:
    volume_discount(double items, double percent): items_for_discount(items), disc_percentage(percent) {};
    double discount(double total, double quantity) final{
        if (quantity > items_for_discount){
            return total * disc_percentage;
        }
        return 0;
    }
};

class price_discount: discount_type{
    double disc_percentage;
    double product_total_for_discount;
public:
    price_discount(double sum, double percent): product_total_for_discount(sum), disc_percentage(percent){};
    double discount(double total, double amount_of_product) final{
        if (amount_of_product > product_total_for_discount){
            return total * disc_percentage;
        }
        return 0;
    }
};

class amount_discount: discount_type{
    double disc_percentage;
    double total_for_dist;
public:
    amount_discount(double total, double percent): total_for_dist(total), disc_percentage(percent){};
    double discount(double total, double amount) final{
        if (amount > total_for_dist){
            return total * disc_percentage;
        }
        return 0;
    }
};

struct customer{
    std::string name;
    discount_type* discounts;
};

enum class article_unit{
    piece, kg, meter, sqmeter, cmeter, liter
};

struct article{
    size_t id;
    std::string name;
    double price;
    article_unit unit;
    discount_type* discountType;
};

struct order_line{
    article product;
    int quantity;
    discount_type* discount;
};

struct order{
    size_t id;
    customer* buyer;
    std::vector<order_line> lines;
    discount_type* discount;
};


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

