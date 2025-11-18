#pragma once
enum orderSide {
	buy = 1,
	sell = 0,

};
enum orderType {
	limitOrder = 0,
	marketOrder =1,
	stopOrder =2,
};
enum timeInForce {
	goodTillCancelled = 0
};