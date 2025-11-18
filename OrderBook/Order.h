#pragma once
#include "string.h"
#include <iostream>
#include "OrderType.h"
#include "ctime"
class Order
{
private:
	const std::string stockSymbol;
	orderSide side;
	orderType timeInForce; 
	int quantity;
	double orderPrice; 
	std::string orderID;
	 std::string orderTime;

	std::string createOrderTime();
	std::string createOrderId();
	
public:
	Order(orderSide side, orderType timeInForce, int quantity, double price);
	Order();
	
	void printOrder() const; 
	
	//Getters 
	std::string getStockSymbol() const;
	orderSide getSide() const;
	orderType getTimeInForce() const;
	int getQuantity() const;
	double getOrderPrice() const;
	std::string getOrderTime() const;
	std::string getOrderId() const;

};

