#include <iostream>
#include <vector>

class discount_type{
public:
    virtual double discount(double total, double num_of_sum) = 0;
    virtual double get_discount_percent(double total) = 0;
    virtual bool is_need_quantity() = 0;
};

class fixed_discount: public discount_type{
    double disc_percentage;
public:
    explicit fixed_discount(double percent): disc_percentage(percent){};
    double discount(double total, double amount) final{
        return total * disc_percentage;
    }
    double get_discount_percent(double total) final {
        return disc_percentage;
    }
    bool is_need_quantity() final { return false;};
};

class volume_discount: public discount_type{
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
    double get_discount_percent(double quantity) final {
        return disc_percentage;
    }
    bool is_need_quantity() final {return true;};
};

class price_discount: public discount_type{
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
    double get_discount_percent(double amount_of_product) final {
        return disc_percentage;
    }
    bool is_need_quantity() final {return false;};
};

class amount_discount: public discount_type{
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
    double get_discount_percent(double amount) final {
        return disc_percentage;
    }
    bool is_need_quantity() final {return false;};
};

struct customer{
    customer(std::string n, discount_type* dis_t): name(n), discounts(dis_t){};
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

class Calculate_order_price{
protected:
    virtual double get_price(order* o) = 0;
};

class for_line_price_calculator: Calculate_order_price{
public:
    double get_price(order* o) final {
        double total_sum = 0;
        double line_price = 0;
        for (const auto it: o->lines){
            line_price = it.product.price * it.quantity;
            total_sum += line_price;
            if (it.discount == nullptr){
                continue;
            }
            if (it.discount->is_need_quantity()){
                total_sum -= it.quantity * it.product.price * it.discount->get_discount_percent(it.quantity);
                continue;
            }
        }
        if (o->discount == nullptr){
            return total_sum;
        }
        return total_sum * (1 - o->discount->get_discount_percent(total_sum));
    }
};

class cumulative_price_calculator: Calculate_order_price{
public:
    double get_price(order* o) final {
        double total_sum = 0;
        for (const auto it : o->lines){
            total_sum += it.product.price * it.quantity; //Считаем цену без скидок
        }
        for (const auto it: o->lines){
            if (it.discount == nullptr){
                continue;
            }
            if (it.discount->is_need_quantity()){
                total_sum = total_sum * (1 - it.discount->get_discount_percent(it.quantity));
                continue;
            }
            total_sum = total_sum * (1 - it.discount->get_discount_percent(it.quantity * it.product.price));
        }
        if (o->discount == nullptr){
            return total_sum;
        }
        return total_sum * (1 -  o->discount->get_discount_percent(total_sum));
    }
};

class without_discount_calculator: Calculate_order_price{
public:
    double get_price(order* o) final {
        double total_sum = 0;
        for (const auto it: o->lines){
            total_sum += it.product.price * it.quantity;
        }
        return total_sum;
    }
};


int main() {

    fixed_discount fixed(0.05);
    volume_discount opt(50, 0.1);
    price_discount uni(100000, 0.15);
    amount_discount all(200000, 0.07);

    customer c1("Jason", &opt);
    customer c2("John", &fixed);
    customer c3("Joane", &uni);

    article a1 {1, "pen", 5, article_unit::piece, nullptr};
    article a2 {2, "expensive pen", 15, article_unit::piece, &opt};
    article a3{3, "scissors", 10, article_unit::piece, &fixed};

    cumulative_price_calculator calc1;
    for_line_price_calculator calc2;
    without_discount_calculator calc3;

    order o1 {101, &c1, {{a1, 1, &opt}, {a3, 4, &uni}, {a2, 80, &opt}}, &all};
    order o2 {101, &c1, {{a1, 1, nullptr}, {a3, 4, nullptr}, {a2, 80, nullptr}}, nullptr};


    std::cout << "Calculator 1:  " << calc1.get_price(&o1) << std::endl;
    std::cout << "Calculator 2:  " << calc2.get_price(&o1) << std::endl;
    std::cout << "Without discount calculator 1:  " << calc1.get_price(&o2) << std::endl;
    std::cout << "Without discount calculator 3:  " << calc2.get_price(&o2) << std::endl;
    std::cout << "Without discount:  " << calc3.get_price(&o2) << std::endl;
    return 0;
}

