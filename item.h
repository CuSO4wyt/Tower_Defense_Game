#ifndef ITEM_H
#define ITEM_H

#include <QPushButton>
#include <QLabel>

class GameScene;

class Item : public QObject
{
    Q_OBJECT

public:
    Item(GameScene *gameScene, const QString &itemName, int initialCount);

    void useItem(); // 使用道具
    void addItem(int count); // 增加道具数量
    int getItemCount() const; // 获取道具数量

private slots:
    void onButtonClicked(); // 按钮点击处理

private:
    GameScene *m_gameScene;
    QString m_itemName;
    int m_itemCount;
    QPushButton *m_button;
    QLabel *m_label;
};

#endif // ITEM_H
