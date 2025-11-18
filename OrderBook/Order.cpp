#define _CRT_SECURE_NO_WARNINGS
#include "Order.h"
#include <random>
#include <chrono>
#include <iomanip>
#include <sstream>



Order::Order(orderSide side, orderType timeInForce, int quantity, double price):orderTime(createOrderTime()) {
	this->side = side;
	this->timeInForce = timeInForce;
	this->quantity = quantity;
	this->orderPrice = price;
	this->orderID = this->createOrderId();
	}
Order::Order() :orderTime(createOrderTime()) {
	this->orderID = this->createOrderId();
}

 std::string Order::createOrderId() {
	char idArray[37];

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> dist(0, 9);

	
	for (int a = 0; a < 36; a++) {
		idArray[a] = '0' + dist(gen);
	}
	idArray[36] = '\0';

	return std::string(idArray);
}


 std::string Order::createOrderTime() {
	 using namespace std::chrono;

	 auto now = system_clock::now();
	 std::time_t t = system_clock::to_time_t(now);
	 std::tm tm = *std::localtime(&t);

	 std::ostringstream oss;
	 oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

	 return oss.str();
 }


 void Order::printOrder() const {
	std::cout << "Order ID = " << orderID << ", ";
	std::cout << " Order Symbol = " << stockSymbol << ", ";
	if (this->side == buy) {
		std::cout << "Order Side = Buy" << ", ";

	}
	else {
		std::cout << "Order Side = Sell" << ", ";

	}
	//Currently Only Have GTC TYPE
	std::cout << "Order Type = GTC"<<", ";


	std::cout << "Order Amount = " << quantity << ", ";
	std::cout << "Order Price = " << orderPrice << " ,";
	std::cout << " Order Placed At " << this->getOrderTime() << std::endl;
}
//Getters
orderSide Order::getSide() const { return this->side; }
orderType Order::getTimeInForce() const { return this->timeInForce; }
int Order::getQuantity() const { return this->quantity; }
double Order::getOrderPrice() const { return this->orderPrice; }
std::string Order::getOrderId() const { return this->orderID; }
std::string Order::getOrderTime() const { return orderTime; }
std::string Order::getStockSymbol() const { return stockSymbol; }

