#include "gamescene.h"
#include "customization.h"
#include "player.h"
#include "terrain.h"
#include "weapon.h"
#include "equipment.h"
#include "armor.h"
#include <QKeyEvent>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QMutableListIterator>
#include <cstdlib>
#include <ctime>
#include <QDebug>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent), m_gameOver(false)
{
    srand(time(0));
}

void GameScene::startNewGame()
{
    // 停止可能正在运行的计时器
    m_timer.stop();
    m_itemSpawnTimer.stop();

    // 重置场景和游戏状态
    setupScene();

    // 连接并启动计时器
    connect(&m_timer, &QTimer::timeout, this, &GameScene::gameLoop);
    m_timer.start(1000 / TARGET_FPS);

    connect(&m_itemSpawnTimer, &QTimer::timeout, this, &GameScene::spawnItem);
    m_itemSpawnTimer.start(3000);
}

void GameScene::setupScene()
{
    // Clear all previous items from the scene and internal lists
    clear();
    m_terrains.clear();
    m_items.clear();

    m_background = new QGraphicsPixmapItem(QPixmap(BACKGROUND_SPRITE));
    m_background->setZValue(-1);
    addItem(m_background);

    // Create Players
    m_player1 = new Player(1, this);
    m_player2 = new Player(2, this);
    m_player1->setPos(150, 400);
    m_player2->setPos(SCREEN_WIDTH - 250, 400);
    addItem(m_player1);
    addItem(m_player2);

     // Terrain* ground = new Terrain(Terrain::Land, 0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, 26);
     // addItem(ground);
     // m_terrains.append(ground);

     Terrain* platform1 = new Terrain(Terrain::Grass, 50, 650, 136*2, 26);
     addItem(platform1);
     m_terrains.append(platform1);

     Terrain* platform2 = new Terrain(Terrain::Ice, SCREEN_WIDTH - 350, 650, 136*2, 26);
     addItem(platform2);
     m_terrains.append(platform2);

     Terrain* platform3 = new Terrain(Terrain::Ice, 50, 350, 136*2, 26);
     addItem(platform3);
     m_terrains.append(platform3);

     Terrain* platform4 = new Terrain(Terrain::Grass, SCREEN_WIDTH - 350, 350, 136*2, 26);
     addItem(platform4);
     m_terrains.append(platform4);

    Terrain* midPlatform = new Terrain(Terrain::Land, (200+136+SCREEN_WIDTH - 500)/2, 500, 136, 26);
    addItem(midPlatform);
    m_terrains.append(midPlatform);

    Terrain* midPlatform1 = new Terrain(Terrain::Land, (200+136+SCREEN_WIDTH - 500)/2, 200, 136, 26);
    addItem(midPlatform1);
    m_terrains.append(midPlatform1);

    m_gameOver = false;
}

void GameScene::gameLoop()
{
    if (m_gameOver) return;

    handlePlayerInput();
    updateGameObjects();
    // checkCollisions();
    checkGameOver();

    advance();
}

void GameScene::updateGameObjects() {
    m_player1->update(m_terrains);
    m_player2->update(m_terrains);

    QMutableListIterator<Item*> i(m_items);
    while (i.hasNext()) {
        Item* item = i.next();
        // 更新物品自身的状态（例如，重力）
        item->update(m_terrains);

        // 检查物品是否掉出了屏幕底部
        if (item->y() > this->height() + 50) { // 增加50像素的缓冲区
            // 从列表中移除
            i.remove();
            // 安全地调度删除该对象
            item->deleteLater();
        }
    }
}

// void GameScene::checkCollisions() {
//     // Use QMutableListIterator to safely remove items while iterating
//     QMutableListIterator<Item*> i(m_items);
//     while (i.hasNext()) {
//         Item* item = i.next();
//         bool pickedUp = false;

//             if (m_player1->collidesWithItem(item)) {
//                 // Pickup logic now fully handled in Player class.
//                 // It will handle swapping weapons or consuming items.
//                 m_player1->pickupItem(item);
//                 pickedUp = true;
//             }
//             else if (m_player2->collidesWithItem(item)) {
//             m_player2->pickupItem(item);
//             pickedUp = true;
//         }

//         if (pickedUp) {
//             // The item is now "owned" by the player, so remove it
//             // from the scene's master list of world items.
//             i.remove();
//         }
//     }
// }

void GameScene::removeItemFromWorld(Item* item) {
    if (item) {
        m_items.removeOne(item); // 从内部列表中移除
        removeItem(item);      // 从场景的图形显示中移除
    }
}

