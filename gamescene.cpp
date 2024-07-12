#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QAction>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include "gamescene.h"
#include "ui_gamescene.h"
#include "enemy.h"
#include "towerposition.h"
#include "meleetower.h"
#include "tower.h"
#include "rangedtower.h"
#include "enemyplus.h"
#include "modifier.h"
#include "mapdata.h"
#include "QSound"
#include "item.h"

GameScene::GameScene(QWidget *parent,int level) :
    QWidget(parent),
    ui(new Ui::GameScene),
    myGameOver(false),
    totalEnemies(10), // 设置敌人总数为8个
    spawnedEnemies(0),
    isPaused(false),
    gameStatus(0),
    enemyKilled(0),
    myMoney(500),
    level(level),
    enemyForCreateItem(0)
{
    ui->setupUi(this);
    AddWayPoint_1();
    AddWayPoint_2();
    AddMeleeTowerPosition();
    AddRangedTowerPosition();
    DisplayMap();

    setLevelParameters(level);

    // 设置定时器
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &GameScene::updateGame);
    updateTimer->start(100); // 每100毫秒更新一次游戏

    spawnTimer = new QTimer(this);
    connect(spawnTimer, &QTimer::timeout, this, &GameScene::spawnEnemy);
    spawnTimer->start(enemySpawnInterval);

    pauseButton = new QPushButton("Pause", this); //暂停按钮
        pauseButton->setGeometry(900, 20, 80, 30);
        connect(pauseButton, &QPushButton::clicked, this, [=]() {
            if (isPaused) {
                resumeGame();
            } else {
                pauseGame();
            }
        });

        // 创建右键菜单

    contextMenu = new QMenu(this);
    installAction = new QAction("Install Modifier", this);
    connect(installAction, &QAction::triggered, this, &GameScene::installModifier);
    contextMenu->addAction(installAction);

    uninstallAction = new QAction("Uninstall Modifier", this);
    connect(uninstallAction, &QAction::triggered, this, &GameScene::uninstallModifier);
    contextMenu->addAction(uninstallAction);

    //导入导出地图
    QPushButton *saveButton = new QPushButton("SaveMap", this);
    saveButton->setGeometry(100, 60, 120, 30);
    connect(saveButton, &QPushButton::clicked, this, &GameScene::saveMap);

    QPushButton *loadButton = new QPushButton("LoadMap", this);
    loadButton->setGeometry(100, 100,120, 30);
    connect(loadButton, &QPushButton::clicked, this, &GameScene::loadMap);

    // 背景音乐
    QMediaPlaylist *playlist = new QMediaPlaylist();
    playlist->addMedia(QUrl("qrc:/img/img/bgm.wav"));
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    QMediaPlayer *music = new QMediaPlayer();
    music->setPlaylist(playlist);
    music->play();

    //初始化道具
    clearScreenItem = new Item(this, "Clear Screen", 1); // 初始数量为1

    this->show();
}

void GameScene::setLevelParameters(int level) //设置关卡难度
{
    switch (level) {
    case 1:
        totalEnemies = 10;
        enemySpawnInterval = 2000; // 2000毫秒生成一个敌人
        break;
    case 2:
        totalEnemies = 20;
        enemySpawnInterval = 1500; // 1500毫秒生成一个敌人
        break;
    case 3:
        totalEnemies = 30000;
        enemySpawnInterval = 2000; // 1000毫秒生成一个敌人
        break;
    default:
        totalEnemies = 10;
        enemySpawnInterval = 2000; // 默认设置
        break;
    }
}

void GameScene::clearEnemies()
{
    while (!enemies.isEmpty()) {
        removeEnemy(enemies.first());
    }
}

void GameScene::pauseGame()
{
    isPaused = true;
    updateTimer->stop();
    spawnTimer->stop();
    pauseButton->setText("Resume");
}

void GameScene::resumeGame()
{
    isPaused = false;
    updateTimer->start(100);
    spawnTimer->start(5000);
    pauseButton->setText("Pause");
}

void GameScene::ResetMaxEnemy(int n)
{
    totalEnemies = n;
}

GameScene::~GameScene()
{
    delete ui;
}

