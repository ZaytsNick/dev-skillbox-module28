#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <mutex>

bool stationIsBusy = false;
std::mutex station;
std::mutex examinationStation;

void path(int timeForPath, int idTrain)
{
    std::this_thread::sleep_for(std::chrono::seconds(timeForPath));
    examinationStation.lock();
    if (stationIsBusy)
        std::cout << "\nTrain #" << idTrain << " is waiting for free space." << std::endl;
    examinationStation.unlock();
    // std::cout << "\nTrain #" << idTrain << " is waiting for free space." << std::endl;

    station.lock();
    std::cout << "Train #" << idTrain << " arrived at the station." << std::endl;
    examinationStation.lock();
    stationIsBusy = true;
    examinationStation.unlock();
    std::string command;
    while (command != "depart")
    {
        std::cout << "Enter \"depart\" for departure: ";
        std::cin >> command;
        if (command == "depart")
        {
            std::cout << "Train #" << idTrain << " departs from the station." << std::endl;
            stationIsBusy = false;
        }
    }
    station.unlock();
}
int main()
{
    std::srand(time(0));
    std::vector<std::thread> train;
    for (int i = 0; i < 3; ++i)
    {
        train.push_back(std::thread(path, rand() % 5 + 1, i));
    }
    for (int i = 0; i < 3; ++i)
    {
        train[i].join();
    }
}