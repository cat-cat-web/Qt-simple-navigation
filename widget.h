#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include"graph.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void on_pushButton_search_clicked();

    void on_pushButton_navigation_clicked();

    void on_pushButton_pathOfAll_clicked();

    void on_pushButton_MST_clicked();


    void on_pushButton_TGS_clicked();

    void on_pushButton_FXS_clicked();

    void on_pushButton_YFS_clicked();

    void on_pushButton_LSS_clicked();

    void on_pushButton_LYS_clicked();

    void on_pushButton_FLF_clicked();



private:
    Ui::Widget *ui;

    static const int vexNum_s = 6;
    Graph* graph;
    std::vector<Spot> spots;

    //从文件中加载数据
    void loadGraphFromJson(const QString& filePath); // 加载 JSON 文件
};
#endif // WIDGET_H
