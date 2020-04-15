#include <mutex>
#include <condition_variable>

#include "../includes/externs.h"
#include "../includes/baker.h"
using namespace std;

mutex m;

/*
 * ID is just a number used to identify this particular baker
 * (used with PRINT statements)
 */
Baker::Baker(int id) :
		id(id) {
}

Baker::~Baker() {
}

/*
 * bake, box and append to anOrder.boxes vector
 * if order has 13 donuts there should be 2 boxes
 * 1 with 12 donuts, 1 with 1 donut
 */
void Baker::bake_and_box(ORDER &anOrder) {
	int numDonuts = anOrder.number_donuts;
	while (numDonuts > 0) {
		Box tmp;
		while (tmp.size() <= DOZEN) {
			DONUT tmp2;
			if (!tmp.addDonut(tmp2)) {
				break;
			}
		}
		anOrder.boxes.push_back(tmp);

	}
}

/*
 * as long as there are orders in order_in_Q then
 * for each order:
 * 	create box(es) filled with number of donuts in the order
 * 	then place finished order on order_outvector
 * 	if waiter is finished (b_WaiterIsFinished) then
 * 	finish up remaining orders in order_in_Q and exit
 *
 * You will use cv_order_inQ to be notified by waiter
 * when either order_in_Q.size() > 0 or b_WaiterIsFinished == true
 * hint: wait for something to be in order_in_Q or b_WaiterIsFinished == true
 */
void Baker::beBaker() {
	{
		std::unique_lock<mutex> lck(mutex_order_inQ);
		cv_order_inQ.wait(lck);
	}
	while (!b_WaiterIsFinished && order_in_Q.size() > 0) {
		std::unique_lock<mutex> lock(m);
		ORDER tmp = order_in_Q.front();
		bake_and_box(tmp);
		order_in_Q.pop();
		order_out_Vector.push_back(tmp);

	}
}
