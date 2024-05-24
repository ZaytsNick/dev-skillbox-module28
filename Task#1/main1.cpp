#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <map>
#include <mutex>

std::map<int, std::vector<std::string>> top;
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
    top[timer].push_back(name);
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
    int place(1);
    for (std::map<int, std::vector<std::string>>::iterator it = top.begin(); it != top.end(); ++it)
    {
        std::cout << "Place #"<<place<<": ";
        if (it->second.size() == 1)
            std::cout << it->second[0];
        else
        {
            for (int j = 0; j < it->second.size(); j++)
            {
                std::cout << it->second[j] << "|";
            }
        }
        std::cout << std::endl;
        place++;
    }
}