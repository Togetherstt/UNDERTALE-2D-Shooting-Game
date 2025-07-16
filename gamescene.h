#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QSet>

// Forward declarations to avoid circular dependencies
// We can use pointers to these types without needing their full definition here.
class Player;
class Terrain;
class Item;

class GameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GameScene(QObject *parent = nullptr);
    void removeItemFromWorld(Item* item);
    void startNewGame();

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void gameLoop();
    void spawnItem(); // This slot handles timed item spawning

private:
    // Scene setup and teardown
    void setupScene();
    void resetGame();

    // Per-frame update logic
    void handlePlayerInput();
    void updateGameObjects();
    void checkCollisions();
    void checkGameOver();

    // UI and state management
    void displayGameOver(const QString& winnerText);

    QTimer m_timer;
    QTimer m_itemSpawnTimer;

    QGraphicsPixmapItem* m_background;
    Player *m_player1;
    Player *m_player2;
    QList<Terrain*> m_terrains;
    QList<Item*> m_items; // GameScene owns and manages all items on the ground

    QSet<int> m_pressedKeys;
    bool m_gameOver;
};

#endif // GAMESCENE_H
