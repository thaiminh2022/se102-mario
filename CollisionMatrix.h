#pragma once
#include <cinttypes>


typedef std::uint8_t matrixType;

enum class CollisionMatrixLayer : matrixType
{
	Default = 0,
	EnemyProjectile,
	CheepCheeps,
	Player,
	Ground,
};

class CollisionMatrix
{
	bool matrix[5][5];
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