void GameScene::DisplayMap()
{
    // 加载背景图片
    backgroundPixmap.load(":/img/img/map.png");
    // 触发重绘事件
    update();
}

void GameScene::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);



    if (!backgroundPixmap.isNull()) {
        painter.drawPixmap(0, 0, 1000, 625, backgroundPixmap);
    }




    foreach(const WayPoint *waypoint, WayPointList_1) //绘制航点1
    {
        waypoint->DrawPoint(&painter);
    }

    foreach(const WayPoint *waypoint, WayPointList_2) //绘制航点2
    {
        waypoint->DrawPoint(&painter);
    }

    for (int i = 1; i < WayPointList_1.size(); ++i) {
        painter.setPen(Qt::red);
        painter.drawLine(WayPointList_1[i - 1]->GetThisWayPoint(), WayPointList_1[i]->GetThisWayPoint());
    }

    for (int i = 1; i < WayPointList_2.size(); ++i) {
        painter.setPen(Qt::red);
        painter.drawLine(WayPointList_2[i - 1]->GetThisWayPoint(), WayPointList_2[i]->GetThisWayPoint());
    }

    //绘制TowerPosition
    foreach(TowerPosition position, MeleeTowerPosition)
    {
        position.DrawTowerPosition(&painter);
    }
    foreach(TowerPosition position, RangedTowerPosition)
    {
        position.DrawTowerPosition(&painter);
    }

    //绘制tower
    foreach(RangedTower *tower, CurrentRangedTower)
    {
        tower->Draw(&painter);
    }

    foreach(MeleeTower *tower, CurrentMeleeTower)
    {
        tower->Draw(&painter);
    }

    //绘制敌人
    foreach(Enemy *enemy, enemies)
    {
        enemy->draw(&painter);
    }

    if (gameStatus == 1) {
            painter.setPen(Qt::green);
            painter.setFont(QFont("Arial", 48, QFont::Bold));
            painter.drawText(rect(), Qt::AlignCenter, "You Win");
        }

    if (gameStatus == 2) {
            painter.setPen(Qt::red);
            painter.setFont(QFont("Arial", 48, QFont::Bold));
            painter.drawText(rect(), Qt::AlignCenter, "You Lost");
        }

    painter.setPen(Qt::yellow);
    painter.setFont(QFont("Arial", 20));
    painter.drawText(10, 40, "Money: " + QString::number(myMoney));

    if(level == 3)
    {
        painter.drawText(300, 40, "Killed: " + QString::number(enemyKilled));
    }


    QWidget::paintEvent(event);
}

void GameScene::AddWayPoint_1() //添加第一条航点
{
    WayPoint *waypoint1 = new WayPoint(QPoint(937, 63));
    WayPointList_1.push_back(waypoint1);

    WayPoint *waypoint2 = new WayPoint(QPoint(937, 188));
    waypoint1->SetNextWayPoint(waypoint2);
    WayPointList_1.push_back(waypoint2);

    WayPoint *waypoint3 = new WayPoint(QPoint(687, 188));
    waypoint2->SetNextWayPoint(waypoint3);
    WayPointList_1.push_back(waypoint3);

    WayPoint *waypoint4 = new WayPoint(QPoint(687, 313));
    waypoint3->SetNextWayPoint(waypoint4);
    WayPointList_1.push_back(waypoint4);

    WayPoint *waypoint5 = new WayPoint(QPoint(187, 313));
    waypoint4->SetNextWayPoint(waypoint5);
    WayPointList_1.push_back(waypoint5);

    WayPoint *waypoint6 = new WayPoint(QPoint(187, 563));
    waypoint5->SetNextWayPoint(waypoint6);
    WayPointList_1.push_back(waypoint6);

    WayPoint *waypoint7 = new WayPoint(QPoint(63, 563));
    waypoint6->SetNextWayPoint(waypoint7);
    WayPointList_1.push_back(waypoint7);
}

