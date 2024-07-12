#include "item.h"
#include "gamescene.h"

Item::Item(GameScene *gameScene, const QString &itemName, int initialCount)
    : m_gameScene(gameScene), m_itemName(itemName), m_itemCount(initialCount)
{
    m_button = new QPushButton(itemName, gameScene);
    m_label = new QLabel(QString::number(initialCount), gameScene);

    // 设置按钮和标签的位置
    m_button->setGeometry(850, 50, 150, 30);
    m_label->setGeometry(835, 50, 50, 30);
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::red); // 将字体颜色设置为红色
    m_label->setPalette(palette);

    connect(m_button, &QPushButton::clicked, this, &Item::onButtonClicked);
}

void Item::useItem()
{
    if (m_itemCount > 0) {
        m_itemCount--;
        m_label->setText(QString::number(m_itemCount));
        // 清屏道具的效果：移除所有敌人
        m_gameScene->clearEnemies();
    }
}

void Item::addItem(int count)
{
    m_itemCount += count;
    m_label->setText(QString::number(m_itemCount));
}

int Item::getItemCount() const
{
    return m_itemCount;
}

void Item::onButtonClicked()
{
    useItem();
}
