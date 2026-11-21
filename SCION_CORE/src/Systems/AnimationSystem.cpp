#include "Core/Systems/AnimationSystem.h"
#include "Core/ECS/Components/AnimationComponent.h"
#include "Core/ECS/Components/SpriteComponent.h"
#include "Core/ECS/Components/TransformComponent.h"
#include "Core/CoreUtilities/CoreUtilities.h"
#include "Core/ECS/Registry.h"

#include "Logger/Logger.h"
#include "Core/Profiling/ProfileCollector.h"
#include <SDL3/SDL.h>

using namespace Scion::Core::ECS;
using namespace Scion::Rendering;

namespace Scion::Core::Systems
{

void AnimationSystem::Update( Scion::Core::ECS::Registry& registry, Scion::Rendering::Camera2D& camera )
{

	SCION_SYSTEM_ZONE( "AnimationSystem" );

	auto view = registry.GetRegistry().view<AnimationComponent, SpriteComponent, TransformComponent>();
	if ( view.size_hint() < 1 )
		return;

	for ( auto entity : view )
	{
		const auto& transform = view.get<TransformComponent>( entity );
		auto& sprite = view.get<SpriteComponent>( entity );
		auto& animation = view.get<AnimationComponent>( entity );

		// We don't want to check if entities with UIComponents are out of the camera.
		// Since they use a different camera.
		if ( !registry.GetRegistry().all_of<UIComponent>( entity ) &&
			 !Scion::Core::EntityInView( transform, sprite.width, sprite.height, camera ) )
			continue;

		if ( animation.numFrames <= 0 )
			continue;

		// if we are not looped and the current from == num frames, skip
		if ( !animation.bLooped && animation.currentFrame >= animation.numFrames - 1 )
			continue;

		// Get the current frame
		animation.currentFrame =
			( ( SDL_GetTicks() - animation.startTime ) * animation.frameRate / 1000 ) % animation.numFrames;

		if ( animation.bVertical )
		{
			sprite.uvs.v = ( animation.currentFrame + sprite.startY ) * sprite.uvHeightUnpadded + sprite.texelOffsetY;

			sprite.uvs.u = sprite.startX * sprite.uvWidthUnpadded + sprite.texelOffsetX;
		}
		else
		{
			sprite.uvs.u = ( animation.currentFrame + sprite.startX ) * sprite.uvWidthUnpadded + sprite.texelOffsetX;

			sprite.uvs.v = sprite.startY * sprite.uvHeightUnpadded + sprite.texelOffsetY;
		}
	}
}

void AnimationSystem::CreateAnimationSystemLuaBind( sol::state& lua, Scion::Core::ECS::Registry& registry )
{
	auto& pCamera = registry.GetContext<std::unique_ptr<Camera2D>>();

	SCION_ASSERT( pCamera && "A camera must exist in the current scene!" );

	lua.new_usertype<AnimationSystem>(
		"AnimationSystem",
		sol::call_constructor,
		sol::constructors<AnimationSystem()>(),
		"update",
		[ & ]( AnimationSystem& system, Registry& reg ) { system.Update( reg, *pCamera ); } );
}
} // namespace Scion::Core::Systems
