#pragma once
#include "Project/GameObject/GameObject.h"
#include "Project/GameObject/Character/Player/Player.h"
#include "Project/GameObject/Character/EnemyManager/Bullet/EnemyBullet.h"
#include "Project/GameObject/Character/Player/Bullet/PlayerBullet.h"

class GameScene;
class IScene;

/// <summary>
/// 敵のパラメーター（レベルによって上下する値）
/// </summary>
/// <param name="hp : 体力"></param>
/// <param name="power : 攻撃力"></param>
/// <param name="speed : 速度"></param>
struct Parameter
{
    int hp;
    int power;
    float speed;
    float searchDistance;
    int pollution;
};

class IEnemy {
public:

    IEnemy() {};
    virtual ~IEnemy() {};

    /// <summary>
    /// 初期化
    /// </summary>
    /// <param name="player : プレイヤーのポインタ"></param>
    /// <param name="position : 初期位置"></param>
    /// <param name="level : レベル(これによってパラメーターが変化する)"></param>
    virtual void Initialize(Player* player, Vector3 position, int level) = 0;
    /// <summary>
    /// 更新処理
    /// </summary>
    virtual void Update() = 0;
    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name="view"></param>
    virtual void Draw(Camera* camera) = 0;

    virtual OBBCollider* GetOBBCollider() = 0;

    bool IsAlive() { return this->isAlive_; }
    bool IsFeed() { return this->isFeed_; }

    void SetGameScene(IScene* scene) { this->gameScene = scene; };

    void SetLevel(int level) { level_ = level; }

    std::list<EnemyBullet*>* GetEnemyBullets() { return &bullet_; }

    virtual void OnCollisionWithPlayer(Player* player) = 0;

    virtual void OnCollisionWithPlayerBullet(PlayerBullet* bullet) = 0;

    int GetAttack() { return param.power; }
    void SetAttack(int atk) { param.power = atk; }

    void SpawnMove();

    void TitleMove();

    bool isSpawn = false;

    const Vector3& GetPos() { return worldTransform_.translate(); }

private:
    Vector3 Jump(const Vector3& initialPosition, float jumpHeight, float jumpDuration, float currentTime);

    void Shake(const Vector3& initialPosition, float jumpHeight, float jumpDuration, float currentTime);

    Vector3 JumpScale(const Vector3& initialScale, float jumpHeight, float jumpDuration, float currentTime);

    Vector3 calculateJumpScale(float currentTime, float jumpDuration);

    Vector3 calculateLandingScale(float currentTime, float jumpDuration, const Vector3& initialScale);

    float CalculateAngle(const Vector3& current, const Vector3& target);

protected:
    //player
    Player* player_;
    //worldTransform
    WorldTransform worldTransform_;
    //速度
    Vector3 velocity_;
    //モデル
    Model model_;
    //生存しているかのフラグ
    bool isAlive_;
    bool isFeed_;
    //Enemyの持つ弾を可変長配列で管理
    std::list<EnemyBullet*> bullet_;
    //playerとの距離
    float distanceToPlayer_;

    int level_;

    Parameter param{};

    IScene* gameScene = nullptr;


    bool isFind = false;
    const int maxFindFrame = 120;
    int findFrame = 0;

    int flame_;

    //アニメーションのための
    float animationTime_;

    //元の位置
    Vector3 originalPos_;

    Vector3 originalScale_;

    bool isAnime_ = false;
};