void GameScene::AddWayPoint_2() //添加第二条航点
{
    WayPoint *waypoint1 = new WayPoint(QPoint(937, 562));
    WayPointList_2.push_back(waypoint1);

    WayPoint *waypoint2 = new WayPoint(QPoint(562, 562));
    waypoint1->SetNextWayPoint(waypoint2);
    WayPointList_2.push_back(waypoint2);

    WayPoint *waypoint3 = new WayPoint(QPoint(562, 312));
    waypoint2->SetNextWayPoint(waypoint3);
    WayPointList_2.push_back(waypoint3);

    WayPoint *waypoint4 = new WayPoint(QPoint(187, 313));
    waypoint3->SetNextWayPoint(waypoint4);
    WayPointList_2.push_back(waypoint4);

    WayPoint *waypoint5 = new WayPoint(QPoint(187, 563));
    waypoint4->SetNextWayPoint(waypoint5);
    WayPointList_2.push_back(waypoint5);

    WayPoint *waypoint6 = new WayPoint(QPoint(63, 563));
    waypoint5->SetNextWayPoint(waypoint6);
    WayPointList_2.push_back(waypoint6);
}

void GameScene::AddMeleeTowerPosition() //手动添加近战塔坑位
{
    QPoint pos[] =
    {
        QPoint(125, 375),
        QPoint(375, 250),
        QPoint(750, 125),
        QPoint(625, 500)
    };

    for (int i = 0; i < 4; ++i) {
        MeleeTowerPosition.push_back(TowerPosition(pos[i]));
    }
}

void GameScene::AddRangedTowerPosition() //手动添加远战塔坑位
{
    QPoint pos[] =
    {
        QPoint(0, 125),
        QPoint(0, 375),
        QPoint(250, 125),
        QPoint(500, 125),
        QPoint(625, 375)
    };

    for (int i = 0; i < 5; ++i) {
        RangedTowerPosition.push_back(TowerPosition(pos[i]));
    }
}

void GameScene::mousePressEvent(QMouseEvent *event)
{
    QPoint pressPos = event->pos(); // 得到鼠标点击的位置
    if (Qt::RightButton == event->button()) // 如果是鼠标右键点击
      {
          auto it_1 = RangedTowerPosition.begin();
          while (it_1 != RangedTowerPosition.end()) // 遍历所有的远程塔坑位
          {
              if (it_1->InPosition(pressPos) && it_1->HasTower()) // 如果鼠标点击的位置在防御塔坑的范围内并且有防御塔
              {
                  for (RangedTower *tower : CurrentRangedTower)
                  {
                      if (tower->getPosition() == it_1->GetMidPosition()) {
                          currentTower = tower; // 设置当前选中的塔
                          break;
                      }
                  }
                  if (currentTower) {
                      contextMenu->exec(event->globalPos()); // 显示右键菜单
                  }
                  return;
              }
              ++it_1;
          }

          auto it_2 = MeleeTowerPosition.begin();
          while (it_2 != MeleeTowerPosition.end()) // 遍历所有的近战塔坑位
          {
              if (it_2->InPosition(pressPos) && it_2->HasTower()) // 如果鼠标点击的位置在防御塔坑的范围内并且有防御塔
              {
                  for (MeleeTower *tower : CurrentMeleeTower)
                  {
                      if (tower->getPosition() == it_2->GetMidPosition()) {
                          currentTower = tower; // 设置当前选中的塔
                          break;
                      }
                  }
                  if (currentTower) {
                      contextMenu->exec(event->globalPos()); // 显示右键菜单
                  }
                  return;
              }
              ++it_2;
          }
      }

    auto it_1 = RangedTowerPosition.begin();
    while (it_1 != RangedTowerPosition.end()) // 遍历所有的防御塔坑
    {
        if (Qt::LeftButton == event->button()) // 如果是鼠标左键点击
        {
            if (it_1->InPosition(pressPos) && !it_1->HasTower()&& myMoney >= 100) // 如果鼠标点击的位置在防御塔坑的范围内并且没有防御塔
            {
                RangedTower *tower = new RangedTower(it_1->GetMidPosition(), this, 5, 300, 300,2,true); // 创建一个新的防御塔
                CurrentRangedTower.push_back(tower); // 把这个防御塔放到储存防御塔的list中
                it_1->SetTower(true); // 设置这个防御塔坑内有防御塔了
                myMoney -= 100;//花费100金币
                update(); // 更新地图
                break; // 进行了一次操作，可以直接退出循环了
            }
        }
        ++it_1;
    }
    auto it_2 = MeleeTowerPosition.begin();
    while (it_2 != MeleeTowerPosition.end()) // 遍历所有的防御塔坑
    {
        if (Qt::LeftButton == event->button()) // 如果是鼠标左键点击
        {
            if (it_2->InPosition(pressPos) && !it_2->HasTower()&& myMoney >= 80) // 如果鼠标点击的位置在防御塔坑的范围内并且没有防御塔
            {
                MeleeTower *tower = new MeleeTower(it_2->GetMidPosition(), this, 10, 200, 150, 2, true); // 创建一个新的防御塔
                CurrentMeleeTower.push_back(tower); // 把这个防御塔放到储存防御塔的list中
                it_2->SetTower(true); // 设置这个防御塔坑内有防御塔了
                myMoney -= 80;//花费80金币
                update(); // 更新地图
                break; // 进行了一次操作，可以直接退出循环了
            }
        }
        ++it_2;
    }
}

