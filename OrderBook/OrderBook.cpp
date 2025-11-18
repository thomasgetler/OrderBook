#include "OrderBook.h"
#include <exception>
#include <sstream>
#include <algorithm>


void OrderBook::updateOrder(const Order& orderRequest) {

    if (orderRequest.getSide() == buy) {

        for (size_t i = 0; i < buyOrders.size(); ++i) {
            if (buyOrders[i].getOrderId() == orderRequest.getOrderId()) {
                buyOrders.erase(buyOrders.begin() + i);  
                buyOrders.push_back(orderRequest);       
                return;                                 
            }
        }
    }
    else {

        for (size_t i = 0; i < sellOrders.size(); ++i) {
            if (sellOrders[i].getOrderId() == orderRequest.getOrderId()) {
                sellOrders.erase(sellOrders.begin() + i);
                sellOrders.push_back(orderRequest);
                return;
            }
        }
    }
}


void OrderBook::addOrder(Order orderRequest) {
    try {
        this->validateOrder(orderRequest);
       
    }
    catch (const std::logic_error& e) {
        std::cout << "ORDER IS INCOMPLETE: " << e.what() << " ORDER IS REJECTED" << std::endl;
        return;
    }

    if (orderRequest.getSide() == buy) {
        buyOrders.push_back(orderRequest);
    }
    if (orderRequest.getSide() == sell) {
        sellOrders.push_back(orderRequest);
    }
	
}

void OrderBook::validateOrder(const Order& orderRequest) {
    if (orderRequest.getQuantity() <= 0) {   
        throw std::logic_error("Quantity cannot be zero or negative");
    }
    if (orderRequest.getOrderPrice() <= 0) {
        throw std::logic_error("Price cannot be zero or negative");
    }
    if (orderRequest.getSide() != buy && orderRequest.getSide() != sell) {
        throw std::logic_error("Price cannot be zero or negative");

    }
    if (orderRequest.getTimeInForce() != goodTillCancelled) {
        throw std::logic_error("Currently This Order Book Only Supports Good Till Cancelled Orders");
    }
    if (orderRequest.getStockSymbol() != this->stockSymbol) {
        throw std::logic_error("Error Wrong Stock For This Order Book");
    }
}

std::unordered_map<std::string, std::string> OrderBook::splitString(const std::string& dateTime) {
    std::unordered_map<std::string, std::string> timeAndDateLookUp;

    // Split date and time by space
    std::istringstream iss(dateTime);
    std::string datePart, timePart;
    if (!(iss >> datePart >> timePart)) {
        return {}; // invalid format
    }

    // Split date by '-'
    std::istringstream dateStream(datePart);
    std::string token;
    std::vector<std::string> dateSplit;
    while (std::getline(dateStream, token, '-')) {
        dateSplit.push_back(token);
    }

    // Split time by ':'
    std::istringstream timeStream(timePart);
    std::vector<std::string> timeSplit;
    while (std::getline(timeStream, token, ':')) {
        timeSplit.push_back(token);
    }


    if (dateSplit.size() == 3) {
        timeAndDateLookUp["Year"] = dateSplit[0];
        timeAndDateLookUp["Month"] = dateSplit[1];
        timeAndDateLookUp["Day"] = dateSplit[2];
    }

    if (timeSplit.size() == 3) {
        timeAndDateLookUp["Hour"] = timeSplit[0];
        timeAndDateLookUp["Min"] = timeSplit[1];
        timeAndDateLookUp["Second"] = timeSplit[2];
    }

    return timeAndDateLookUp;

}


bool OrderBook::dateComparison(const std::unordered_map<std::string, std::string>& firstDate,
    const std::unordered_map<std::string, std::string>& secondDate)
{

    const std::string keys[] = { "Year", "Month", "Day", "Hour", "Min", "Second" };

    for (const auto& key : keys) {
        int firstValue = std::stoi(firstDate.at(key));
        int secondValue = std::stoi(secondDate.at(key));

        if (firstValue > secondValue) return true;  
        if (firstValue < secondValue) return false;
    }

    // All units are equal
    return false;
}
void OrderBook::printBuyOrders() const {
    for ( const auto& each : buyOrders) {
        each.printOrder();
    }
}

void OrderBook::printSellOrders()const {
    for (const auto& each : sellOrders) {
        each.printOrder();
    }
}



void OrderBook::sortBuyOrders() {
    std::sort(buyOrders.begin(), buyOrders.end(),
        [](const Order& a, const Order& b) {
            return a.getOrderPrice() > b.getOrderPrice(); // highest first
        }
    );
}

void OrderBook::sortSellOrders() {
    std::sort(sellOrders.begin(), sellOrders.end(),
        [](const Order& a, const Order& b) {
            return a.getOrderPrice() < b.getOrderPrice(); // lowest first
        }
    );

}


    void OrderBook::runOrders() {
        // Sort the order books first
        sortBuyOrders();   // highest first
        sortSellOrders();  // lowest first

        // Outer loop over buy orders
        for (size_t i = 0; i < buyOrders.size(); ++i) {
            Order& buyOrder = buyOrders[i];

            // Inner loop over sell orders
            for (size_t j = 0; j < sellOrders.size(); ++j) {
                Order& sellOrder = sellOrders[j];

                // Trade possible?
                if (buyOrder.getOrderPrice() >= sellOrder.getOrderPrice()) {

                    int tradeQty = std::min(buyOrder.getQuantity(), sellOrder.getQuantity());

                    // Execute trade
                    std::cout << "Trade executed: "
                        << tradeQty << " units at price "
                        << sellOrder.getOrderPrice() << std::endl;
                 
                        // Update quantities
                        buyOrder.setQuantity(buyOrder.getQuantity() - tradeQty);
                    sellOrder.setQuantity(sellOrder.getQuantity() - tradeQty);

                    // Remove fully filled orders
                    if (buyOrder.getQuantity() == 0) {
                        buyOrders.erase(buyOrders.begin() + i);
                        --i; // adjust index after erase
                        break; // move to next buy order
                    }

                    if (sellOrder.getQuantity() == 0) {
                        sellOrders.erase(sellOrders.begin() + j);
                        --j; // adjust index after erase
                    }
                }
            }
        }
    }

    void OrderBook::deleteOrders(const std::string& orderID) {

        // --- BUY ORDERS ---
        for (int i = 0; i < buyOrders.size(); i++) {
            if (buyOrders[i].getOrderId() == orderID) {
                buyOrders.erase(buyOrders.begin() + i);
                std::cout << "ORDER ID: " << orderID << " HAS BEEN DELETED" << std::endl;
                this->sortBuyOrders();
                return;
            }
        }

        // --- SELL ORDERS ---
        for (int i = 0; i < sellOrders.size(); i++) {
            if (sellOrders[i].getOrderId() == orderID) {

                // ? BUG FIX: You used buyOrders.begin() here by mistake
                sellOrders.erase(sellOrders.begin() + i);

                std::cout << "ORDER ID: " << orderID << " HAS BEEN DELETED" << std::endl;
                this->sortSellOrders();
                return;
            }
        }

        std::cout << "COULD NOT FIND ANY ORDER IN BUY OR SELL LIST WITH ID "
            << orderID << std::endl;
    }