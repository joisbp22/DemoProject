#include "ComponentGameManager.h"

#include "ComponentEnemyManager.h"
#include "ComponentEnemy.h"

#include "GameObjectSystem/GameObject.h"

#include "Serialization/SerializationModule.h"

#include "Application/ApplicationBase.h"

#include <random>

IMPLEMENT_COMPONENT(ComponentGameManager)

ComponentGameManager::ComponentGameManager()
{

}

ComponentGameManager::~ComponentGameManager()
{

}

bool ComponentGameManager::Deserialize(std::shared_ptr<SerializerBase> serializer)
{
	bool success = true;

	success &= DESERIALIZE_VARIABLE(m_InitialEnemyCount, ComponentGameManager);
	assert(success);

	success &= DESERIALIZE_VARIABLE(m_EnemyManagerComponent, ComponentGameManager);
	assert(success);

	return success;
}

void ComponentGameManager::PostInitialize()
{
	StartNewGame();
}

void ComponentGameManager::Update(unsigned long deltaTime)
{
	std::shared_ptr<const ComponentEnemyManager> enemyManagerComponent = m_EnemyManagerComponent.lock();
	if (enemyManagerComponent != nullptr && enemyManagerComponent->GetSpawnedEnemiesCount() == 0)
	{
		StartNewGame();
	}
}

void ComponentGameManager::StartNewGame()
{
	std::shared_ptr<ComponentEnemyManager> enemyManagerComponent = m_EnemyManagerComponent.lock();
	if (enemyManagerComponent != nullptr)
	{
		std::shared_ptr<ApplicationBase> application = ApplicationBase::GetInstance<ApplicationBase>().lock();
		if (application != nullptr)
		{
			const ApplicationSettings& applicationSettings = application->GetSettings();
			unsigned int screenWidth = applicationSettings.GetScreenWidth();
			unsigned int screenHeight = applicationSettings.GetScreenHeight();

			Vector2D enemyHalfExtent = enemyManagerComponent->GetDefaultEnemyColliderHalfExtent();

			unsigned int screenPartitionCount[2] = { 1, 1 };
			unsigned int partitionDimensionIndex = 0;
			while (screenPartitionCount[0] * screenPartitionCount[1] < m_InitialEnemyCount)
			{
				++screenPartitionCount[partitionDimensionIndex];
				++partitionDimensionIndex;
				partitionDimensionIndex %= 2;
			}

			unsigned int finalPartitionCount = screenPartitionCount[0] * screenPartitionCount[1];

			std::unordered_map<unsigned int, unsigned int> grid;
			for (unsigned int i = 0; i < finalPartitionCount; ++i)
			{
				grid[i] = i;
			}

			unsigned int xPartitionSize = screenWidth / screenPartitionCount[0];
			unsigned int yPartitionSize = screenHeight / screenPartitionCount[1];

			for (unsigned int i = 0; i < m_InitialEnemyCount; ++i)
			{
				unsigned int randomGridElement = rand() % grid.size();
				unsigned int randomScreenPartitionIndex = grid[randomGridElement];
				grid.erase(randomGridElement);

				unsigned int columnIndex = randomScreenPartitionIndex % screenPartitionCount[0];
				unsigned int rowIndex = randomScreenPartitionIndex / screenPartitionCount[0];

				unsigned int minPositionX = columnIndex * xPartitionSize + (unsigned int)enemyHalfExtent.m_X;
				unsigned int maxPositionX = (columnIndex + 1) * xPartitionSize - (unsigned int)enemyHalfExtent.m_X;
				unsigned int minPositionY = rowIndex * yPartitionSize + (unsigned int)enemyHalfExtent.m_Y;
				unsigned int maxPositionY = (rowIndex + 1) * yPartitionSize - (unsigned int)enemyHalfExtent.m_Y;

				Point2D finalPosition;
				finalPosition.m_X = (float)(rand() % (maxPositionX - minPositionX) + minPositionX);
				finalPosition.m_Y = (float)(rand() % (maxPositionY - minPositionY) + minPositionY);

				std::shared_ptr<ComponentEnemy> enemyComponent = enemyManagerComponent->SpawnEnemy().lock();
				if (enemyComponent != nullptr)
				{
					enemyComponent->Spawn(finalPosition);

					std::shared_ptr<GameObject> newEnemyOwner = enemyComponent->GetOwner().lock();
					if (newEnemyOwner != nullptr)
					{
						newEnemyOwner->Update(0);
					}
				}
			}
		}
	}
}