void GameScene::Win()
{
    myGameOver = true;
    updateTimer->stop();
    spawnTimer->stop();
    gameStatus = 1;
    update();

}

void GameScene::Lost()
{
    myGameOver = true;
    updateTimer->stop();
    spawnTimer->stop();
    gameStatus = 2;
    update();
}

QList<Enemy *> GameScene::getEnemies() const
{
    return enemies;
}

QList<Tower *> GameScene::getTowers() const
{
    QList<Tower *> allTowers;
    foreach(MeleeTower * tower,CurrentMeleeTower)
    {
        allTowers.push_back(tower);
    }
    foreach(RangedTower * tower,CurrentRangedTower)
    {
        allTowers.push_back(tower);
    }
    return allTowers;
}

QList<MeleeTower*> GameScene::getMeleeTowers()
{
    return CurrentMeleeTower;
}

bool GameScene::isGameOver() const
{
    return myGameOver;
}

void GameScene::updateGame()
{
    if (myGameOver || isPaused) return;
    //已经打完所有的敌人

    if(spawnedEnemies == totalEnemies && enemies.empty())
    {
        Win();
    }
    // 处理敌人自动拾取词缀
        for (int i = 0; i < enemyModifierLibrary.size(); ++i) {
            Modifier *modifier = enemyModifierLibrary[i];
            bool installed = false;
            for (Enemy *enemy : enemies) {
                if (enemy->getModifiers().size() < 2) {
                    enemy->addModifier(modifier);
                    installed = true;
                    break;
                }
            }
            if (installed) {
                enemyModifierLibrary.removeAt(i);
                --i;
            }
        }


    for (Enemy *enemy : enemies) {
        if (enemy->isAlive()) {
            enemy->move();
        }
    }

    for (MeleeTower *tower : CurrentMeleeTower) {
        tower->AttackEnemy();
    }
    for (RangedTower *tower : CurrentRangedTower) {
        tower->AttackEnemy();
    }

    update(); // 刷新界面
}


void GameScene::installModifier()
{
    if (!currentTower) return;

    QMenu modifierMenu(this);
    if (auto meleeTower = dynamic_cast<MeleeTower *>(currentTower)) {
        for (Modifier *modifier : meleeModifierLibrary) {
            QAction *action = new QAction(modifier->getName(), this);
            connect(action, &QAction::triggered, [=]() {
                meleeModifierLibrary.removeOne(modifier);
                currentTower->addModifier(modifier);
            });
            modifierMenu.addAction(action);
        }
    } else if (auto rangedTower = dynamic_cast<RangedTower *>(currentTower)) {
        for (Modifier *modifier : rangedModifierLibrary) {
            QAction *action = new QAction(modifier->getName(), this);
            connect(action, &QAction::triggered, [=]() {
                rangedModifierLibrary.removeOne(modifier);
                currentTower->addModifier(modifier);
            });
            modifierMenu.addAction(action);
        }
    }

    modifierMenu.exec(QCursor::pos());
}

