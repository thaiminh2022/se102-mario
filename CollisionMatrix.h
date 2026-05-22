#pragma once
#include <cinttypes>
#include "Debug.h"

typedef std::uint8_t matrixType;

enum class CollisionMatrixLayer : matrixType
{
	Default = 0,
	EnemyProjectile,
	CheepCheeps,
	Player,
	Ground,
	Enemies,
	Shells,
	MarioFireball
};

class CollisionMatrix
{
	bool matrix[8][8];
	static CollisionMatrix* _instance;

	CollisionMatrix()
	{
		for (auto& row : matrix)
		{
			for (bool& col : row)
			{
				col = true;
			}
		}

		SetCollisionValue(CollisionMatrixLayer::CheepCheeps, CollisionMatrixLayer::Ground, false);
		SetCollisionValue(CollisionMatrixLayer::Enemies, CollisionMatrixLayer::Enemies, false);
		SetCollisionValue(CollisionMatrixLayer::Shells, CollisionMatrixLayer::Enemies, false);
		SetCollisionValue(CollisionMatrixLayer::MarioFireball, CollisionMatrixLayer::MarioFireball, false);
		SetCollisionValue(CollisionMatrixLayer::EnemyProjectile, CollisionMatrixLayer::EnemyProjectile, false);
		SetCollisionValue(CollisionMatrixLayer::EnemyProjectile, CollisionMatrixLayer::Enemies, false);
		SetCollisionValue(CollisionMatrixLayer::MarioFireball, CollisionMatrixLayer::Player, false);
		SetCollisionValue(CollisionMatrixLayer::EnemyProjectile, CollisionMatrixLayer::Ground, false);
		SetCollisionValue(CollisionMatrixLayer::Player, CollisionMatrixLayer::Player, false);
		SetCollisionValue(CollisionMatrixLayer::Enemies, CollisionMatrixLayer::Ground, true);

	}

	void SetCollisionValue(CollisionMatrixLayer a, CollisionMatrixLayer b, const bool value)
	{
		matrix[static_cast<matrixType>(a)][static_cast<matrixType>(b)] = value;
		matrix[static_cast<matrixType>(b)][static_cast<matrixType>(a)] = value;
	}
	static CollisionMatrix* GetInstance()
	{
		if (_instance == nullptr)
			_instance = new CollisionMatrix;

		return _instance;
	}

public:
	static bool IsLayerCollide(CollisionMatrixLayer a, CollisionMatrixLayer b)
	{
		return GetInstance()->matrix[static_cast<matrixType>(a)][static_cast<matrixType>(b)];
	}
};

