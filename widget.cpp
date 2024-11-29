#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include "widget.h"
#include "ui_widget.h"
#include "graph.h"
#include<QInputDialog>
#include <QPainter>
#include <QPaintEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , graph(nullptr)
{
    ui->setupUi(this);
    setFixedSize(510, 400);
    ui->plainTextEdit->setReadOnly(true);
    ui->lineEdit_spot->setPlaceholderText("请输入景点名称");
    ui->lineEdit_start->setPlaceholderText("请输入起点");
    ui->lineEdit_end->setPlaceholderText("请输入终点");

    try{
        loadGraphFromJson("D:\\Desktop\\C++\\Qt\\navigation\\information.json");
    }catch(const std::exception& e)
    {
         QMessageBox::critical(this, "Error", e.what());
    }
}

Widget::~Widget()
{
    delete ui;
    if(graph)
    {
        delete graph;
    }
}

void Widget::paintEvent(QPaintEvent* event)
{
    ui->pushButton_TGS->raise();
    ui->pushButton_FXS->raise();
    // 在 paintEvent 中进行绘制
    QPainter painter(this);  // 创建一个绘制器，绑定到当前 Widget

    painter.setPen(QPen(Qt::black, 2));  // 设置画笔，黑色，粗细为2

/* 不要学我，时间有限直接暴力画线了 */
    // 0->1
    QPoint start(60, 110);
    QPoint end(60, 300);
    painter.drawLine(start, end);

    // 0->2
    QPoint start2(60, 110);
    QPoint end2(130, 300);
    painter.drawLine(start2, end2);

    // 0->5
    QPoint start3(60, 110);
    QPoint end3(200, 110);
    painter.drawLine(start3, end3);

    // 1->3->5
    QPoint start4(130, 300);
    QPoint end4(300, 150);
    painter.drawLine(start4, end4);

    QPoint start5(300, 150);
    QPoint end5(200, 110);
    painter.drawLine(start5, end5);

    QPoint start6(60, 300);
    QPoint end6(200, 110);
    painter.drawLine(start6, end6);
}