void GameScene::uninstallModifier()
{
    if (!currentTower) return;

    QMenu modifierMenu(this);
    QList<Modifier *> installedModifiers = currentTower->getModifiers();
    for (Modifier *modifier : installedModifiers) {
        QAction *action = new QAction(modifier->getName(), this);
        connect(action, &QAction::triggered, [=]() {
            Modifier *removedModifier = currentTower->removeModifier(modifier);
            if (removedModifier) {
                if (auto meleeTower = dynamic_cast<MeleeTower *>(currentTower)) {
                    meleeModifierLibrary.append(removedModifier);
                } else if (auto rangedTower = dynamic_cast<RangedTower *>(currentTower)) {
                    rangedModifierLibrary.append(removedModifier);
                }
                update(); // 更新界面以显示卸下的词缀
            }
        });
        modifierMenu.addAction(action);
    }

    modifierMenu.exec(QCursor::pos());
}


void GameScene::spawnEnemy()
{
    if (spawnedEnemies >= totalEnemies || myGameOver) return;

    // 随机选择一个出生点
    int spawnIndex = qrand() % 2;
    QList<WayPoint *> waypoints = (spawnIndex == 0) ? WayPointList_1 : WayPointList_2;

    // 创建敌人并添加到列表
    Enemy *enemy;
        if (qrand() % 2 == 0) {
            enemy = new Enemy(waypoints.first(), this);
        } else {
            enemy = new EnemyPlus(waypoints.first(), this);
        }

    enemies.append(enemy);
    ++spawnedEnemies;
}

void GameScene::removeEnemy(Enemy *enemy)
{
        Q_ASSERT(enemy);
        enemies.removeOne(enemy); // 从敌人列表中移除死亡的敌人
        delete enemy; // 删除敌人对象
        myMoney += 100;
        enemyKilled++;
        enemyForCreateItem++;
        if(enemyForCreateItem == 3)
        {
             enemyForCreateItem = 0;
             clearScreenItem->addItem(1);
        }
        // 敌人死亡时掉modifier
            int modifierType = rand() % 4;
            Modifier *modifier = nullptr;

            switch (modifierType)
            {
            case 0:
                modifier = new BerserkModifier();
                break;
            case 1:
                modifier = new IceModifier();
                break;
            case 2:
                modifier = new AoeModifier();
                break;
            case 3:
                modifier = new BleedModifier();
                break;
            }


            if (modifier) {
               if(dynamic_cast<BleedModifier*>(modifier))
               {
                   rangedModifierLibrary.append(modifier);
               }
               else {
                    meleeModifierLibrary.append(modifier);
               }
            }

}

//敌人的词缀是自动装载
void GameScene::removeTower(Tower *tower)
{
    Q_ASSERT(tower);

    // 移除近战塔
    auto meleeIt = std::find(CurrentMeleeTower.begin(), CurrentMeleeTower.end(), tower);
    if (meleeIt != CurrentMeleeTower.end()) {
        CurrentMeleeTower.erase(meleeIt);
        delete tower;     
    }

    // 移除远程塔
    auto rangedIt = std::find(CurrentRangedTower.begin(), CurrentRangedTower.end(), tower);
    if (rangedIt != CurrentRangedTower.end()) {
        CurrentRangedTower.erase(rangedIt);
        delete tower;
    }

    // 随机生成一个modifier并添加到词缀库中
        int modifierType = rand() % 2;
        Modifier *modifier = nullptr;

        switch (modifierType) {
        case 0:
            modifier = new FlashModifier();
            break;
        case 1:
            modifier = new SpeedModifier();
            break;
        }

        if (modifier) {
                // 先尝试装载到敌人
                for (Enemy *enemy : enemies) {
                    if (enemy->getModifiers().size() < 2) {
                        enemy->addModifier(modifier);
                        return;
                    }
                }

                // 如果装载不了，则添加到词缀库
                enemyModifierLibrary.append(modifier);
            }
        return;
}

