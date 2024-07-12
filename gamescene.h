#ifndef GAMESCENE_H
#define GAMESCENE_H


#include <QWidget>
#include <QList>
#include <QTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPushButton>
#include <QComboBox>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QLabel>


#include "waypoint.h"

namespace Ui {
class GameScene;
}
class WayPoint;
class TowerPosition;
class Tower;
class MeleeTower;
class RangedTower;
class Enemy;
class Modifier;
class Item;

class GameScene : public QWidget
{
    Q_OBJECT
    friend Tower;
public:
    explicit GameScene(QWidget *parent = nullptr,int level = 1);
    ~GameScene();


    void DisplayMap(); // 展示地图


    void AddWayPoint_1(); // 添加第一条航点
    void AddWayPoint_2(); // 添加第二条航点


    void AddMeleeTowerPosition(); // 添加近战塔坑位
    void AddRangedTowerPosition(); // 添加远战塔坑位


    void mousePressEvent(QMouseEvent *event); // 处理鼠标点击的事件


    void Win();
    void Lost();

    void ResetMaxEnemy(int n);//重新设置最大敌人数量

    //清屏道具效果
    void clearEnemies();


    QList<Enemy *> getEnemies() const;
    QList<Tower *> getTowers() const;
    QList<MeleeTower *> getMeleeTowers();

    bool isGameOver() const;

    void removeEnemy(Enemy *enemy); //移除敌人
    void removeTower(Tower *tower); // 移除防御塔

private slots:
    void updateGame(); // 更新游戏状态
    void spawnEnemy(); // 刷新敌人
    void on_Load_Button_clicked();//加载游戏
    void on_Store_Button_clicked();//保存游戏

//存档导入导出
public slots:
    void saveGame(const QString &filePath);//保存游戏
    void loadGame(const QString &filePath);//加载游戏

//地图导入导出
public slots:
    void saveMap(); // 保存地图
    void loadMap(); // 加载地图

//暂停按钮
private slots:
    void pauseGame(); // 暂停游戏
    void resumeGame(); // 恢复游戏

//词缀安装
    void installModifier(); // 安装词缀
    void uninstallModifier(); // 卸下词缀




private:
    Ui::GameScene *ui;

protected:
    void paintEvent(QPaintEvent *event);
    QPixmap backgroundPixmap; // 地图背景
    QList<WayPoint*> WayPointList_1; // 存储第一条路径的航点
    QList<WayPoint*> WayPointList_2; // 存储第二条路径的航点
    QList<TowerPosition> MeleeTowerPosition; // 近战塔坑位
    QList<TowerPosition> RangedTowerPosition; // 远战塔坑位

    QList<MeleeTower*> CurrentMeleeTower;//存近战塔
    QList<RangedTower*> CurrentRangedTower;//存远战塔
    QList<Enemy*> enemies; // 存储敌人

    QList<Modifier *> meleeModifierLibrary; // 存储近战塔词缀
    QList<Modifier *> rangedModifierLibrary; // 存储远程塔词缀
    QList<Modifier *> enemyModifierLibrary; // 存储敌人词缀


    bool myGameOver;
    int totalEnemies; // 敌人总数
    int spawnedEnemies; // 已刷新的敌人数
    int enemyKilled;//击杀的敌人总数
    QTimer *updateTimer; // 更新定时器
    QTimer *spawnTimer; // 刷新敌人定时器
    bool isPaused; // 标记游戏是否暂停
    int gameStatus;//标记游戏状态
    int myMoney;//金币系统



    QPushButton *pauseButton; // 暂停按钮
    QMenu *contextMenu; // 右键菜单
    QAction *installAction; // 安装词缀动作
    QAction *uninstallAction; // 卸下词缀动作
    Tower *currentTower; // 当前选中的塔

    // 关卡相关
    int level; // 当前关卡
    int enemySpawnInterval; // 敌人生成间隔
    void setLevelParameters(int level); // 设置关卡参数

    // 道具系统
     Item *clearScreenItem;
     int enemyForCreateItem;


};

#endif // GAMESCENE_H
