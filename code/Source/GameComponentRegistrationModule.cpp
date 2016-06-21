#include "GameComponentRegistrationModule.h"

#include "Components/ComponentProjectileManager.h"
#include "Components/ComponentProjectile.h"
#include "Components/ComponentCanon.h"
#include "Components/ComponentEnemyManager.h"
#include "Components/ComponentEnemy.h"
#include "Components/ComponentGameManager.h"
#include "Components/ComponentClock.h"

IMPLEMENT_MODULE(GameComponentRegistrationModule)

GameComponentRegistrationModule::GameComponentRegistrationModule()
	:ModuleBase()
{

}

GameComponentRegistrationModule::~GameComponentRegistrationModule()
{

}

void GameComponentRegistrationModule::RegisterSerializables(Factory<Serializable>& factory)
{
	REGISTER_FACTORY_CONSTRUCTABLE(ComponentProjectileManager, factory);
	REGISTER_FACTORY_CONSTRUCTABLE(ComponentProjectile, factory);
	REGISTER_FACTORY_CONSTRUCTABLE(ComponentCanon, factory);
	REGISTER_FACTORY_CONSTRUCTABLE(ComponentEnemyManager, factory);
	REGISTER_FACTORY_CONSTRUCTABLE(ComponentEnemy, factory);
	REGISTER_FACTORY_CONSTRUCTABLE(ComponentGameManager, factory);
	REGISTER_FACTORY_CONSTRUCTABLE(ComponentClock, factory);
}