void GameScene::spawnItem() {
    if (m_gameOver) return;

    int itemType = rand() % 10;
    Item* newItem = nullptr;
    qreal xPos = (rand() % (SCREEN_WIDTH - 200)) + 100;

    switch (itemType) {
        case 0: newItem = new Weapon(Weapon::Rifle, this); break;
        case 1: newItem = new Weapon(Weapon::SniperRifle, this); break;
        case 2: newItem = new Weapon(Weapon::SolidBall, this); break;
        case 3: newItem = new Weapon(Weapon::Knife, this); break;
        case 4: newItem = new Equipment(Equipment::Bandage, this); break;
        case 5: newItem = new Equipment(Equipment::MedKit, this); break;
        case 6: newItem = new Equipment(Equipment::Adrenaline, this); break;
        case 7: newItem = new Armor(Armor::Chainmail, this); break;
        case 8: newItem = new Armor(Armor::BulletproofVest, this); break;
        case 9: newItem = new Equipment(Equipment::ExpOrb, this); break;
    }

    if (newItem) {
        newItem->setPos(xPos, 0);
            addItem(newItem);
        m_items.append(newItem);
    }
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    if (m_gameOver && event->key() == Qt::Key_R) {
        resetGame();
        return;
    }
    if (!event->isAutoRepeat()) {
        m_pressedKeys.insert(event->key());
    }
    QGraphicsScene::keyPressEvent(event);
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if (!event->isAutoRepeat()) {
        m_pressedKeys.remove(event->key());
    }

    if (event->key() == Qt::Key_S) m_player1->setCrouching(false);
    if (event->key() == Qt::Key_Down) m_player2->setCrouching(false);

    QGraphicsScene::keyReleaseEvent(event);
}

void GameScene::handlePlayerInput()
{
    if (m_pressedKeys.contains(Qt::Key_A)) m_player1->moveLeft();
    if (m_pressedKeys.contains(Qt::Key_D)) m_player1->moveRight();
    if (m_pressedKeys.contains(Qt::Key_W)) m_player1->jump();
    if (m_pressedKeys.contains(Qt::Key_S)) m_player1->crouch(); // Only handles crouching animation
    if (m_pressedKeys.contains(Qt::Key_F)) m_player1->fire();

    if (m_pressedKeys.contains(Qt::Key_Left)) m_player2->moveLeft();
    if (m_pressedKeys.contains(Qt::Key_Right)) m_player2->moveRight();
    if (m_pressedKeys.contains(Qt::Key_Up)) m_player2->jump();
    if (m_pressedKeys.contains(Qt::Key_Down)) m_player2->crouch();
    if (m_pressedKeys.contains(Qt::Key_L)) m_player2->fire();
}

void GameScene::checkGameOver() {
    if (m_player1->getHealth() <= 0) {
        displayGameOver("Player 2 Wins!");
        m_gameOver = true;
    } else if (m_player2->getHealth() <= 0) {
        displayGameOver("Player 1 Wins!");
        m_gameOver = true;
    }
}

void GameScene::displayGameOver(const QString& winnerText) {
    m_itemSpawnTimer.stop();

    QGraphicsRectItem* overlay = new QGraphicsRectItem(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    overlay->setBrush(QColor(0, 0, 0, 150));
    overlay->setZValue(4);
    addItem(overlay);

    QGraphicsTextItem* gameOverText = new QGraphicsTextItem(winnerText);
    gameOverText->setFont(QFont("Arial", 50, QFont::Bold));
    gameOverText->setDefaultTextColor(Qt::white);
    gameOverText->setPos(SCREEN_WIDTH/2 - gameOverText->boundingRect().width()/2, SCREEN_HEIGHT/2 - 100);
    gameOverText->setZValue(5);
    addItem(gameOverText);

    QGraphicsTextItem* restartText = new QGraphicsTextItem("Press 'R' to Restart");
    restartText->setFont(QFont("Arial", 24));
    restartText->setDefaultTextColor(Qt::white);
    restartText->setPos(SCREEN_WIDTH/2 - restartText->boundingRect().width()/2, SCREEN_HEIGHT/2);
    restartText->setZValue(5);
    addItem(restartText);
}

void GameScene::resetGame() {
    // This function will clear everything and call setupScene again
    QList<QGraphicsItem*> all = items();
    for (QGraphicsItem* i : all) {
        removeItem(i);
        delete i;
    }
    setupScene();
    m_itemSpawnTimer.start(3000);
}
