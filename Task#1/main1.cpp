#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <map>
#include <mutex>

std::vector<std::string> top;
std::mutex topList;

void swim(int speed, std::string name)
{
    int distance(0), timer(0);
    while (distance < 100)
    {
        distance += speed;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << name << " Distance: " << ({ distance >= 100 ? 100 : distance % 100; })
                  << std::endl;
        timer++;
    }
    topList.lock();
    top.push_back(name);
    topList.unlock();
}
int main()
{
    std::srand(time(0));
    std::vector<std::thread> swimmingLanes;
    for (int i = 0; i < 6; ++i)
    {
        swimmingLanes.push_back(std::thread(swim, rand() % 11 + 5, "Swimmer#" + std::to_string(i + 1)));
    }
    for (int i = 0; i < 6; ++i)
    {
        swimmingLanes[i].join();
    }

    for (int i = 0; i < top.size(); ++i)
    {
        std::cout << "Place #" << i + 1 << ": " << top[i] << std::endl;
    }
}