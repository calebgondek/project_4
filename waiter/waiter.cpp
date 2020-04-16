#include <string>
#include <exception>
#include "stdlib.h"
#include "../includes/externs.h"
#include "../includes/waiter.h"

using namespace std;

/*
 * ID is just a number used to identify this particular baker
 * (used with PRINT statements)
 * filename is what waiter reads in orders from
 */
Waiter::Waiter(int id, std::string filename) :
		id(id), myIO(filename) {
}

Waiter::~Waiter() {
}

/*
 * gets next Order from file_IO
 * if return == SUCCESS then anOrder
 * contains new order
 * otherwise return contains fileIO error
 */
int Waiter::getNext(ORDER &anOrder) {
	return myIO.getNext(anOrder); // Seems overly simplistic, but we'll see
}

/*
 * contains a loop that will get orders from filename one at a time
 * then puts them in order_in_Q then signals baker(s) using cv_order_inQ
 * so they can be consumed by baker(s)
 * when finished exits loop and signals baker(s) using cv_order_inQ that
 * it is done using b_WaiterIsFinished
 */
void Waiter::beWaiter() {
	int check;
	do {
		ORDER anOrder;
		check = getNext(anOrder);
		if (check == SUCCESS) {
			order_in_Q.push(anOrder); // If successful, push to the order queue
			cv_order_inQ.notify_all();
		}

		//Otherwise, fail out of the program (unless it's NO_ORDERS)
		if (check == COULD_NOT_OPEN_FILE || check == FAIL
				|| check == UNINITIALIZED) {
			throw new exception();
		}

	} while (check == SUCCESS);

	b_WaiterIsFinished = true;
	cv_order_inQ.notify_all();
}

