#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>

int onlineOrders(0), readyOrders(0), deliveredOrders(0);
std::mutex orders, cook;
void delivery()
{
    while (true)
    {
        orders.lock();
        if (readyOrders > 0)
        {
            readyOrders--;
            orders.unlock();
        }
        else
        {
            orders.unlock();
            continue;
        }
        std::this_thread::sleep_for(std::chrono::seconds(30));
        orders.lock();
        deliveredOrders++;
        std::cout << "Delivered orders: " << deliveredOrders << std::endl;
        orders.unlock();
    }
}
void cooking()
{
    while (true)
    {
        cook.lock();
        orders.lock();
        if (onlineOrders > 0)
        {
            onlineOrders--;
            orders.unlock();
        }
        else
        {
            orders.unlock();
            cook.unlock();
            continue;
        }
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 16 + 5));
        orders.lock();
        readyOrders++;
        std::cout << "Ready orders: " << readyOrders << std::endl;
        orders.unlock();
        cook.unlock();
    }
}
void receiptOfOrder()
{
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(rand() % 11 + 5));
        orders.lock();
        onlineOrders++;
        std::cout << "Orders: " << onlineOrders << std::endl;
        orders.unlock();
    }
}
int main()
{
    std::thread waiter(receiptOfOrder);
    std::thread kitchen(cooking);
    std::thread courier(delivery);
    while (true)
    {
        orders.lock();
        if (deliveredOrders > 9)
        {
            waiter.detach();
            kitchen.detach();
            courier.detach();
            orders.unlock();
            break;
        }
        orders.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
}