#include "entity.h"
ECS::Impl::Id ECS::Entity::id_counter;
std::vector<ECS::Entity::Remover> ECS::Entity::removers;