void GameScene::saveGame(const QString &filePath)
{
    QJsonObject gameState;

    // 保存路径点信息
    QJsonArray path1Array;
    for (WayPoint *waypoint : WayPointList_1) {
        QJsonArray point;
        point.append(waypoint->GetThisWayPoint().x());
        point.append(waypoint->GetThisWayPoint().y());
        path1Array.append(point);
    }
    gameState["path1"] = path1Array;

    QJsonArray path2Array;
    for (WayPoint *waypoint : WayPointList_2) {
        QJsonArray point;
        point.append(waypoint->GetThisWayPoint().x());
        point.append(waypoint->GetThisWayPoint().y());
        path2Array.append(point);
    }
    gameState["path2"] = path2Array;

    // 保存防御塔信息
    QJsonArray meleeTowersArray;
    for (MeleeTower *tower : CurrentMeleeTower) {
        QJsonObject towerObject;
        towerObject["position"] = QJsonArray({tower->getPosition().x(), tower->getPosition().y()});
        towerObject["hp"] = tower->getMyHP();
        towerObject["damage"] = tower->getMyHurt();
        towerObject["range"] = tower->getMyAttackRange();
        meleeTowersArray.append(towerObject);
    }
    gameState["meleeTowers"] = meleeTowersArray;

    QJsonArray rangedTowersArray;
    for (RangedTower *tower : CurrentRangedTower) {
        QJsonObject towerObject;
        towerObject["position"] = QJsonArray({tower->getPosition().x(), tower->getPosition().y()});
        towerObject["hp"] = tower->getMyHP();
        towerObject["damage"] = tower->getMyHurt();
        towerObject["range"] = tower->getMyAttackRange();
        rangedTowersArray.append(towerObject);
    }
    gameState["rangedTowers"] = rangedTowersArray;

    // 保存敌人信息
    QJsonArray enemiesArray;
    for (Enemy *enemy : enemies) {
        QJsonObject enemyObject;
        enemyObject["position"] = QJsonArray({enemy->get_pos().x(), enemy->get_pos().y()});
        enemyObject["hp"] = enemy->getMyCurrentHP();
        enemiesArray.append(enemyObject);
    }
    gameState["enemies"] = enemiesArray;

    // 保存到文件
    QFile saveFile(filePath);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonDocument saveDoc(gameState);
    saveFile.write(saveDoc.toJson());
}

void GameScene::loadGame(const QString &filePath)
{
    QFile loadFile(filePath);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    QJsonObject gameState = loadDoc.object();

    // 加载路径点信息
    QJsonArray path1Array = gameState["path1"].toArray();
    WayPointList_1.clear();
    for (int i = 0; i < path1Array.size(); ++i) {
        QJsonArray point = path1Array[i].toArray();
        WayPoint *waypoint = new WayPoint(QPoint(point[0].toInt(), point[1].toInt()));
        if (!WayPointList_1.isEmpty()) {
            WayPointList_1.last()->SetNextWayPoint(waypoint);
        }
        WayPointList_1.push_back(waypoint);
    }

    QJsonArray path2Array = gameState["path2"].toArray();
    WayPointList_2.clear();
    for (int i = 0; i < path2Array.size(); ++i) {
        QJsonArray point = path2Array[i].toArray();
        WayPoint *waypoint = new WayPoint(QPoint(point[0].toInt(), point[1].toInt()));
        if (!WayPointList_2.isEmpty()) {
            WayPointList_2.last()->SetNextWayPoint(waypoint);
        }
        WayPointList_2.push_back(waypoint);
    }

    // 加载防御塔信息
    CurrentMeleeTower.clear();
    QJsonArray meleeTowersArray = gameState["meleeTowers"].toArray();
    for (int i = 0; i < meleeTowersArray.size(); ++i) {
        QJsonObject towerObject = meleeTowersArray[i].toObject();
        QPoint position(towerObject["position"].toArray()[0].toInt(), towerObject["position"].toArray()[1].toInt());
        int hp = towerObject["hp"].toInt();
        int damage = towerObject["damage"].toInt();
        int range = towerObject["range"].toInt();
        MeleeTower *tower = new MeleeTower(position, this, damage, range, hp, 2, true);
        CurrentMeleeTower.push_back(tower);
    }

    CurrentRangedTower.clear();
    QJsonArray rangedTowersArray = gameState["rangedTowers"].toArray();
    for (int i = 0; i < rangedTowersArray.size(); ++i) {
        QJsonObject towerObject = rangedTowersArray[i].toObject();
        QPoint position(towerObject["position"].toArray()[0].toInt(), towerObject["position"].toArray()[1].toInt());
        int hp = towerObject["hp"].toInt();
        int damage = towerObject["damage"].toInt();
        int range = towerObject["range"].toInt();
        RangedTower *tower = new RangedTower(position, this, damage, range,hp, 2, true);
        CurrentRangedTower.push_back(tower);
    }

    // 加载敌人信息
    enemies.clear();
    QJsonArray enemiesArray = gameState["enemies"].toArray();
    for (int i = 0; i < enemiesArray.size(); ++i) {
        QJsonObject enemyObject = enemiesArray[i].toObject();
        QPoint position(enemyObject["position"].toArray()[0].toInt(), enemyObject["position"].toArray()[1].toInt());
        int hp = enemyObject["hp"].toInt();
        Enemy *enemy = new Enemy(WayPointList_1.first(), this); // 假设所有敌人都从路径1开始，可以根据实际情况修改
        enemy->setMyCurrentHP(hp) ;
        enemy->setMyCurrentPosition(position);
        enemies.append(enemy);
    }

    update();
}

