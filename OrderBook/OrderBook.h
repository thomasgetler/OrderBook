#pragma once
#include <vector>
#include"Order.h"
#include <iostream>
#include <unordered_map>
class OrderBook
{
private:
	const std::string stockSymbol = "TEST";
	std::vector<Order> buyOrders; 
	std::vector<Order> sellOrders;
	void validateOrder(const Order& orderRequest);
	std::unordered_map<std::string, std::string> splitString(const std::string& dateTime);
	bool dateComparison(const std::unordered_map<std::string,std::string>& firstDate, const std::unordered_map<std::string, std::string>& secondDate);
	void sortBuyOrders();
	void sortSellOrders();
	

public:
	void addOrder(Order orderRequest);
	void updateOrder(const Order &orderRequest);
	void printBuyOrders() const;
	void printSellOrders() const;
	void runOrders();
	void deleteOrders(const std::string&);

};

