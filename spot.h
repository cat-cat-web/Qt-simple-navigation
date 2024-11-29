#ifndef SPOT_H
#define SPOT_H

#include <QString>

class Spot
{
private:
    QString name;          // 景点名称
    QString discription;   // 景点描述
    QString ticketPrice;   // 门票价格
    QString openHours;     // 开放时间

public:
    // 默认构造函数
    Spot();

    // 参数化构造函数
    Spot(QString name, QString discription, QString ticketPrice, QString openHours);

    Spot& operator=(const Spot&) = default;

    // 获取属性
    QString getName() const;
    QString getDiscription() const;
    QString getTicketPrice() const;
    QString getOpenHours() const;

    // 设置属性
    void setName(const QString& newName);
    void setDiscription(const QString& newDiscription);
    void setTicketPrice(const QString& newTicketPrice);
    void setOpenHours(const QString& newOpenHours);
    void PrintSpot();//用作调试
};

#endif // SPOT_H