void GameScene::saveMap()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Map"), "", tr("JSON Files (*.json);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    MapData mapData;
    for (WayPoint *wayPoint : WayPointList_1) {
        mapData.addWayPoint(wayPoint->GetThisWayPoint(), 0);
    }
    for (WayPoint *wayPoint : WayPointList_2) {
        mapData.addWayPoint(wayPoint->GetThisWayPoint(), 1);
    }
    for (TowerPosition &position : MeleeTowerPosition) {

        mapData.addMeleeTowerPosition(position.GetMidPosition()-QPoint(62,62));
    }
    for (TowerPosition &position : RangedTowerPosition) {
        mapData.addRangedTowerPosition(position.GetMidPosition()-QPoint(62,62));
    }

    QJsonDocument saveDoc(mapData.toJson());
    QFile saveFile(fileName);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }
    saveFile.write(saveDoc.toJson());
}

void GameScene::loadMap()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Map"), "", tr("JSON Files (*.json);;All Files (*)"));
    if (fileName.isEmpty()) {
        return;
    }

    QFile loadFile(fileName);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open load file.");
        return;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    MapData mapData;
    mapData.fromJson(loadDoc.object());

    WayPointList_1.clear();
    WayPointList_2.clear();
    MeleeTowerPosition.clear();
    RangedTowerPosition.clear();

    // 加载路径 1 的航点
    QList<QList<QPoint>> waypoints = mapData.getWayPoints();
    if (waypoints.size() > 0) {
        WayPoint *prevWayPoint = nullptr;
        for (const QPoint &wayPointPos : waypoints[0]) {
            WayPoint *wayPoint = new WayPoint(wayPointPos);
            if (prevWayPoint) {
                prevWayPoint->SetNextWayPoint(wayPoint);
            }
            WayPointList_1.append(wayPoint);
            prevWayPoint = wayPoint;
        }
    }

    // 加载路径 2 的航点
    if (waypoints.size() > 1) {
        WayPoint *prevWayPoint = nullptr;
        for (const QPoint &wayPointPos : waypoints[1]) {
            WayPoint *wayPoint = new WayPoint(wayPointPos);
            if (prevWayPoint) {
                prevWayPoint->SetNextWayPoint(wayPoint);
            }
            WayPointList_2.append(wayPoint);
            prevWayPoint = wayPoint;
        }
    }

    // 加载近战塔位置
    QList<QPoint> meleePositions = mapData.getMeleeTowerPositions();
    for (const QPoint &pos : meleePositions) {
        MeleeTowerPosition.append(TowerPosition(pos));
    }

    // 加载远战塔位置
    QList<QPoint> rangedPositions = mapData.getRangedTowerPositions();
    for (const QPoint &pos : rangedPositions) {
        RangedTowerPosition.append(TowerPosition(pos));
    }

    update(); // 更新地图
}



void GameScene::on_Load_Button_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Load Game", "", "JSON Files (*.json)");
        if (!filePath.isEmpty()) {
            this->loadGame(filePath);
        }
}

void GameScene::on_Store_Button_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save Game", "", "JSON Files (*.json)");
        if (!filePath.isEmpty()) {
            this->saveGame(filePath);
        }
}
