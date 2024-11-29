#include "Spot.h"
#include<iostream>
#include<QString>
// 默认构造函数
Spot::Spot() {}


Spot::Spot(QString name, QString discription, QString ticketPrice, QString openHours)
    : name(name), discription(discription),
    ticketPrice(ticketPrice), openHours(openHours) {}


// 获取属性
QString Spot::getName() const {
    return name;
}

QString Spot::getDiscription() const {
    return discription;
}

QString Spot::getTicketPrice() const {
    return ticketPrice;
}

QString Spot::getOpenHours() const {
    return openHours;
}

// 设置属性
void Spot::setName(const QString& newName) {
    name = newName;
}

void Spot::setDiscription(const QString& newDiscription) {
    discription = newDiscription;
}

void Spot::setTicketPrice(const QString& newTicketPrice) {
    ticketPrice = newTicketPrice;
}

void Spot::setOpenHours(const QString& newOpenHours) {
    openHours = newOpenHours;
}

void Spot::PrintSpot()
{


}