//从文件中读取数据并构造Graph和Spot
void Widget::loadGraphFromJson(const QString& filename)
{
    // 打开文件
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Error", "Failed to open file: " + filename);
        return;
    }

    // 读取文件内容
    QByteArray data = file.readAll();
    file.close();

    // 解析JSON文档
    QJsonDocument document = QJsonDocument::fromJson(data);
    if (document.isNull() || !document.isObject()) {
        QMessageBox::critical(this, "Error", "Invalid JSON format in file: " + filename);
        return;
    }

    QJsonObject obj = document.object();
    QJsonArray spotsArray = obj["spots"].toArray();
    QJsonArray matrixArray = obj["matrix"].toArray();

    // 清空 spots 向量
    spots.clear();

    // 构造 Spot 数组
    for (const auto& spotVal : spotsArray) {
        QJsonObject spotObj = spotVal.toObject();
        Spot newSpot(
            spotObj["name"].toString(),
            spotObj["description"].toString(),
            spotObj["ticketPrice"].toString(),
            spotObj["openHours"].toString()
            );
        spots.push_back(newSpot);  // 动态添加到 vector 中

        std::cout << "Loaded Spot: " << newSpot.getName().toStdString() << ", "
                  << newSpot.getDiscription().toStdString() << std::endl;
    }

    std::cout << "Graph:" << std::endl;

    // 构造邻接矩阵
    std::vector<std::vector<double>> matrix(vexNum_s, std::vector<double>(vexNum_s, 9999));  // 使用 vector 初始化矩阵

    for (int i = 0; i < vexNum_s; ++i) {
        QJsonArray row = matrixArray[i].toArray();
        for (int j = 0; j < vexNum_s; ++j) {
            if (i < row.size() && row[j].isDouble()) {
                matrix[i][j] = row[j].toDouble();
            }
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // 创建 Graph 对象
    graph = new Graph(spots, matrix);  // 将 spots 和 matrix 传递给 Graph
    graph->PrintGraph();

    // 通知成功加载
    QMessageBox::information(this, "Success", "Graph successfully loaded from JSON.");
}



// 查询两点之间最短路径(Floyd)
void Widget::on_pushButton_navigation_clicked()
{
    QString start_String = ui->lineEdit_start->text();
    QString end_String = ui->lineEdit_end->text();

    // 获取景点索引
    int start = -1, end = -1;

    // 遍历 Spots 向量，查找对应的景点索引
    for (int i = 0; i < spots.size(); ++i) {
        if (spots[i].getName() == start_String) {
            start = i; // 找到起始景点索引
        }
        if (spots[i].getName() == end_String) {
            end = i; // 找到目标景点索引
        }
    }

    // 检查是否找到了起点和终点
    if (start == -1 || end == -1) {
        QMessageBox::warning(this, "输入错误", "无法找到对应的景点，请检查输入的景点名称！");
        return;
    }

    //找到则调用Floyd算法（两点之间最短路径）
    graph->Floyd();
    graph->printFloydPath(start, end);
}


//推荐游览路线（通过DFS）
void Widget::on_pushButton_pathOfAll_clicked()
{

    // 提示用户输入起点名称
    bool ok;
    QString startName = QInputDialog::getText(this, "输入起点名称", "请输入起点名称:", QLineEdit::Normal, "", &ok);
    int startIndex=-1;
    //OK被点击
    if (ok && !startName.isEmpty()) {

        for(size_t i = 0; i<vexNum_s;++i)
        {
            if(spots[i].getName()==startName)
            {
                startIndex=i;
            }
        }

        if (startIndex == -1) {
            QMessageBox::warning(this, "错误", "未找到该起点！");
        } else {
            // 调用 DFS 算法，传入找到的索引
            graph->visited = std::vector<bool>(vexNum_s, false);
            graph->pathOfAll =std::vector<int>();
            graph->DFS(startIndex);

            // 打印 DFS 结果
            graph->printDFSresult();
        }
    } else {
        // 用户取消了输入
        QMessageBox::information(this, "取消", "操作已取消");
    }
}


//MST铺设道路
void Widget::on_pushButton_MST_clicked()
{
    graph->PrimMST();
}


//按景点显示景点信息
void Widget::on_pushButton_TGS_clicked()
{
    QString spotInfo = "Name: " + spots[0].getName() + "\n"
                       + "Description: " + spots[0].getDiscription() + "\n"
                       + "Open Hours: " + spots[0].getOpenHours() + "\n"
                       + "Ticket Price: " + spots[0].getTicketPrice();
    QMessageBox::information(this, "景点信息", spotInfo);
}


void Widget::on_pushButton_FXS_clicked()
{
    QString spotInfo = "Name: " + spots[1].getName() + "\n"
                       + "Description: " + spots[1].getDiscription() + "\n"
                       + "Open Hours: " + spots[1].getOpenHours() + "\n"
                       + "Ticket Price: " + spots[1].getTicketPrice();
    QMessageBox::information(this, "景点信息", spotInfo);
}


void Widget::on_pushButton_YFS_clicked()
{
    QString spotInfo = "Name: " + spots[2].getName() + "\n"
                       + "Description: " + spots[2].getDiscription() + "\n"
                       + "Open Hours: " + spots[2].getOpenHours() + "\n"
                       + "Ticket Price: " + spots[2].getTicketPrice();
    QMessageBox::information(this, "景点信息", spotInfo);
}


void Widget::on_pushButton_LSS_clicked()
{
    QString spotInfo = "Name: " + spots[3].getName() + "\n"
                       + "Description: " + spots[3].getDiscription() + "\n"
                       + "Open Hours: " + spots[3].getOpenHours() + "\n"
                       + "Ticket Price: " + spots[3].getTicketPrice();
    QMessageBox::information(this, "景点信息", spotInfo);
}


void Widget::on_pushButton_LYS_clicked()
{
    QString spotInfo = "Name: " + spots[4].getName() + "\n"
                       + "Description: " + spots[4].getDiscription() + "\n"
                       + "Open Hours: " + spots[4].getOpenHours() + "\n"
                       + "Ticket Price: " + spots[4].getTicketPrice();
    QMessageBox::information(this, "景点信息", spotInfo);
}


void Widget::on_pushButton_FLF_clicked()
{
    QString spotInfo = "Name: " + spots[5].getName() + "\n"
                       + "Description: " + spots[5].getDiscription() + "\n"
                       + "Open Hours: " + spots[5].getOpenHours() + "\n"
                       + "Ticket Price: " + spots[5].getTicketPrice();
    QMessageBox::information(this, "景点信息", spotInfo);
}


// 查询景点信息
void Widget::on_pushButton_search_clicked()
{
    QString purpose = ui->lineEdit_spot->text(); // 获取用户输入的景点名称

    for (int i = 0; i < vexNum_s; ++i) {
        if (purpose == spots[i].getName()) { // 如果找到匹配的景点
            // 构造景点信息
            QString spotInfo = "Name: " + spots[i].getName() + "\n"
                               + "Description: " + spots[i].getDiscription() + "\n"
                               + "Open Hours: " + spots[i].getOpenHours() + "\n"
                               + "Ticket Price: " + spots[i].getTicketPrice();

            // 显示景点信息
            QMessageBox::information(this, "查找成功", spotInfo);
            return; // 一旦找到就退出函数
        }
    }

    // 如果没有找到匹配的景点
    QMessageBox::warning(this, "查找失败", "没有找到该景点！");
}




